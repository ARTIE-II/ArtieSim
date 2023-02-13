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
        if(mConfig["generate_perfect_beam"]) { mGeneratePerfectBeam = mConfig["generate_perfect_beam"].as<G4bool>(); }
        if(mConfig["generate_uniform_beam"]) { mGenerateUniformBeam = mConfig["generate_uniform_beam"].as<G4bool>(); }
        if(mConfig["uniform_beam_radius"])  { mUniformBeamRadius = mConfig["uniform_beam_radius"].as<G4double>() * cm; }

        if(mConfig["generate_uniform_discs"]) { mGenerateUniformDiscs = mConfig["generate_uniform_discs"].as<G4bool>(); }
        if(mConfig["uniform_disc_radius"])  { mUniformDiscRadius = mConfig["uniform_disc_radius"].as<G4double>() * cm; }  

        if(mConfig["generate_uniform_momentum"])    { mGenerateUniformMomentum = mConfig["generate_uniform_momentum"].as<G4bool>(); }
        if(mConfig["uniform_momentum_disc_radius"])    { mUniformMomentumRadius = mConfig["uniform_momentum_disc_radius"].as<G4double>() * cm; }
        if(mConfig["generate_signal_momentum"])    { mGenerateSignalMomentum = mConfig["generate_signal_momentum"].as<G4bool>(); }

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
        else if(mGenerateSignalMomentum)
        {
            G4double start_radius = EventManager::GetEventManager()->TZeroRadii();
            if(G4UniformRand() <= 0.5) {
                x = start_radius * G4UniformRand();
            }
            else {
                x = -start_radius * G4UniformRand();
            }
            G4double x_min = -sqrt(start_radius * start_radius - x*x);
            G4double x_max = +sqrt(start_radius * start_radius - x*x);
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
        if(mGenerateSignalMomentum)
        {
            G4double x_init = StartPosition[0];
            if(x_init < 0) {
                x_init += mBeamHoleSeparation * 0.5;
            }
            else {
                x_init -= mBeamHoleSeparation * 0.5;
            }
            G4double y_init = StartPosition[1];
            G4double z_init = StartPosition[2];
            G4double x_final = 0;
            G4double y_final = 0;
            G4double z_final = EventManager::GetEventManager()->SmallestRadiiZEnd();
            G4double z_final_begin = EventManager::GetEventManager()->SmallestRadiiZBegin();
            G4double intersect_radius = EventManager::GetEventManager()->SmallestRadii();
            G4cout << "z: " << z_final << "," << z_final_begin << G4endl;
            G4double r_init = sqrt(x_init * x_init + y_init * y_init);
            G4double r_max = intersect_radius;
            if(r_init > intersect_radius) {
                r_max -= ((r_init - intersect_radius) * (z_final - z_final_begin)/z_final_begin);
            }
            G4cout << "r: " << r_init << "," << r_max << G4endl;
            if(G4UniformRand() <= 0.5) {
                x_final = r_max * G4UniformRand();
            }
            else {
                x_final = -r_max * G4UniformRand();
            }
            G4double x_min = -sqrt(r_max * r_max - x_final*x_final);
            G4double x_max = +sqrt(r_max * r_max - x_final*x_final);
            if(G4UniformRand() <= 0.5) {
                y_final = (G4UniformRand() * (x_max - x_min) + x_min);
            }
            else {
                y_final = -(G4UniformRand() * (x_max - x_min) + x_min);
            }
            if(x_init < 0) {
                x_final -= mBeamHoleSeparation * 0.5;
            }
            else {
                x_final += mBeamHoleSeparation * 0.5;
            }
            G4double length = sqrt(
                (x_final - x_init)*(x_final - x_init) + 
                (y_final - y_init)*(y_final - y_init) + 
                (z_final - z_init)*(z_final - z_init)
            );
            G4ThreeVector Momentum(
                (x_final - x_init)/length,
                (y_final - y_init)/length,
                (z_final - z_init)/length
            );
            G4cout << x_final - x_init << "," << y_final - y_init << G4endl;
            G4cout << Momentum << G4endl;
            return Momentum;
        }
        else {
            return G4ThreeVector(0, 0, 1);
        }
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
        G4cout << mParticleGun->GetParticleMomentumDirection() << G4endl;
        mParticleGun->SetParticleMomentumDirection(StartMomentum);
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}