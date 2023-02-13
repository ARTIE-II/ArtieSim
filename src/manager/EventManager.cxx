/**
 * @file EventManager.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#include "EventManager.hh"

namespace Artie
{
    std::shared_ptr<EventManager> EventManager::sInstance;
    std::mutex EventManager::sMutex;

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

#ifdef ARTIE_YAML
    void EventManager::SetConfig(YAML::Node config)
    {
        mConfig = config;
        if(mConfig["manager"])
        {
            if(mConfig["manager"]["number_of_threads"]) { sNumberOfThreads = mConfig["manager"]["number_of_threads"].as<G4int>(); }
            if(mConfig["manager"]["number_of_runs"])    { sNumberOfRuns = mConfig["manager"]["number_of_runs"].as<G4int>(); }
            if(mConfig["manager"]["number_of_events"])  { sNumberOfEvents = mConfig["manager"]["number_of_events"].as<G4int>(); }
            if(mConfig["manager"]["output_filename"])   { sOutputFileName = mConfig["manager"]["output_filename"].as<std::string>(); }
            if(mConfig["manager"]["save_profile_data"]) { sSaveProfileData = mConfig["manager"]["save_profile_data"].as<G4bool>(); }
            if(mConfig["manager"]["save_non_detected_neutrons"]) { sSaveNonDetectedNeutrons = mConfig["manager"]["save_non_detected_neutrons"].as<G4bool>(); }
            if(mConfig["argon"]["use_g4_definition"])   { mUseG4Definition = mConfig["argon"]["use_g4_definition"].as<G4bool>(); }
            if(mConfig["argon"]["argon_36_ratio"])      { mArgon36Ratio = mConfig["argon"]["argon_36_ratio"].as<G4double>(); }
            if(mConfig["argon"]["argon_38_ratio"])      { mArgon38Ratio = mConfig["argon"]["argon_38_ratio"].as<G4double>(); }
            if(mConfig["argon"]["argon_40_ratio"])      { mArgon40Ratio = mConfig["argon"]["argon_40_ratio"].as<G4double>(); }
            if(mConfig["argon"]["lar_density"])         { mLArDensity = mConfig["argon"]["lar_density"].as<G4double>() * g/cm3; }
            if(mConfig["argon"]["lar_temperature"])     { mLArTemperature = mConfig["argon"]["lar_temperature"].as<G4double>() * kelvin; }
            if(mConfig["argon"]["lar_pressure"])        { mLArPressure = mConfig["argon"]["lar_pressure"].as<G4double>() * atmosphere; }
            if(mConfig["detector"]["detector_entrace"]) { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }
        }
        if(mConfig["dicer"]["rbb_entrance"])    { mRotatingBeamBlockerEntrance = mConfig["dicer"]["rbb_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["rbb_length"])      { mRotatingBeamBlockerLength = mConfig["dicer"]["rbb_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["rbb_hole_size"])   { mRotatingBeamBlockerHoleSize = mConfig["dicer"]["rbb_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["rbb_hole_separation"])   { mRotatingBeamBlockerHoleSeparation = mConfig["dicer"]["rbb_hole_separation"].as<G4double>() * cm; }

        if(mConfig["dicer"]["bc_entrance"])    { mBinocularCollimatorEntrance = mConfig["dicer"]["bc_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["bc_length"])      { mBinocularCollimatorLength = mConfig["dicer"]["bc_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_hole_size"])   { mBinocularCollimatorHoleSize = mConfig["dicer"]["bc_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_hole_separation"])   { mBinocularCollimatorHoleSeparation = mConfig["dicer"]["bc_hole_separation"].as<G4double>() * cm; }

        if(mConfig["dicer"]["as_entrance"])    { mAperatureStopEntrance = mConfig["dicer"]["as_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["as_length"])      { mAperatureStopLength = mConfig["dicer"]["as_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_hole_size"])   { mAperatureStopHoleSize = mConfig["dicer"]["as_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_hole_separation"])   { mAperatureStopHoleSeparation = mConfig["dicer"]["as_hole_separation"].as<G4double>() * cm; }        if(mConfig["generator"]["t_zero_location"]) { mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m; }

        if(mConfig["generator"]["energy_cut_low"])  { mEnergyCutLow = mConfig["generator"]["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["generator"]["energy_cut_high"]) { mEnergyCutHigh = mConfig["generator"]["energy_cut_high"].as<G4double>() * keV; }
    #ifdef ARTIE_ROOT
        if(mConfig["generator"]["use_lanl_distribution"]) { mUseLANLDistribution = mConfig["generator"]["use_lanl_distribution"].as<G4bool>(); }
        if(mConfig["generator"]["use_lanl_tof"]) { mUseLANLTOF = mConfig["generator"]["use_lanl_tof"].as<G4bool>(); }
        if(mConfig["generator"]["use_ntof_tof"]) { mUsenTOFTOF = mConfig["generator"]["use_ntof_tof"].as<G4bool>(); }
        if(mConfig["generator"]["use_ntof_beam_profile"]) { mUsenTOFBeamProfile = mConfig["generator"]["use_ntof_beam_profile"].as<G4bool>(); }

        if(mConfig["generator"]["lanl_distribution_filename"])  { mLANLEnergyDistributionFileName = mConfig["generator"]["lanl_distribution_filename"].as<std::string>(); }
        if(mConfig["generator"]["lanl_distribution_name"])      { mLANLEnergyDistributionName = mConfig["generator"]["lanl_distribution_name"].as<std::string>(); }
        if(mConfig["generator"]["lanl_tof_filename"])  { mLANLTOFFileName = mConfig["generator"]["lanl_tof_filename"].as<std::string>(); }
        if(mConfig["generator"]["lanl_tof_name"])      { mLANLTOFName = mConfig["generator"]["lanl_tof_name"].as<std::string>(); }

        if(mConfig["generator"]["ntof_tof_filename"])  { mnTOFTOFFileName = mConfig["generator"]["ntof_tof_filename"].as<std::string>(); }
        if(mConfig["generator"]["ntof_tof_name"])      { mnTOFTOFName = mConfig["generator"]["ntof_tof_name"].as<std::string>(); }

        if(mConfig["generator"]["ntof_beam_profile_filename"])  { mnTOFBeamProfileFileName = mConfig["generator"]["ntof_beam_profile_filename"].as<std::string>(); }
        if(mConfig["generator"]["ntof_beam_profile_name"])      { mnTOFBeamProfileName = mConfig["generator"]["ntof_beam_profile_name"].as<std::string>(); }

        if(mUseLANLDistribution) 
        {
            mLANLEnergyDistributionFile = new TFile(mLANLEnergyDistributionFileName);
            TGraph *DistributionGraph = (TGraph*)mLANLEnergyDistributionFile->Get(mLANLEnergyDistributionName);

            // Make variable-bin histogram for beam energy
            const G4int nlogbins=500;        
            G4double xmin = 1.e-3;  //eV
            G4double xmax = 1.e7;   //eV
            G4double *xbins = new G4double[nlogbins+1];
            G4double xlogmin = TMath::Log10(xmin);
            G4double xlogmax = TMath::Log10(xmax);
            G4double dlogx   = (xlogmax-xlogmin)/((G4double)nlogbins);
            for (G4int i=0;i<=nlogbins;i++) 
            {
                G4double xlog = xlogmin+ i*dlogx;
                xbins[i] = TMath::Exp( TMath::Log(10) * xlog ); 
            }
            mLANLEnergyDistribution.reset(
                new TH1D("LANLBeamEnergy", "LANLBeamEnergy", nlogbins, xbins)
            );
            auto nPoints = DistributionGraph->GetN(); // number of points 
            G4double x, y;
            for(G4int i=0; i < nPoints; ++i) {
                DistributionGraph->GetPoint(i, x, y); //eV
                if( 
                    x / 1000 > mEnergyCutLow && 
                    x / 1000 < mEnergyCutHigh
                ) 
                {
                    mLANLEnergyDistribution->Fill(x,y);
                }
            }
        }
        if(mUseLANLTOF) 
        {
            mLANLTOFFile = new TFile(mLANLTOFFileName);
            // Set up tof profile and projections
            mLANLTOF.reset((TH2D*)mLANLTOFFile->Get(mLANLTOFName));
            auto beam_x = mLANLTOF->GetXaxis();
            auto num_bins = beam_x->GetNbins();
            for(G4int ii = 0; ii < num_bins; ii++)
            {
                std::string projection_name = "profile_" + std::to_string(ii);
                TH1D* projection = (TH1D*)mLANLTOF->ProjectionY(
                    projection_name.c_str(),
                    ii, ii+1
                );
                mLANLTOFProjections.emplace_back(projection);
            }
        }
        if(mUsenTOFTOF) 
        {
            mnTOFTOFFile = new TFile(mnTOFTOFFileName);
            // Set up tof profile and projections
            mnTOFTOF.reset((TH2D*)mnTOFTOFFile->Get(mnTOFTOFName));
            
            auto beam_x = mnTOFTOF->GetXaxis();
            auto num_bins = beam_x->GetNbins();
            for(G4int ii = 0; ii < num_bins; ii++)
            {
                std::string projection_name = "profile_" + std::to_string(ii);
                TH1D* projection = (TH1D*)mnTOFTOF->ProjectionY(
                    projection_name.c_str(),
                    ii, ii+1
                );
                mnTOFTOFProjections.emplace_back(projection);
            }
        }
        if(mUsenTOFBeamProfile)
        {
            mnTOFBeamProfileFile = new TFile(mnTOFBeamProfileFileName);
            // Set up tof profile and projections
            mnTOFBeamProfile.reset((TH2D*)mnTOFBeamProfileFile->Get(mnTOFBeamProfileName));
        }
    #endif
        // Setting up the GDML parser
        G4GDMLParser* mGDMLParser;
        // mGDMLParser->SetStripFlag(false);
        // mGDMLParser->SetOverlapCheck(true);
#endif
    }

    void EventManager::SaveGDML()
    {
        G4GDMLParser* mGDMLParser;
        mGDMLParser->SetRegionExport(true);
        mGDMLParser->SetOutputFileOverwrite(true);
        mGDMLParser->Write(
            "gdml/" + OutputFileName() + ".gdml", 
            G4TransportationManager::GetTransportationManager()
            ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume()
        );
    }

    void EventManager::FindSignalPath()
    {
        std::vector<G4double> radii = {
            mRotatingBeamBlockerHoleSize,
            mBinocularCollimatorHoleSize,
            mAperatureStopHoleSize
        };
        std::vector<G4double> z = {
            mTZeroLocation + mRotatingBeamBlockerEntrance,
            mTZeroLocation + mBinocularCollimatorEntrance,
            mTZeroLocation + mAperatureStopEntrance,
        };
        std::vector<G4double> l = {
            mRotatingBeamBlockerLength,
            mBinocularCollimatorLength,
            mAperatureStopLength
        };
        mSmallestRadii = 10e8;
        G4int smallest_index = 0;
        for(G4int ii = 0; ii < radii.size(); ii++)
        {
            if(radii[ii] < mSmallestRadii) {
                mSmallestRadii = radii[ii];
                mSmallestRadiiZBegin = z[ii];
                mSmallestRadiiZEnd = z[ii] + l[ii];
                smallest_index = ii;
            }
        }
        G4double smallest_tan_theta = 1.0;
        for(G4int ii = 0; ii < radii.size(); ii++)
        {
            if(ii != mSmallestRadii) {
                G4double tan_theta = radii[ii] / abs(z[ii] - (z[smallest_index] + l[smallest_index]));
                if(tan_theta < smallest_tan_theta) {
                    smallest_tan_theta = tan_theta;
                }
            }
        }
        mTZeroRadii = abs(smallest_tan_theta * mTZeroLocation);
        mDetectorEntranceRadii = abs(smallest_tan_theta * mDetectorEntrance);
    }

    G4int EventManager::GetIndex(G4String tuple)
    {
        for(size_t ii = 0; ii < sTuples.size(); ii++)
        {
            if(sTuples[ii].name == tuple) {
                return ii;
            }
        }
        sCurrentTupleIndex += 1;
        sTuples.emplace_back(Tuple(tuple, sCurrentTupleIndex));
        return sCurrentTupleIndex;
    }

    void EventManager::OpenOutputFile(G4int RunID)
    {
        std::filesystem::create_directory("outputs");
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4String OutputFile = "outputs/" + OutputFileName() + "_" + std::to_string(RunID) + ".root";
        G4bool fileopen = AnalysisManager->OpenFile(OutputFile);
        if (!fileopen) {
            G4cout << "File - " + OutputFile 
                << " - not opened!" << G4endl;
        }
        else {
            G4cout << "File - " + OutputFile
                << " - opened successfully." << G4endl;
        }
    }

    void EventManager::ConstructEnergyDistribution()
    {
#ifdef ARTIE_ROOT
        mLANLEnergyDistributionFile = new TFile(mLANLEnergyDistributionFileName);
        TGraph *DistributionGraph = (TGraph*)mLANLEnergyDistributionFile->Get(mLANLEnergyDistributionName);

        // Make variable-bin histogram for beam energy
        const G4int nlogbins=500;        
        G4double xmin = 1.e-3;  //eV
        G4double xmax = 1.e7;   //eV
        G4double *xbins = new G4double[nlogbins+1];
        G4double xlogmin = TMath::Log10(xmin);
        G4double xlogmax = TMath::Log10(xmax);
        G4double dlogx   = (xlogmax-xlogmin)/((G4double)nlogbins);
        for (G4int i=0;i<=nlogbins;i++) 
        {
            G4double xlog = xlogmin+ i*dlogx;
            xbins[i] = TMath::Exp( TMath::Log(10) * xlog ); 
        }

        mLANLEnergyDistribution.reset(
            new TH1D("LANLBeamEnergy", "LANLBeamEnergy", nlogbins, xbins)
        );
        auto nPoints = DistributionGraph->GetN(); // number of points 
        G4double x, y;
        for(G4int i=0; i < nPoints; ++i) {
            DistributionGraph->GetPoint(i, x, y); //eV
            if( 
                x / 1000 > mEnergyCutLow && 
                x / 1000 < mEnergyCutHigh
            ) 
            {
                mLANLEnergyDistribution->Fill(x,y);
            }
        }
#endif
    }

    void EventManager::CloseOutputFile(G4int RunID)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
#ifdef ARTIE_YAML
        if(mSavedParameters == false)
        {
            G4int index = GetIndex("Configuration");
            for(YAML::const_iterator it = mConfig.begin(); it != mConfig.end(); it++)
            {
                for(YAML::const_iterator iit = it->second.begin(); iit != it->second.end(); iit++)
                {
                    if (iit->first.as<std::string>() == "filter_materials" ||
                        iit->first.as<std::string>() == "filter_z_position" ||
                        iit->first.as<std::string>() == "filter_thickness" ||
                        iit->first.as<std::string>() == "filter_radius"
                    )
                    {
                        for(size_t jj = 0; jj < iit->second.size(); jj++)
                        {
                            AnalysisManager->FillNtupleSColumn(index, 0, it->first.as<std::string>());
                            AnalysisManager->FillNtupleSColumn(index, 1, iit->first.as<std::string>());
                            AnalysisManager->FillNtupleSColumn(index, 2, iit->second[jj].as<std::string>());
                            AnalysisManager->AddNtupleRow(index);
                        }
                    }
                    else
                    {
                        AnalysisManager->FillNtupleSColumn(index, 0, it->first.as<std::string>());
                        AnalysisManager->FillNtupleSColumn(index, 1, iit->first.as<std::string>());
                        AnalysisManager->FillNtupleSColumn(index, 2, iit->second.as<std::string>());
                        AnalysisManager->AddNtupleRow(index);
                    }
                }
            }
            mSavedParameters = true;
        }
#endif
        AnalysisManager->Write();
        AnalysisManager->CloseFile();

#ifdef LARGEANT_PROFILING
        std::ofstream ProfilingFile;
        std::filesystem::create_directory(".logs");
        auto ThreadID = G4Threading::G4GetThreadId();
        if(ThreadID < 0) { 
            return;
        }
        ProfilingFile.open(
            ".logs/" + OutputFileName() + "_" + std::to_string(RunID)
            + "_t" + std::to_string(ThreadID) + ".profile"  
        );
        ProfilingFile << "function,number_of_calls,total_time[ms]\n"; 
        
        auto Profiles = GetFunctionProfiles();
        for(auto const& [key, val] : Profiles)
        {
            ProfilingFile << key << ","; 
            ProfilingFile << val.calls << ","; 
            ProfilingFile << val.time << "\n";
        }
        ProfilingFile.close();
#endif
    }

    void EventManager::CreateTuples()
    {
        StartFunctionProfile();
        auto AnalysisManager = G4AnalysisManager::Instance();
#ifdef ARTIE_GEANT_10
#else
        AnalysisManager->SetDefaultFileType("root");
#endif
        AnalysisManager->SetVerboseLevel(0);
        AnalysisManager->SetNtupleMerging(true);

#ifdef ARTIE_YAML
        G4int index = GetIndex("Configuration");
        AnalysisManager->CreateNtuple("Configuration", "Configuration");
        AnalysisManager->CreateNtupleSColumn("category");
        AnalysisManager->CreateNtupleSColumn("parameter");
        AnalysisManager->CreateNtupleSColumn("value");
        AnalysisManager->FinishNtuple(index);
        mSavedParameters = false;
#endif
        if(SaveParticleMaps())
        {
            G4int index = GetIndex("ParticleMaps");
            AnalysisManager->CreateNtuple("ParticleMaps", "ParticleMaps");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleIColumn("parent_track_id");
            AnalysisManager->CreateNtupleIColumn("ancestor_track_id");
            AnalysisManager->FinishNtuple(index);
        }
        if(SavePrimaryInfo())
        {
            G4int index = GetIndex("Primaries");
            AnalysisManager->CreateNtuple("Primaries", "Primaries");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleDColumn("init_t");
            AnalysisManager->CreateNtupleDColumn("init_x");
            AnalysisManager->CreateNtupleDColumn("init_y");
            AnalysisManager->CreateNtupleDColumn("init_z");
            AnalysisManager->CreateNtupleDColumn("init_energy");
            AnalysisManager->CreateNtupleDColumn("final_t");
            AnalysisManager->CreateNtupleDColumn("final_x");
            AnalysisManager->CreateNtupleDColumn("final_y");
            AnalysisManager->CreateNtupleDColumn("final_z");
            AnalysisManager->CreateNtupleDColumn("final_energy");
            AnalysisManager->CreateNtupleIColumn("number_of_daughters");
            AnalysisManager->CreateNtupleDColumn("total_daughter_init_energy");
            AnalysisManager->CreateNtupleDColumn("total_daughter_final_energy");
            AnalysisManager->CreateNtupleDColumn("total_daughter_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_edeps");
            AnalysisManager->CreateNtupleDColumn("total_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_hits");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveHits())
        {
            G4int index = GetIndex("Hits");
            AnalysisManager->CreateNtuple("Hits", "Hits");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("copy_number");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleIColumn("parent_id");
            AnalysisManager->CreateNtupleDColumn("local_time");
            AnalysisManager->CreateNtupleDColumn("global_time");
            AnalysisManager->CreateNtupleDColumn("x_particle");
            AnalysisManager->CreateNtupleDColumn("y_particle");
            AnalysisManager->CreateNtupleDColumn("z_particle");
            AnalysisManager->CreateNtupleDColumn("px_particle");
            AnalysisManager->CreateNtupleDColumn("py_particle");
            AnalysisManager->CreateNtupleDColumn("pz_particle");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleIColumn("detected");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveNeutronData())
        {
            G4int index = GetIndex("NeutronRunData");
            AnalysisManager->CreateNtuple("NeutronRunData", "NeutronRunData");
            AnalysisManager->CreateNtupleIColumn("num_events");
            AnalysisManager->CreateNtupleIColumn("num_detected");
            AnalysisManager->CreateNtupleIColumn("num_elastic");
            AnalysisManager->CreateNtupleIColumn("num_inelastic");
            AnalysisManager->CreateNtupleIColumn("num_capture");
            AnalysisManager->CreateNtupleIColumn("num_fission");
            AnalysisManager->CreateNtupleIColumn("num_scatter");
            AnalysisManager->CreateNtupleIColumn("num_scatter_out");
            AnalysisManager->CreateNtupleIColumn("num_scatter_detector");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveNeutronData())
        {
            G4int index = GetIndex("NeutronEventData");
            AnalysisManager->CreateNtuple("NeutronEventData", "NeutronEventData");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleDColumn("neutron_energy");
            AnalysisManager->CreateNtupleDColumn("nominal_tof");
            AnalysisManager->CreateNtupleDColumn("start_time");
            AnalysisManager->CreateNtupleDColumn("start_x");
            AnalysisManager->CreateNtupleDColumn("start_y");
            AnalysisManager->CreateNtupleDColumn("start_z");
            AnalysisManager->CreateNtupleIColumn("detector");
            AnalysisManager->CreateNtupleDColumn("arrival_time");
            AnalysisManager->CreateNtupleDColumn("arrival_energy");
            AnalysisManager->CreateNtupleIColumn("num_elastic");
            AnalysisManager->CreateNtupleIColumn("num_inelastic");
            AnalysisManager->CreateNtupleIColumn("num_capture");
            AnalysisManager->CreateNtupleIColumn("num_fission");
            AnalysisManager->CreateNtupleIColumn("num_scatter");
            AnalysisManager->CreateNtupleIColumn("num_scatter_out");
            AnalysisManager->CreateNtupleIColumn("gas_first");
            AnalysisManager->CreateNtupleDColumn("first_scatter_z");
            AnalysisManager->CreateNtupleDColumn("first_scatter_t");
            AnalysisManager->CreateNtupleDColumn("max_dphi");
            AnalysisManager->CreateNtupleDColumn("max_dp");
            AnalysisManager->CreateNtupleDColumn("max_dE");
            AnalysisManager->CreateNtupleIColumn("safe_passage");
            AnalysisManager->CreateNtupleDColumn("first_target_step_time");
            AnalysisManager->CreateNtupleDColumn("first_target_step_energy");
            AnalysisManager->CreateNtupleDColumn("first_target_step_z");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveProfileData())
        {
            G4int index = GetIndex("ProfileEventData");
            AnalysisManager->CreateNtuple("ProfileEventData", "ProfileEventData");
            AnalysisManager->CreateNtupleSColumn("name");
            AnalysisManager->CreateNtupleSColumn("volume");
            AnalysisManager->CreateNtupleIColumn("exit");
            AnalysisManager->CreateNtupleDColumn("x");
            AnalysisManager->CreateNtupleDColumn("y");
            AnalysisManager->FinishNtuple(index);
        }
        EndFunctionProfile("CreateTuples");
    }

    void EventManager::FillParticleMaps(G4int EventID)
    {
        if(!SaveParticleMaps()) {
            return;
        }
        StartFunctionProfile();

        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("ParticleMaps");
        for(auto const& [key, val] : mParticleName)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, key);
            AnalysisManager->FillNtupleSColumn(index, 2, GetParticleName(key));
            AnalysisManager->FillNtupleIColumn(index, 3, GetParticlePDG(key));
            AnalysisManager->FillNtupleIColumn(index, 4, GetParticleParentTrackID(key));
            AnalysisManager->FillNtupleIColumn(index, 5, GetParticleAncestorTrackID(key));
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillParticleMaps");
    }
    void EventManager::FillPrimaryInfo(G4int EventID)
    {
        if(!SavePrimaryInfo()) {
            return;
        }
        StartFunctionProfile();

        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("Primaries");
        for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mPrimaryData[ii].track_id);
            AnalysisManager->FillNtupleSColumn(index, 2, mPrimaryData[ii].name);
            AnalysisManager->FillNtupleIColumn(index, 3, mPrimaryData[ii].pdg);
            AnalysisManager->FillNtupleDColumn(index, 4, mPrimaryData[ii].init_t);
            AnalysisManager->FillNtupleDColumn(index, 5, mPrimaryData[ii].init_x);
            AnalysisManager->FillNtupleDColumn(index, 6, mPrimaryData[ii].init_y);
            AnalysisManager->FillNtupleDColumn(index, 7, mPrimaryData[ii].init_z);
            AnalysisManager->FillNtupleDColumn(index, 8, mPrimaryData[ii].init_energy);
            AnalysisManager->FillNtupleDColumn(index, 9, mPrimaryData[ii].final_t);
            AnalysisManager->FillNtupleDColumn(index, 10, mPrimaryData[ii].final_x);
            AnalysisManager->FillNtupleDColumn(index, 11, mPrimaryData[ii].final_y);
            AnalysisManager->FillNtupleDColumn(index, 12, mPrimaryData[ii].final_z);
            AnalysisManager->FillNtupleDColumn(index, 13, mPrimaryData[ii].final_energy);
            AnalysisManager->FillNtupleIColumn(index, 14, mPrimaryData[ii].number_of_daughters);
            AnalysisManager->FillNtupleDColumn(index, 15, mPrimaryData[ii].total_daughter_init_energy);
            AnalysisManager->FillNtupleDColumn(index, 16, mPrimaryData[ii].total_daughter_final_energy);
            AnalysisManager->FillNtupleDColumn(index, 17, mPrimaryData[ii].total_daughter_edep);
            AnalysisManager->FillNtupleIColumn(index, 18, mPrimaryData[ii].number_of_edeps);
            AnalysisManager->FillNtupleDColumn(index, 19, mPrimaryData[ii].total_edep);
            AnalysisManager->FillNtupleIColumn(index, 20, mPrimaryData[ii].number_of_hits);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillPrimaryInfo");
    }

    void EventManager::FillHits(G4int EventID)
    {
        if (!SaveHits()) {
            return;
        }
        StartFunctionProfile();

        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("Hits");
        for(size_t ii = 0; ii < mHits.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mHits[ii].copy_number);
            AnalysisManager->FillNtupleIColumn(index, 2, mHits[ii].track_id);
            AnalysisManager->FillNtupleIColumn(index, 3, mHits[ii].parent_track_id);
            AnalysisManager->FillNtupleDColumn(index, 4, mHits[ii].local_time);
            AnalysisManager->FillNtupleDColumn(index, 5, mHits[ii].global_time);
            AnalysisManager->FillNtupleDColumn(index, 6, mHits[ii].particle_position[0]);
            AnalysisManager->FillNtupleDColumn(index, 7, mHits[ii].particle_position[1]);
            AnalysisManager->FillNtupleDColumn(index, 8, mHits[ii].particle_position[2]);
            AnalysisManager->FillNtupleDColumn(index, 9, mHits[ii].particle_momentum[0]);
            AnalysisManager->FillNtupleDColumn(index, 10,mHits[ii].particle_momentum[1]);
            AnalysisManager->FillNtupleDColumn(index, 11,mHits[ii].particle_momentum[2]);
            AnalysisManager->FillNtupleDColumn(index, 12,mHits[ii].energy);
            AnalysisManager->FillNtupleIColumn(index, 13,mHits[ii].detected);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillHits");
    }

    void EventManager::FillNeutronEventData(G4int EventID)
    {
        if (!SaveNeutronData()) {
            return;
        }
        StartFunctionProfile();

        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("NeutronEventData");
        for(size_t ii = 0; ii < mNeutronEventData.size(); ii++)
        {
            if(!sSaveNonDetectedNeutrons && mNeutronEventData[ii].detector == -1) {
                continue;
            }
            AnalysisManager->FillNtupleIColumn(index, 0, mNeutronEventData[ii].event);
            AnalysisManager->FillNtupleIColumn(index, 1, mNeutronEventData[ii].track_id);
            AnalysisManager->FillNtupleDColumn(index, 2, mNeutronEventData[ii].neutron_energy);
            AnalysisManager->FillNtupleDColumn(index, 3, mNeutronEventData[ii].nominal_tof);
            AnalysisManager->FillNtupleDColumn(index, 4, mNeutronEventData[ii].start_time);
            AnalysisManager->FillNtupleDColumn(index, 5, mNeutronEventData[ii].start_x);
            AnalysisManager->FillNtupleDColumn(index, 6, mNeutronEventData[ii].start_y);
            AnalysisManager->FillNtupleDColumn(index, 7, mNeutronEventData[ii].start_z);
            AnalysisManager->FillNtupleIColumn(index, 8, mNeutronEventData[ii].detector);
            AnalysisManager->FillNtupleDColumn(index, 9, mNeutronEventData[ii].arrival_time);
            AnalysisManager->FillNtupleDColumn(index, 10, mNeutronEventData[ii].arrival_energy);
            AnalysisManager->FillNtupleIColumn(index, 11, mNeutronEventData[ii].num_elastic);
            AnalysisManager->FillNtupleIColumn(index, 12, mNeutronEventData[ii].num_inelastic);
            AnalysisManager->FillNtupleIColumn(index, 13, mNeutronEventData[ii].num_capture);
            AnalysisManager->FillNtupleIColumn(index, 14, mNeutronEventData[ii].num_fission);
            AnalysisManager->FillNtupleIColumn(index, 15, mNeutronEventData[ii].num_scatter);
            AnalysisManager->FillNtupleIColumn(index, 16, mNeutronEventData[ii].num_scatter_out);
            AnalysisManager->FillNtupleIColumn(index, 17, mNeutronEventData[ii].gas_first);
            AnalysisManager->FillNtupleDColumn(index, 18, mNeutronEventData[ii].first_scatter_z);
            AnalysisManager->FillNtupleDColumn(index, 19, mNeutronEventData[ii].first_scatter_t);
            AnalysisManager->FillNtupleDColumn(index, 20, mNeutronEventData[ii].max_dphi);
            AnalysisManager->FillNtupleDColumn(index, 21, mNeutronEventData[ii].max_dp);
            AnalysisManager->FillNtupleDColumn(index, 22, mNeutronEventData[ii].max_dE);
            AnalysisManager->FillNtupleIColumn(index, 23, mNeutronEventData[ii].safe_passage);
            AnalysisManager->FillNtupleDColumn(index, 24, mNeutronEventData[ii].first_target_step_time);
            AnalysisManager->FillNtupleDColumn(index, 25, mNeutronEventData[ii].first_target_step_energy);
            AnalysisManager->FillNtupleDColumn(index, 26, mNeutronEventData[ii].first_target_step_z);
            AnalysisManager->AddNtupleRow(index);
        }

        EndFunctionProfile("FillNeutronEventData");
    }

    void EventManager::FillNeutronRunData()
    {
        if (!SaveNeutronData()) {
            return;
        }
        StartFunctionProfile();

        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("NeutronRunData");
        AnalysisManager->FillNtupleIColumn(index, 0, sNeutronRunData.num_events);
        AnalysisManager->FillNtupleIColumn(index, 1, sNeutronRunData.num_detected);
        AnalysisManager->FillNtupleIColumn(index, 2, sNeutronRunData.num_elastic);
        AnalysisManager->FillNtupleIColumn(index, 3, sNeutronRunData.num_inelastic);
        AnalysisManager->FillNtupleIColumn(index, 4, sNeutronRunData.num_capture);
        AnalysisManager->FillNtupleIColumn(index, 5, sNeutronRunData.num_fission);
        AnalysisManager->FillNtupleIColumn(index, 6, sNeutronRunData.num_scatter);
        AnalysisManager->FillNtupleIColumn(index, 7, sNeutronRunData.num_scatter_out);
        AnalysisManager->FillNtupleIColumn(index, 8, sNeutronRunData.num_scatter_detector);
        AnalysisManager->AddNtupleRow(index);

        EndFunctionProfile("FillNeutronRunData");
    }

    void EventManager::AddParticleMapsFromTrack(const G4Track* track)
    {
        StartFunctionProfile();
        G4int track_id = track->GetTrackID();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4int parent_track_id = track->GetParentID();
        
        const G4VProcess* creator_process = track->GetCreatorProcess();
        G4String process = "none";
        if(creator_process) {
            process = creator_process->GetProcessName();
        }

        // Save Particle Info    
        AddParticleName(track_id, particle_name);
        AddParticlePDG(track_id, particle_pdg);
        AddParticleParentTrackID(track_id, parent_track_id);

        // Add ancestor info
        if (parent_track_id == 0) {
            AddParticleAncestorTrackID(track_id, 0);
        }
        else if (GetParticleParentTrackID(parent_track_id) == 0) {
            AddParticleAncestorTrackID(track_id, parent_track_id);
        }
        else {
            AddParticleAncestorTrackID(track_id, GetParticleAncestorTrackID(parent_track_id));
        }

        EndFunctionProfile("AddParticleMapsFromTrack");
    }

    void EventManager::AddPrimaryInfoFromTrackBegin(const G4Track* track)
    {
        StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector particle_position = track->GetVertexPosition();

        G4double global_time = track->GetGlobalTime();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_id = track->GetTrackID();
        G4int parent_track_id = track->GetParentID();

        const G4VProcess* creator_process = track->GetCreatorProcess();
        G4String process = "none";
        if(creator_process) {
            process = creator_process->GetProcessName();
        }

        if(parent_track_id == 0)
        {
            AddPrimaryData(
                PrimaryData(
                    particle_name, track_id, particle_pdg,
                    global_time, particle_position[0], particle_position[1],
                    particle_position[2], kinetic_energy 
                )
            );
        }
        else
        {
            GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_daughter_init_energy += kinetic_energy;
            GetPrimaryData(GetParticleAncestorTrackID(track_id)).number_of_daughters += 1;
        }
        EndFunctionProfile("AddPrimaryInfoFromTrackBegin");
    }

    void EventManager::AddPrimaryInfoFromTrackEnd(const G4Track* track)
    {
        StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4ThreeVector particle_position = track->GetVertexPosition();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_id = track->GetTrackID();
        G4double global_time = track->GetGlobalTime();
        
        if(GetParticleParentTrackID(track_id) == 0)
        {
            GetPrimaryData(track_id).AddFinalTrackData(
                global_time, particle_position[0], particle_position[1],
                particle_position[2], kinetic_energy
            );
        }
        else {
            GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_daughter_final_energy += kinetic_energy;
        }
        EndFunctionProfile("AddPrimaryInfoFromTrackEnd");
    }

    void EventManager::AddHitInfoFromStep(G4Step* step, G4TouchableHistory* history)
    {
        StartFunctionProfile();
        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        const G4Track* track = step->GetTrack();
        const G4StepPoint *preStepPoint = step->GetPreStepPoint();

        G4int           copyNo = touchable->GetCopyNumber();
        G4double        globalTime = preStepPoint->GetGlobalTime();
        G4int           trackID = track->GetTrackID();
        G4int           parentID = track->GetParentID();
        G4double        localTime = preStepPoint->GetGlobalTime();
        G4ThreeVector   particlePosition = preStepPoint->GetPosition();
        G4double        energy = preStepPoint->GetTotalEnergy();
        G4ThreeVector   particleMomentum = preStepPoint->GetMomentum();

        mHits.emplace_back(
            Hit(
                copyNo, trackID,
                parentID, localTime, globalTime,
                particlePosition, particleMomentum,
                energy
            )
        );
        EndFunctionProfile("AddHitInfoFromStep");
    }

    void EventManager::AddNeutronInfoFromTrackBegin(const G4Track* track)
    {
        if(track->GetParticleDefinition()->GetParticleName() != "neutron") {
            return;
        }
        if(track->GetParentID() == 0)
        {
            mNeutronEventData.emplace_back(
                NeutronEventData(
                    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID(),
                    track->GetTrackID(),
                    track->GetKineticEnergy(),
                    GetNominalTOF(track->GetKineticEnergy()),
                    track->GetGlobalTime(),
                    track->GetPosition()
                )
            );
            AddNeutronEventDataMapTrackID(track->GetTrackID(), mNeutronEventData.size() - 1);
        }

    }
    void EventManager::AddNeutronInfoFromTrackEnd(const G4Track* track)
    {
        if(track->GetParticleDefinition()->GetParticleName() != "neutron") {
            return;
        }
    }

    void EventManager::AddNeutronInfoFromStep(const G4Step* step)
    {
        if(
            step->GetTrack()->GetParticleDefinition()->GetParticleName() != "neutron" ||
            step->GetTrack()->GetParentID() != 0
        ) {
            return;
        }

        StartFunctionProfile();
        const G4Track* track = step->GetTrack();
        const G4StepPoint *preStepPoint = step->GetPreStepPoint();
        const G4StepPoint *postStepPoint = step->GetPostStepPoint();

        G4String volumeName = GetVolumeName(step);
        G4String postProcessName = GetPostProcessName(step);

        G4int           trackID = track->GetTrackID();
        G4ThreeVector   position = track->GetPosition();

        G4int neutron_index = GetNeutronEventDataIndex(trackID);

        if(mNeutronEventData[neutron_index].track_id != trackID) {
            G4cout << "ERROR! Bug in neutron index." << G4endl;
            exit(0);
        }
        
        // check if neutron has arrived at the detector
        if(mNeutronEventData[neutron_index].arrival_time == 0)
        {   
            // calculate delta phi, delta energy, and delta momentum.
            G4ThreeVector preMomentumDirection = preStepPoint->GetMomentumDirection();
            G4ThreeVector postMomentumDirection = postStepPoint->GetMomentumDirection();
            double dphi = preMomentumDirection.angle(postMomentumDirection);
            double dp   = (preMomentumDirection - postMomentumDirection).mag();
            double dE   = fabs(
                postStepPoint->GetKineticEnergy() - preStepPoint->GetKineticEnergy()
            );

            if(step->IsFirstStepInVolume() && volumeName == "Logical_ArtieIIActiveVolume")
            {
                mNeutronEventData[neutron_index].first_target_step_time = track->GetGlobalTime();
                mNeutronEventData[neutron_index].first_target_step_energy = track->GetKineticEnergy();
                mNeutronEventData[neutron_index].first_target_step_z = position.z();
            }
            // Keep track of how often each process occurs.
            if(postProcessName == "hadElastic") {
                mNeutronEventData[neutron_index].num_elastic += 1;
            }
            else if(postProcessName == "neutronInelastic") {
                mNeutronEventData[neutron_index].num_inelastic += 1;
            }
            else if(postProcessName == "nCapture") {
                mNeutronEventData[neutron_index].num_capture += 1;
            }
            else if(postProcessName == "nFission") {
                mNeutronEventData[neutron_index].num_fission += 1;
            }
            if(
                step->IsLastStepInVolume() && 
                volumeName == "Logical_ArtieIIActiveVolume" &&
                mNeutronEventData[neutron_index].num_elastic == 0 &&
                mNeutronEventData[neutron_index].num_inelastic == 0 &&
                mNeutronEventData[neutron_index].num_capture == 0 &&
                mNeutronEventData[neutron_index].num_fission == 0
            ) 
            {
                mNeutronEventData[neutron_index].safe_passage = 1;    
            }

            // If we have just reached the detector, 
            // record the time and energy
            if(step->IsFirstStepInVolume() && volumeName == "Logical_ArtieIITargetDetector")
            {
                mNeutronEventData[neutron_index].arrival_time = track->GetGlobalTime();
                mNeutronEventData[neutron_index].arrival_energy = postStepPoint->GetKineticEnergy();
            }
            else if(step->IsFirstStepInVolume() && volumeName == "Logical_ArtieIITargetDetectorLeft")
            {
                mNeutronEventData[neutron_index].detector = 0;
                mNeutronEventData[neutron_index].arrival_time = track->GetGlobalTime();
                mNeutronEventData[neutron_index].arrival_energy = postStepPoint->GetKineticEnergy();
            }
            else if(step->IsFirstStepInVolume() && volumeName == "Logical_ArtieIITargetDetectorRight")
            {
                mNeutronEventData[neutron_index].detector = 1;
                mNeutronEventData[neutron_index].arrival_time = track->GetGlobalTime();
                mNeutronEventData[neutron_index].arrival_energy = postStepPoint->GetKineticEnergy();
            }

            // Quantify scattering
            if (dp > 0)
            {
                if (volumeName == "Logical_ArtieIIActiveVolume")
                {
                    mNeutronEventData[neutron_index].num_scatter += 1;
                    if (
                        mNeutronEventData[neutron_index].num_scatter == 1 &&
                        mNeutronEventData[neutron_index].num_scatter_out == 0
                    )
                    {
                        mNeutronEventData[neutron_index].first_scatter_z = position.z();
                        mNeutronEventData[neutron_index].first_scatter_t = track->GetGlobalTime();
                        mNeutronEventData[neutron_index].gas_first = 1;
                    }
                } 
                else 
                {
                    mNeutronEventData[neutron_index].num_scatter_out += 1;    
                    if (
                        mNeutronEventData[neutron_index].num_scatter == 0 &&
                        mNeutronEventData[neutron_index].num_scatter_out == 1
                    ) 
                    {
                        mNeutronEventData[neutron_index].first_scatter_z = position.z();
                        mNeutronEventData[neutron_index].first_scatter_t = track->GetGlobalTime();
                    }
                }    
            }
            if (volumeName != "Logical_ArtieIITargetDetector" || 
                volumeName != "Logical_ArtieIITargetDetectorLeft" || 
                volumeName != "Logical_ArtieIITargetDetectorRight"
            ){  
                if (dp > mNeutronEventData[neutron_index].max_dp) {
                    mNeutronEventData[neutron_index].max_dp = dp;
                }
                if (dE > mNeutronEventData[neutron_index].max_dE) {
                    mNeutronEventData[neutron_index].max_dE = dE;
                }
                if (dphi > mNeutronEventData[neutron_index].max_dphi) {
                    mNeutronEventData[neutron_index].max_dphi = dphi;
                }   
            }
        }

        EndFunctionProfile("AddNeutronInfoFromStep");
    }

    void EventManager::EvaluateEvent()
    {
        for(size_t ii = 0; ii < mAnalysisFunctions.size(); ii++)
        {
            mAnalysisFunctions[ii]();
        }
    }
}