/**
 * @file ArtieIInTOFPrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "ArtieIInTOFPrimaryGeneratorAction.hh"

namespace Artie
{
    ArtieIInTOFPrimaryGeneratorAction::ArtieIInTOFPrimaryGeneratorAction()
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

    ArtieIInTOFPrimaryGeneratorAction::~ArtieIInTOFPrimaryGeneratorAction()
    {
    }

#ifdef ARTIE_YAML
    ArtieIInTOFPrimaryGeneratorAction::ArtieIInTOFPrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config["generator"])
    {
        if(mConfig["use_ntof_distribution"]) { mUsenTOFDistribution = mConfig["use_ntof_distribution"].as<G4bool>(); }
        if(mConfig["use_ntof_tof"])          { mUsenTOFTOF = mConfig["use_ntof_tof"].as<G4bool>(); }
        if(mConfig["use_ntof_beam_profile"]) { mUsenTOFBeamProfile = mConfig["use_ntof_beam_profile"].as<G4bool>(); }

        if(mConfig["t_zero_location"])  { mTZeroLocation = mConfig["t_zero_location"].as<G4double>() * m; }
        if(mConfig["energy_cut_low"])   { mEnergyCutLow = mConfig["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["energy_cut_high"])  { mEnergyCutHigh = mConfig["energy_cut_high"].as<G4double>() * keV; }
#ifdef ARTIE_ROOT
        //mnTOFEnergyDistribution = EventManager::GetEventManager()->GetnTOFEnergyDistribution();
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

    G4double ArtieIInTOFPrimaryGeneratorAction::SampleBeamEnergy()
    {
#ifdef ARTIE_ROOT
        // if(mUsenTOFDistribution) {
        //     return mnTOFEnergyDistribution->GetRandom() * keV;
        // }
#endif
        return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
    }

    G4double ArtieIInTOFPrimaryGeneratorAction::SampleTOF(G4double beam_energy)
    {
#ifdef ARTIE_ROOT
        Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
        Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
        if(mUsenTOFTOF) 
        {
            Int_t energy_bin = mnTOFTOF->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetnTOFTOFProjection(energy_bin);
            Double_t lambda = TOF->GetRandom() * cm;   
            return lambda / (nominalVelocity * 100);
        }
#endif
        return 0.0;
    }

    G4ThreeVector ArtieIInTOFPrimaryGeneratorAction::SampleBeamProfile(G4double t_zero_location)
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
    G4ThreeVector ArtieIInTOFPrimaryGeneratorAction::SampleBeamMomentum(G4ThreeVector StartPosition)
    {
        return G4ThreeVector(0, 0, 1);
    }

    void ArtieIInTOFPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
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