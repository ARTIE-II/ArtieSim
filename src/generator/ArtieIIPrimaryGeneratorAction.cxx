/**
 * @file ArtieIIPrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "ArtieIIPrimaryGeneratorAction.hh"

namespace Artie
{
    ArtieIIPrimaryGeneratorAction::ArtieIIPrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
    {

        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);
    }

    ArtieIIPrimaryGeneratorAction::~ArtieIIPrimaryGeneratorAction()
    {
    }

    ArtieIIPrimaryGeneratorAction::ArtieIIPrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config)
    {
        if(mConfig["generator"]["t_zero_location"])  { 
            mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m; 
        }
        if(mConfig["generator"]["energy_cut_low"])   { 
            mEnergyCutLow = mConfig["generator"]["energy_cut_low"].as<G4double>() * keV; 
        }
        if(mConfig["generator"]["energy_cut_high"])  { 
            mEnergyCutHigh = mConfig["generator"]["energy_cut_high"].as<G4double>() * keV; 
        }
        
        std::string distribution_type = mConfig["generator"]["energy_distribution"]["distribution_type"].as<std::string>();
        std::string profile_type = mConfig["generator"]["beam_profile"]["profile_type"].as<std::string>();
        std::string tof_type = mConfig["generator"]["time_of_flight"]["tof_type"].as<std::string>();

        // Set up energy distribution 
        if(distribution_type == "lanl")
        {
            mUseLANLDistribution = true;
            mLANLEnergyDistribution = EventManager::GetEventManager()->GetLANLEnergyDistribution();
        }
        else if(distribution_type == "ntof")
        {
            mUsenTOFDistribution = true;
            //mnTOFEnergyDistribution = EventManager::GetEventManager()->GetnTOFEnergyDistribution();
            mnTOFEnergyDistribution = EventManager::GetEventManager()->GetLANLEnergyDistribution();
        }
        else
        {
            mUseUniformDistribution = true;
        }
        // Set up beam profile
        if(profile_type == "lanl")
        {
            mUseLANLBeamProfile = true;
            mLANLBeamProfile = EventManager::GetEventManager()->GetLANLBeamProfile();
            mLANLBeamLocation = 0;
            std::string beam_location = mConfig["generator"]["beam_profile"]["beam_location"].as<std::string>();
            mBeamHoleSeparation = mConfig["dicer"]["rbb_hole_separation"].as<G4double>() * cm; 
            if(beam_location == "left") {
                mLANLBeamLocation = -1;
            }
            else if(beam_location == "right") {
                mLANLBeamLocation = 1;
            }
            else if(beam_location == "profile") {
                mLANLBeamLocation = 2;
            }
        }
        else if(profile_type == "ntof")
        {
            mUsenTOFDistribution = true;
            mnTOFBeamProfile = EventManager::GetEventManager()->GetnTOFBeamProfile();
        }
        // Set up TOF
        if(tof_type == "lanl")
        {
            mUseLANLTOF = true;
        }
        else if(tof_type == "ntof")
        {
            mUsenTOFTOF = true;
        }        
        
        mTRandom3 = new TRandom3();
        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0., mTZeroLocation));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);
    }

    G4double ArtieIIPrimaryGeneratorAction::SampleBeamEnergy()
    {

        if(mUseLANLDistribution) {
            G4double result = mLANLEnergyDistribution->GetRandom();
            //int i = 0;
            while (result < 150) {
                result = mLANLEnergyDistribution->GetRandom();
                //std::cout<<"i: "<<i<<" result: "<<result<<std::endl;
                //i++;
            }
            return result * keV;
        }
        else if(mUsenTOFDistribution) {
            return mnTOFEnergyDistribution->GetRandom() * keV;
        }
        else {
            /*double base = 10;
            double minValue = mEnergyCutLow;
            double maxValue = mEnergyCutHigh;
            double logMin = std::log(minValue) / std::log(base);
            double logMax = std::log(maxValue) / std::log(base);
            double logValue = logMin + (G4UniformRand() * (logMax - logMin));
            return std::pow(base, logValue); */
            return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
        }
    }

    G4double ArtieIIPrimaryGeneratorAction::SampleTOF(G4double beam_energy)
    {
        if(mUseLANLTOF) 
        {
            Double_t deltaT;
            // beam_energy is in keV
            if (beam_energy <= 0.000001) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(0);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.000001 && beam_energy <= 0.00001) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(1);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.00001 && beam_energy <= 0.0001) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(2);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.0001 && beam_energy <= 0.001) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(3);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.001 && beam_energy <= 0.01) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(4);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.01 && beam_energy <= 0.03) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(5);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.03) {
                TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFHist(6);
                deltaT = TOF->GetRandom();  
            }
            return deltaT * ns;
        }
        else if(mUsenTOFTOF)
        {
            Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
            Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
            Int_t energy_bin = mnTOFTOF->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetnTOFTOFProjection(energy_bin);
            Double_t lambda = TOF->GetRandom() * cm;   
            return lambda / (nominalVelocity);
        }
        return 0.0;
    }

    G4ThreeVector ArtieIIPrimaryGeneratorAction::SampleBeamProfile(
        G4double t_zero_location
    )
    {
        Double_t x = 0.0;
        Double_t y = 0.0;

        if(mUseLANLBeamProfile) 
        {
            if(mLANLBeamLocation == 0) {
                if(G4UniformRand() <= 0.5) {
                    x = -mBeamHoleSeparation * 0.5;
                }
                else {
                    x = mBeamHoleSeparation * 0.5;
                }
            }
            else if(mLANLBeamLocation == -1) {
                x = -mBeamHoleSeparation * 0.5;
            }
            else if(mLANLBeamLocation == 1) {
                x = mBeamHoleSeparation * 0.5;
            }
            else {
                mLANLBeamProfile->GetRandom2(x, y, mTRandom3);
                x *= cm;
                y *= cm;
            }
        }
        else if(mUsenTOFBeamProfile) {
            mnTOFBeamProfile->GetRandom2(x, y, mTRandom3);
        }
        return G4ThreeVector(x, y, t_zero_location);
    }

    G4ThreeVector ArtieIIPrimaryGeneratorAction::SampleBeamMomentum(
        G4ThreeVector StartPosition
    )
    {
        return G4ThreeVector(0, 0, 1);
    }

    void ArtieIIPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        std::cout<<"event id: "<<event->GetEventID()<<std::endl;
        mParticleGun->SetNumberOfParticles(1);
        G4double BeamEnergy = SampleBeamEnergy();
        G4double deltaTOF = SampleTOF(BeamEnergy);
        G4ThreeVector StartPosition = SampleBeamProfile(mTZeroLocation);
        G4ThreeVector StartMomentum = SampleBeamMomentum(StartPosition);
        mParticleGun->SetParticleTime(deltaTOF);
        mParticleGun->SetParticlePosition(StartPosition);
        mParticleGun->SetParticleMomentumDirection(StartMomentum);
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}