/**
 * @file ArtieIGenerator.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ArtieIGenerator.hh"

namespace Artie
{
    ArtieIGenerator::ArtieIGenerator()
    : Generator()
    {
        ConstructEnergyDistribution();
    }

#ifdef ARTIE_YAML
    ArtieIGenerator::ArtieIGenerator(YAML::Node config)
    : Generator(config)
    {
        if(Config()["use_lanl_distribution"])       { mUseLANLDistribution = Config()["use_lanl_distribution"].as<G4bool>(); }
#ifdef ARTIE_ROOT
        if(Config()["lanl_distribution_filename"])  { mLANLDistributionFileName = Config()["lanl_distribution_filename"].as<std::string>(); }
        if(Config()["lanl_distribution_name"])      { mLANLDistributionName = Config()["lanl_distribution_name"].as<std::string>(); }
#endif
        if(Config()["t_zero_location"]) { mTZeroLocation = Config()["t_zero_location"].as<G4double>() * m; }
        if(Config()["energy_cut_low"])  { mEnergyCutLow = Config()["energy_cut_low"].as<G4double>() * keV; }
        if(Config()["energy_cut_high"]) { mEnergyCutHigh = Config()["energy_cut_high"].as<G4double>() * keV; }
        ConstructEnergyDistribution();
    }
#endif

    void ArtieIGenerator::ConstructEnergyDistribution()
    {
#ifdef ARTIE_ROOT
        mLANLDistributionFile = new TFile(mLANLDistributionFileName);
        TGraph *DistributionGraph = (TGraph*)mLANLDistributionFile->Get(mLANLDistributionName);
        
        // Make variable-bin histogram for beam energy
        const G4int nlogbins=500;        
        G4double xmin = 1.e-3;  //eV
        G4double xmax = 1.e7;   //eV
        G4double *xbins    = new G4double[nlogbins+1];
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
            ) {
                mLANLDistribution->Fill(x,y);
            }
        }
#endif
    }

    ArtieIGenerator::~ArtieIGenerator()
    {
#ifdef ARTIE_ROOT
        mLANLDistributionFile->Close();
#endif
    }

    ArtieIGenerator::ArtieIGenerator(G4double energyCutLow, G4double energyCutHigh)
    : mEnergyCutLow(energyCutLow)
    , mEnergyCutHigh(energyCutHigh)
    {    
    }

    G4double ArtieIGenerator::SampleBeamEnergy()
    {
#ifdef ARTIE_ROOT
        if(mUseLANLDistribution) {
            return mLANLDistribution->GetRandom() / 1000 * keV;
        }
#endif
        return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand()) * keV;
    }

    std::vector<PrimaryGeneration> ArtieIGenerator::GeneratePrimaryList()
    {
        std::vector<PrimaryGeneration> primaries;
        primaries.emplace_back(
            PrimaryGeneration(
                "neutron",
                0,
                {0., 0., mTZeroLocation},
                {SampleBeamEnergy()},
                {0., 0., 1.}
            )
        );
        return primaries;
    }
}