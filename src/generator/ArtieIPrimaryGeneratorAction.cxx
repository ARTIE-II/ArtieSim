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
#ifdef ARTIE_ROOT
        mLANLDistributionFile->Close();
#endif
    }

#ifdef ARTIE_YAML
    ArtieIPrimaryGeneratorAction::ArtieIPrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config["generator"])
    {
        if(mConfig["use_lanl_distribution"])       { mUseLANLDistribution = mConfig["use_lanl_distribution"].as<G4bool>(); }
#ifdef ARTIE_ROOT
        if(mConfig["lanl_distribution_filename"])  { mLANLDistributionFileName = mConfig["lanl_distribution_filename"].as<std::string>(); }
        if(mConfig["lanl_distribution_name"])      { mLANLDistributionName = mConfig["lanl_distribution_name"].as<std::string>(); }
#endif
        if(mConfig["t_zero_location"]) { mTZeroLocation = mConfig["t_zero_location"].as<G4double>() * m; }
        if(mConfig["energy_cut_low"])  { mEnergyCutLow = mConfig["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["energy_cut_high"]) { mEnergyCutHigh = mConfig["energy_cut_high"].as<G4double>() * keV; }

        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0., mTZeroLocation));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);

        ConstructEnergyDistribution();
    }
#endif

    void ArtieIPrimaryGeneratorAction::ConstructEnergyDistribution()
    {
#ifdef ARTIE_ROOT
        mLANLDistributionFile = new TFile(mLANLDistributionFileName);
        TGraph *DistributionGraph = (TGraph*)mLANLDistributionFile->Get(mLANLDistributionName);

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

        mLANLDistribution = new TH1D("LANLBeamEnergy", "LANLBeamEnergy", nlogbins, xbins);
        auto nPoints = DistributionGraph->GetN(); // number of points 
        G4double x, y;
        for(G4int i=0; i < nPoints; ++i) {
            DistributionGraph->GetPoint(i, x, y); //eV
            if( 
                x / 1000 > mEnergyCutLow && 
                x / 1000 < mEnergyCutHigh
            ) 
            {
                mLANLDistribution->Fill(x,y);
            }
        }
#endif
    }

    G4double ArtieIPrimaryGeneratorAction::SampleBeamEnergy()
    {
#ifdef ARTIE_ROOT
        if(mUseLANLDistribution) {
            G4double energy = mLANLDistribution->GetRandom();
            return mLANLDistribution->GetRandom() * keV;
        }
#endif
        return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
    }

    void ArtieIPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        mParticleGun->SetNumberOfParticles(1);
        G4double BeamEnergy = SampleBeamEnergy();
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}