/**
 * @file EventManager.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 * @date 2022-12-15
 */
#pragma once
#include <memory>
#include <mutex>
#include <filesystem>
#include <functional>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UIExecutive.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VModularPhysicsList.hh"
#include "G4GDMLParser.hh"

#ifdef ARTIE_ROOT
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMath.h"
#endif

#ifdef ARTIE_GEANT_10
#include "g4root.hh"
#else
#include "G4AnalysisManager.hh"
#endif

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Core.hh"
#include "Hit.hh"
#include "Neutron.hh"
#include "Profile.hh"

class PhysicsList;

namespace Artie
{
    struct Tuple
    {
        G4String name = "none";
        G4int index = -1;
        Tuple(G4String n, G4int i)
        : name(n), index(i)
        {}
    };
#ifdef ARTIE_PROFILING
    struct Profile
    {
        G4int calls = 0;
        G4int time = 0;

        Profile(){}
    };
#endif

    /**
     * @brief Class for handling generation of source primaries,
     * input/output, data saving, messenging, etc.
     */
    class EventManager
    {
    public:
        EventManager(EventManager const&) = delete;
        void operator=(EventManager const&) = delete;
		EventManager();
        ~EventManager();

		static void SetParticle(G4String);
#ifdef ARTIE_YAML
        static void SetConfig(YAML::Node config);
#endif

        // get the event manager
        static std::shared_ptr<EventManager>& GetEventManager() 
        { 	
            std::lock_guard<std::mutex> lock(sMutex);
			if (sInstance == nullptr) {
				sInstance = std::make_shared<EventManager>();
                sTuples.clear();
                sCurrentTupleIndex = -1;
			}return sInstance; 
		}
        inline static thread_local G4int EventID() { return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID(); }

        // Number of threads
        G4int NumberOfThreads()             { return sNumberOfThreads; }
        void NumberOfThreads(G4int threads) { sNumberOfThreads = threads;}

        // Number of runs
        G4int NumberOfRuns()             { return sNumberOfRuns; }
        void NumberOfRuns(G4int Runs) { sNumberOfRuns = Runs;}

        // Number of events
        G4int NumberOfEvents()             { return sNumberOfEvents; }
        void NumberOfEvents(G4int Events) { sNumberOfEvents = Events;}

        // Argon properties
        G4bool UseG4Definition()    { return mUseG4Definition; }
        G4double Argon36Ratio()     { return mArgon36Ratio; }
        G4double Argon38Ratio()     { return mArgon38Ratio; }
        G4double Argon40Ratio()     { return mArgon40Ratio; }
        G4double LArDensity()       { return mLArDensity; }
        G4double LArTemperature()   { return mLArTemperature; }
        G4double LArPressure()      { return mLArPressure; }

        G4double FlightPath()       { return mDetectorEntrance - mTZeroLocation; }
        inline static G4double GetNominalTOF(G4double energy)
        {
            G4double kinetic_mass = energy/NeutronMassMeV();
            G4double denominator = 1.0 - 1.0/((kinetic_mass + 1.0)*(kinetic_mass + 1.0));
            G4double correction_factor = sqrt(1.0 / denominator);
            return ((mDetectorEntrance - mTZeroLocation) / SpeedOfLight()) * correction_factor;
        }
        inline static G4double GetNominalVelocity(G4double energy)
        {
            G4double kinetic_mass = energy/NeutronMassMeV();
            G4double factor = sqrt(1.0 - 1.0/((kinetic_mass + 1.0)*(kinetic_mass + 1.0)));
            return SpeedOfLight() * factor;
        }

        // Tuple related functions
        G4String OutputFileName()           { return sOutputFileName; }
        void OutputFileName(G4String name)  { sOutputFileName = name; }
        void OpenOutputFile(G4int RunID);
        void CloseOutputFile(G4int RunID);
        G4int GetIndex(G4String);

        // event max time
        const G4double& EventMaxTime()             { return sEventMaxTime; }
        void EventMaxTime(G4double maxTime) { sEventMaxTime = maxTime; }
        inline static thread_local G4int GetNumberOfParticles()         { return mParticleName.size(); }
        inline static thread_local G4int GetNumberOfSimulatedParticles(){ return mParticleName.size(); }

        // lanl distribution
        void ConstructEnergyDistribution();
#ifdef ARTIE_ROOT
        TH1D* GetLANLEnergyDistribution()       { return mLANLEnergyDistribution.get(); }
        TH2D* GetLANLTOF()                      { return mLANLTOF.get(); }
        TH1D* GetLANLTOFProjection(G4int ii)    { return mLANLTOFProjections[ii].get(); }

        TH2D* GetnTOFTOF()                      { return mnTOFTOF.get(); }
        TH1D* GetnTOFTOFProjection(G4int ii)    { return mnTOFTOFProjections[ii].get(); }
        TH2D* GetnTOFBeamProfile()              { return mnTOFBeamProfile.get(); }
#endif
        //*************************************************************************************************//
        // Options to save various data to root files.
        void SaveParticleMaps(G4bool save)      { sSaveParticleMaps = save; }
        void SavePrimaryInfo(G4bool save)       { sSavePrimaryInfo = save; }
        void SaveHits(G4bool save)              { sSaveHits = save; }
        void SaveNeutronData(G4bool save)       { sSaveNeutronData = save; }
        void SaveNonDetectedNeutrons(G4bool save) { sSaveNonDetectedNeutrons = save; }
        void SaveProfileData(G4bool save)       { sSaveProfileData = save; }

        G4bool SaveParticleMaps()       { return sSaveParticleMaps; }
        G4bool SavePrimaryInfo()        { return sSavePrimaryInfo; }
        G4bool SaveHits()               { return sSaveHits; }
        G4bool SaveNeutronData()        { return sSaveNeutronData; }
        G4bool SaveNonDetectedNeutrons() { return sSaveNonDetectedNeutrons; }
        G4bool SaveProfileData()        { return sSaveProfileData; }

        G4double TZeroRadii()               { return mTZeroRadii; }
        G4double DetectorEntranceRadii()    { return mDetectorEntranceRadii; }
        G4double DetectorEntrance()         { return mDetectorEntrance; }
        G4double SmallestRadii()            { return mSmallestRadii; }
        G4double SmallestRadiiZBegin()      { return mSmallestRadiiZBegin; }
        G4double SmallestRadiiZEnd()        { return mSmallestRadiiZEnd; }

        void CreateTuples();
        void FillParticleMaps(G4int EventID = -1);
        void FillPrimaryInfo(G4int EventID = -1);
        void FillHits(G4int EventID = -1);
        void FillNeutronEventData(G4int EventID = -1);
        void FillNeutronRunData();
        inline static thread_local void ClearEventData()
        {
            mParticleTrackID.clear();
            mParticleName.clear();
            mParticlePDG.clear();
            mParticleParentTrackID.clear();
            mParticleAncestorTrackID.clear();
            mPrimaryData.clear();
            mHits.clear();
            mNeutronEventData.clear();
            mNeutronEventDataMap.clear();
        }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        // Add Particle info
        void AddParticleMapsFromTrack(const G4Track* track);

        inline static thread_local void AddParticleTrackID(G4int track_id, G4int location)
        { mParticleTrackID[track_id] = location; }
        inline static thread_local void AddParticleName(G4int track_id, G4String name)                 
        { mParticleName[track_id] = name; }
        inline static thread_local void AddParticlePDG(G4int track_id, G4int pdg)
        { mParticlePDG[track_id] = pdg; }
        inline static thread_local void AddParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { mParticleParentTrackID[track_id] = parent_track_id; }
        inline static thread_local void AddParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { mParticleAncestorTrackID[track_id] = ancestor_track_id; }

        inline static thread_local const G4int& GetParticleTrackID(G4int track_id)          { return mParticleTrackID[track_id]; }
        inline static thread_local const G4String& GetParticleName(G4int track_id)          { return mParticleName[track_id]; }
        inline static thread_local const G4int& GetParticlePDG(G4int track_id)              { return mParticlePDG[track_id]; }
        inline static thread_local const G4int& GetParticleParentTrackID(G4int track_id)    { return mParticleParentTrackID[track_id]; }
        inline static thread_local const G4int& GetParticleAncestorTrackID(G4int track_id)  { return mParticleAncestorTrackID[track_id]; }

        // Get the entire map object
        inline static thread_local const std::map<G4int, G4int>&    GetParticleTrackIDMap()        { return mParticleTrackID; }               
        inline static thread_local const std::map<G4int, G4String>& GetParticleNameMap()           { return mParticleName; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticlePDGMap()            { return mParticlePDG; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticleParentTrackIDMap()  { return mParticleParentTrackID; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticleAncestorTrackIDMap(){ return mParticleAncestorTrackID; }

        // Neutron event data map
        inline static thread_local void AddNeutronEventDataMapTrackID(G4int track_id, G4int index)
        { mNeutronEventDataMap[track_id] = index; }
        inline static thread_local const G4int& GetNeutronEventDataIndex(G4int track_id) { return mNeutronEventDataMap[track_id]; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Event level primary info to keep track of
        void AddPrimaryInfoFromTrackBegin(const G4Track* track);
        void AddPrimaryInfoFromTrackEnd(const G4Track* track);
        inline static thread_local void AddPrimaryData(PrimaryData primary) { mPrimaryData.emplace_back(primary); }
        inline static thread_local G4int GetNumberOfPrimaries()             { return mPrimaryData.size(); }
        inline static thread_local const std::vector<PrimaryData>& GetPrimaries()  { return mPrimaryData; }
        
        inline static thread_local PrimaryData& GetPrimaryData(G4int track_id)
        {
            for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
            {
                if(mPrimaryData[ii].track_id == track_id) {
                    return mPrimaryData[ii];
                }
            }
            throw std::invalid_argument(
                "No PrimaryData object matches track_id: " + std::to_string(track_id)
            );
        }
        // Generate primaries
        std::vector<PrimaryGeneration> GeneratePrimaryList();
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Hit level info to keep track of
        void AddHitInfoFromStep(G4Step* step, G4TouchableHistory* history);
        inline static thread_local const std::vector<Hit>& GetHits() { return mHits; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Neutron level info to keep track of
        void AddNeutronInfoFromTrackBegin(const G4Track* track);
        void AddNeutronInfoFromTrackEnd(const G4Track* track);
        void AddNeutronInfoFromStep(const G4Step* step);
        void AddNeutronInfoFromRun();
        inline static thread_local const std::vector<NeutronEventData>& GetNeutronEventData() { return mNeutronEventData; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Analysis related functions
        void EvaluateEvent();
        void AddAnalysisFunction(std::function<void()> func) { mAnalysisFunctions.emplace_back(func); }
        //*************************************************************************************************//

        void SaveGDML();
        void FindSignalPath();

#ifdef ARTIE_PROFILING
        std::map<G4String, Profile> GetFunctionProfiles()     { return sFunctionProfiles; }
        inline void EndFunctionProfile(G4String func)   { 
            sFunctionProfiles[func].calls += 1; 
            sFunctionProfiles[func].time += (GetTimeInMilliseconds() - sProfilingTime.back()); 
            sProfilingTime.pop_back();
        }
        inline void StartFunctionProfile() {
            sProfilingTime.emplace_back(GetTimeInMilliseconds());
        }
        inline void ResetProfiling() {
            sProfilingTime.clear();
            sFunctionProfiles.clear();
        }
#else 
        inline void EndFunctionProfile(G4String func){}
        inline void StartFunctionProfile(){}
        inline void ResetProfiling(){}
#endif

    private:
        inline static G4int sNumberOfThreads =  {1};
        inline static G4int sNumberOfRuns =  {1};
        inline static G4int sNumberOfEvents =  {1};

        static std::shared_ptr<EventManager> sInstance;
        static std::mutex sMutex;

        inline static G4double sEventMaxTime = {2.e19 * ns};

        inline static G4String sOutputFileName = {"default"};
        inline static G4int sCurrentTupleIndex = {0};

        // Options to save various data to root files.
        inline static std::vector<Tuple> sTuples;
        inline static G4bool sSaveParticleMaps = {true};
        inline static G4bool sSavePrimaryInfo = {true};
        inline static G4bool sSaveHits = {true};
        inline static G4bool sSaveNeutronData = {true};
        inline static G4bool sSaveProfileData = {true};
        inline static G4bool sSaveNonDetectedNeutrons = {true};

        // Argon related parameters
        inline static G4bool mUseG4Definition = {false};
        inline static G4double mArgon36Ratio = {0.334};
        inline static G4double mArgon38Ratio = {0.063};
        inline static G4double mArgon40Ratio = {99.603};
        inline static G4double mLArDensity = {1.3954 * g/cm3};
        inline static G4double mLArTemperature = {85.0 * kelvin};
        inline static G4double mLArPressure = {0.952 * atmosphere};

        inline static G4double mRotatingBeamBlockerEntrance = {14.35 * m};
        inline static G4double mRotatingBeamBlockerLength = {30.5 * cm};
        inline static G4double mRotatingBeamBlockerHoleSize = {0.8 * cm};
        inline static G4double mRotatingBeamBlockerHoleSeparation = {3.0 * cm};

        // BC
        inline static G4double mBinocularCollimatorEntrance = {14.85 * m};
        inline static G4double mBinocularCollimatorLength = {30.0 * cm};
        inline static G4double mBinocularCollimatorHoleSize = {0.1 * cm};
        inline static G4double mBinocularCollimatorHoleSeparation = {3.0 * cm};

        // AS
        inline static G4double mAperatureStopEntrance = {18.5 * m};
        inline static G4double mAperatureStopLength = {15.0 * cm};
        inline static G4double mAperatureStopHoleSize = {1.2 * cm};
        inline static G4double mAperatureStopHoleSeparation = {3.0 * cm};

        inline static G4double mTZeroRadii = {0.0};
        inline static G4double mDetectorEntranceRadii = {0.0};
        inline static G4double mDetectorEntranceZ = {0.0};
        inline static G4double mSmallestRadii = {0.0};
        inline static G4double mSmallestRadiiZBegin = {0.0};
        inline static G4double mSmallestRadiiZEnd = {0.0};

        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        inline static thread_local std::map<G4int, G4int>      mParticleTrackID;
        inline static thread_local std::map<G4int, G4String>   mParticleName;
        inline static thread_local std::map<G4int, G4int>      mParticlePDG;
        inline static thread_local std::map<G4int, G4int>      mParticleParentTrackID;
        inline static thread_local std::map<G4int, G4int>      mParticleAncestorTrackID;

        inline static thread_local std::vector<PrimaryData>    mPrimaryData;
        inline static thread_local std::vector<Hit> mHits;
        inline static thread_local std::vector<NeutronEventData> mNeutronEventData;
        inline static thread_local std::vector<ProfileEventData> mProfileEventData;
        inline static thread_local std::map<G4int, G4int> mNeutronEventDataMap;
        inline static NeutronRunData sNeutronRunData;

        // Analysis functions
        std::vector<std::function<void()>> mAnalysisFunctions;

        // GDML parser
        G4GDMLParser* mGDMLParser;

#ifdef ARTIE_ROOT
        inline static G4bool mUseLANLDistribution = { false };
        inline static G4bool mUseLANLTOF =  { false };

        inline static G4bool mUsenTOFTOF =  { false };
        inline static G4bool mUsenTOFBeamProfile =  { false };
        
        
        // LANL energy distribution
        inline static G4String mLANLEnergyDistributionFileName = {"resolution13a.root"};
        inline static G4String mLANLEnergyDistributionName = {"tally5"};
        inline static TFile* mLANLEnergyDistributionFile = {0};
        inline static std::shared_ptr<TH1D> mLANLEnergyDistribution = {nullptr};
        inline static G4double mEnergyCutLow = { 40 * keV };
        inline static G4double mEnergyCutHigh = { 70 * keV };
        // LANL tof distribution
        inline static G4String mLANLTOFFileName = {"resolution13a.root"};
        inline static G4String mLANLTOFName = {"tally15"};
        inline static TFile* mLANLTOFFile = {0};
        inline static std::shared_ptr<TH2D> mLANLTOF = {nullptr};
        inline static std::vector<std::shared_ptr<TH1D>> mLANLTOFProjections = {};

        // nTOF tof distribution
        inline static G4String mnTOFTOFFileName = {"RF.root"};
        inline static G4String mnTOFTOFName = {"histfluka"};
        inline static TFile* mnTOFTOFFile = {0};
        inline static std::shared_ptr<TH2D> mnTOFTOF = {nullptr};
        inline static std::vector<std::shared_ptr<TH1D>> mnTOFTOFProjections = {};
        // nTOF beam profile
        inline static G4String mnTOFBeamProfileFileName = {"Profile_188m.root"};
        inline static G4String mnTOFBeamProfileName = {"histfluka"};
        inline static TFile* mnTOFBeamProfileFile = {0};
        inline static std::shared_ptr<TH2D> mnTOFBeamProfile = {nullptr};

        inline static G4double mTZeroLocation = {-1 * m};
        inline static G4double mDetectorEntrance = {30.0 * m};
#endif

#ifdef ARTIE_PROFILING
        inline static thread_local std::map<G4String, Profile> sFunctionProfiles = {};
        inline static thread_local std::vector<G4int> sProfilingTime = {};
#endif

#ifdef ARTIE_YAML
        inline static G4bool mSavedParameters = {false};
        inline static YAML::Node mConfig;
#endif     
    };
}