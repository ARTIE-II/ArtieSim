/**
 * @file ArtieIIDICERPrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "ArtieIIDICERPrimaryGeneratorAction.hh"

namespace Artie
{
    ArtieIIDICERPrimaryGeneratorAction::ArtieIIDICERPrimaryGeneratorAction()
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

    ArtieIIDICERPrimaryGeneratorAction::~ArtieIIDICERPrimaryGeneratorAction()
    {
    }

#ifdef ARTIE_YAML
    ArtieIIDICERPrimaryGeneratorAction::ArtieIIDICERPrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config["generator"])
    {
        if(mConfig["use_lanl_distribution"]) { mUseLANLDistribution = mConfig["use_lanl_distribution"].as<G4bool>(); }
        if(mConfig["use_lanl_tof"]) { mUseLANLTOF = mConfig["use_lanl_tof"].as<G4bool>(); }
        if(mConfig["use_lanl_beam_profile"]) { mUseLANLBeamProfile = mConfig["use_lanl_beam_profile"].as<G4bool>(); }

        if(mConfig["beam_hole_separation"]) { mBeamHoleSeparation = mConfig["beam_hole_separation"].as<G4double>() * cm; }
        if(mConfig["t_zero_location"])  { mTZeroLocation = mConfig["t_zero_location"].as<G4double>() * m; }
        if(mConfig["energy_cut_low"])   { mEnergyCutLow = mConfig["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["energy_cut_high"])  { mEnergyCutHigh = mConfig["energy_cut_high"].as<G4double>() * keV; }
#ifdef ARTIE_ROOT
        mLANLEnergyDistribution = EventManager::GetEventManager()->GetLANLEnergyDistribution();
        mLANLTOF = EventManager::GetEventManager()->GetLANLTOF();

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

    G4double ArtieIIDICERPrimaryGeneratorAction::SampleBeamEnergy()
    {
#ifdef ARTIE_ROOT
        if(mUseLANLDistribution) {
            return mLANLEnergyDistribution->GetRandom() * keV;
        }
#endif
        return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
    }

    G4double ArtieIIDICERPrimaryGeneratorAction::SampleTOF(G4double beam_energy)
    {
#ifdef ARTIE_ROOT
        Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
        Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
#endif
        return 0.0;
    }

    G4ThreeVector ArtieIIDICERPrimaryGeneratorAction::SampleBeamProfile(G4double t_zero_location)
    {
        Double_t x = 0.0;
        Double_t y = 0.0;
#ifdef ARTIE_ROOT
        if(mUseLANLBeamProfile) 
        {
            mLANLBeamProfile->GetRandom2(x, y, mTRandom3);
            x *= cm;
            y *= cm;
        }
        else 
        {
            if(G4UniformRand() <= 0.5) {
                x = mBeamHoleSeparation * 0.5;
            }
            else {
                x = -mBeamHoleSeparation * 0.5;
            }
        }
#endif
        return G4ThreeVector(x, y, t_zero_location);
    }

    void ArtieIIDICERPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
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