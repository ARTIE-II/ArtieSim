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
        if(mConfig["lanl_proton_pulse_FWHM"])  { mProtonPulseFWHM = mConfig["lanl_proton_pulse_FWHM"].as<G4double>(); }

        if(mConfig["use_lanl_beam_profile"]) { mUseLANLBeamProfile = mConfig["use_lanl_beam_profile"].as<G4bool>(); }
        if(mConfig["generate_single_beam"]) { mGenerateSingleBeam = mConfig["generate_single_beam"].as<G4bool>(); }
        if(mConfig["single_beam_location"]) { mSingleBeamLocation = mConfig["single_beam_location"].as<std::string>(); }

        if(mConfig["generate_perfect_beam"]) { mGeneratePerfectBeam = mConfig["generate_perfect_beam"].as<G4bool>(); }
        if(mConfig["generate_uniform_beam"]) { mGenerateUniformBeam = mConfig["generate_uniform_beam"].as<G4bool>(); }
        if(mConfig["uniform_beam_radius"])  { mUniformBeamRadius = mConfig["uniform_beam_radius"].as<G4double>() * cm; }

        if(mConfig["generate_uniform_discs"]) { mGenerateUniformDiscs = mConfig["generate_uniform_discs"].as<G4bool>(); }
        if(mConfig["uniform_disc_radius"])  { mUniformDiscRadius = mConfig["uniform_disc_radius"].as<G4double>() * cm; }  

        if(mConfig["generate_uniform_momentum"])    { mGenerateUniformMomentum = mConfig["generate_uniform_momentum"].as<G4bool>(); }
        if(mConfig["uniform_momentum_disc_radius"])    { mUniformMomentumRadius = mConfig["uniform_momentum_disc_radius"].as<G4double>() * cm; }

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
        if(mUseLANLTOF) 
        {
            // Double_t delPulse = SampleTriangularDist(mProtonPulseFWHM);
            Double_t deltaT;
            // beam_energy is in keV
            if (beam_energy <= 0.001) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(0);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.001 && beam_energy <= 0.01) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(1);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.01 && beam_energy <= 0.1) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(2);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 0.1 && beam_energy <= 1) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(3);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 1 && beam_energy <= 10) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(4);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 10 && beam_energy <= 30) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(5);
                deltaT = TOF->GetRandom();  
            }
            if (beam_energy > 30 && beam_energy <= 100) {
                TH1D* TOF = EventManager::GetEventManager()->GetDICERToFHist(6);
                deltaT = TOF->GetRandom();  
            }
            return deltaT * nominalTOF;
        }
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
        else if(mGenerateSingleBeam)
        {
            if(mSingleBeamLocation == "right") {
                x = mBeamHoleSeparation * 0.5;
            }
            else{
                x = -mBeamHoleSeparation * 0.5;
            }
        }
        else if(mGeneratePerfectBeam)
        {
            if(G4UniformRand() <= 0.5) {
                x = mBeamHoleSeparation * 0.5;
            }
            else{
                x = -mBeamHoleSeparation * 0.5;
            }
        }
        else if(mGenerateUniformBeam)
        {
            if(G4UniformRand() <= 0.5) {
                x = mUniformBeamRadius * G4UniformRand();
            }
            else {
                x = -mUniformBeamRadius * G4UniformRand();
            }
            G4double x_min = -sqrt(mUniformBeamRadius * mUniformBeamRadius - x*x);
            G4double x_max = +sqrt(mUniformBeamRadius * mUniformBeamRadius - x*x);
            if(G4UniformRand() <= 0.5) {
                y = (G4UniformRand() * (x_max - x_min) + x_min);
            }
            else {
                y = -(G4UniformRand() * (x_max - x_min) + x_min);
            }
        }
        else if(mGenerateUniformDiscs)
        {
            if(G4UniformRand() <= 0.5) {
                x = mUniformDiscRadius * G4UniformRand();
            }
            else {
                x = -mUniformDiscRadius * G4UniformRand();
            }
            G4double x_min = -sqrt(mUniformDiscRadius * mUniformDiscRadius - x*x);
            G4double x_max = +sqrt(mUniformDiscRadius * mUniformDiscRadius - x*x);
            if(G4UniformRand() <= 0.5) {
                y = (G4UniformRand() * (x_max - x_min) + x_min);
            }
            else {
                y = -(G4UniformRand() * (x_max - x_min) + x_min);
            }
            if(G4UniformRand() <= 0.5) {
                x -= mBeamHoleSeparation * 0.5;
            }
            else {
                x += mBeamHoleSeparation * 0.5;
            }
        }
#endif
        return G4ThreeVector(x, y, t_zero_location);
    }
    G4ThreeVector ArtieIIDICERPrimaryGeneratorAction::SampleBeamMomentum(G4ThreeVector StartPosition)
    {
        return G4ThreeVector(0, 0, 1);
    }

    void ArtieIIDICERPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
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