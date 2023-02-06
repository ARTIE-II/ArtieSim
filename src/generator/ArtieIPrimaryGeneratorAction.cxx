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
        if(mConfig["use_lanl_beam_profile"]) { mUseLANLBeamProfile = mConfig["use_lanl_beam_profile"].as<G4bool>(); }
        if(mConfig["flight_path"])      { mFlightPath = mConfig["flight_path"].as<G4double>() * m; }
        if(mConfig["t_zero_location"])  { mTZeroLocation = mConfig["t_zero_location"].as<G4double>() * m; }
        if(mConfig["energy_cut_low"])   { mEnergyCutLow = mConfig["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["energy_cut_high"])  { mEnergyCutHigh = mConfig["energy_cut_high"].as<G4double>() * keV; }
#ifdef ARTIE_ROOT
        mLANLEnergyDistribution = EventManager::GetEventManager()->GetLANLEnergyDistribution();
        mLANLBeamProfile = EventManager::GetEventManager()->GetLANLBeamProfile();
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

    G4double ArtieIPrimaryGeneratorAction::SampleModeratorFunction(G4double beam_energy)
    {
#ifdef ARTIE_ROOT
        if(mUseLANLBeamProfile) {
            Int_t energy_bin = mLANLBeamProfile->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetLANLBeamProjection(energy_bin);
            Double_t deltaT = TOF->GetRandom();       
            return deltaT;
        }
#endif
        return 0.0;
    }

    void ArtieIPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        mParticleGun->SetNumberOfParticles(1);
        G4double BeamEnergy = SampleBeamEnergy();
        Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(BeamEnergy);
        G4double Moderator = SampleModeratorFunction(BeamEnergy);
        mParticleGun->SetParticleTime(Moderator * nominalTOF);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0., mTZeroLocation));
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}