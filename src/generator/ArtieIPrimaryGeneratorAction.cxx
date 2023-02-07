/**
 * @file ArtieIPrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "ArtieIPrimaryGeneratorAction.hh"

namespace Artie
{
    ArtieIPrimaryGeneratorAction::ArtieIPrimaryGeneratorAction()
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

    ArtieIPrimaryGeneratorAction::~ArtieIPrimaryGeneratorAction()
    {
    }

#ifdef ARTIE_YAML
    ArtieIPrimaryGeneratorAction::ArtieIPrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config["generator"])
    {
        if(mConfig["use_lanl_distribution"]) { mUseLANLDistribution = mConfig["use_lanl_distribution"].as<G4bool>(); }
        if(mConfig["use_lanl_tof"]) { mUseLANLTOF = mConfig["use_lanl_tof"].as<G4bool>(); }

        if(mConfig["use_ntof_tof"]) { mUsenTOFTOF = mConfig["use_ntof_tof"].as<G4bool>(); }
        if(mConfig["use_ntof_beam_profile"]) { mUsenTOFBeamProfile = mConfig["use_ntof_beam_profile"].as<G4bool>(); }

        if(mConfig["flight_path"])      { mFlightPath = mConfig["flight_path"].as<G4double>() * m; }
        if(mConfig["t_zero_location"])  { mTZeroLocation = mConfig["t_zero_location"].as<G4double>() * m; }
        if(mConfig["energy_cut_low"])   { mEnergyCutLow = mConfig["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["energy_cut_high"])  { mEnergyCutHigh = mConfig["energy_cut_high"].as<G4double>() * keV; }
#ifdef ARTIE_ROOT
        mLANLEnergyDistribution = EventManager::GetEventManager()->GetLANLEnergyDistribution();
        mLANLTOF = EventManager::GetEventManager()->GetLANLTOF();

        mnTOFTOF = EventManager::GetEventManager()->GetnTOFTOF();
        mnTOFBeamProfile = EventManager::GetEventManager()->GetnTOFBeamProfile();

        mTRandom3 = new TRandom3();

#endif
        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0., mTZeroLocation));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);
    }
#endif

    G4double ArtieIPrimaryGeneratorAction::SampleBeamEnergy()
    {
#ifdef ARTIE_ROOT
        if(mUseLANLDistribution) {
            return mLANLEnergyDistribution->GetRandom() * keV;
        }
#endif
        return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
    }

    G4double ArtieIPrimaryGeneratorAction::SampleTOF(G4double beam_energy)
    {
#ifdef ARTIE_ROOT
        Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
        Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
        if(mUseLANLTOF) 
        {
            Int_t energy_bin = mLANLTOF->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetLANLTOFProjection(energy_bin);
            Double_t deltaT = TOF->GetRandom();       
            return deltaT * nominalTOF;
        }
        else if(mUsenTOFTOF)
        {
            /**
             * delta T is defined through:
             * lambda = v(E) * deltaT.  
             * Need to compute v(E) from relativistic KE.
             */
            Int_t energy_bin = mnTOFTOF->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetnTOFTOFProjection(energy_bin);
            Double_t lambda = TOF->GetRandom() * cm;   
            return lambda / nominalVelocity;
        }
#endif
        return 0.0;
    }

    G4ThreeVector ArtieIPrimaryGeneratorAction::SampleBeamProfile(G4double t_zero_location)
    {
        Double_t x = 0.0;
        Double_t y = 0.0;
#ifdef ARTIE_ROOT
        if(mUsenTOFBeamProfile) {
            mnTOFBeamProfile->GetRandom2(x, y, mTRandom3);
            x *= cm;
            y *= cm;
        }
#endif
        return G4ThreeVector(x, y, t_zero_location);
    }

    void ArtieIPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        mParticleGun->SetNumberOfParticles(1);
        G4double BeamEnergy = SampleBeamEnergy();
        G4double deltaTOF = SampleTOF(BeamEnergy);
        G4ThreeVector StartPosition = SampleBeamProfile(mTZeroLocation);

        mParticleGun->SetParticleTime(deltaTOF);
        mParticleGun->SetParticlePosition(StartPosition);
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}