/**
 * @file ArtieIGenerator.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <vector>

#ifdef ARTIE_ROOT
#include "TFile.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TMath.h"
#endif

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif 

#include "Core.hh"
#include "Generator.hh"

namespace Artie
{
    class ArtieIGenerator : public Generator
    {
    public:
        ArtieIGenerator();
        ~ArtieIGenerator();

        ArtieIGenerator(G4double energyCutLow, G4double energyCutHigh);

#ifdef ARTIE_YAML
        ArtieIGenerator(YAML::Node config);
#endif

        void ConstructEnergyDistribution();

        std::vector<PrimaryGeneration> GeneratePrimaryList();

        // use the LANL beam distribution
        // to generate an energy value
        G4double SampleBeamEnergy();

        void SetTZeroLocation(G4double tzeroLocation) { mTZeroLocation = tzeroLocation; }
        void SetEnergyCutLow(G4double energyCutLow) { mEnergyCutLow = energyCutLow; }
        void SetEnergyCutHigh(G4double energyCutHigh) { mEnergyCutHigh = energyCutHigh; }
        void SetUseLANLDistribution(G4bool use) { mUseLANLDistribution = use; }

        G4double GetTZeroLocation() const { return mTZeroLocation; }
        G4double GetEnergyCutLow() const { return mEnergyCutLow; }
        G4double GetEnergyCutHigh() const { return mEnergyCutHigh; }
        G4bool GetUseLANLDistribution() const { return mUseLANLDistribution; }
        
    private:
        G4double mTZeroLocation = { -1.0 * m };
        G4double mEnergyCutLow = { 40 * keV };
        G4double mEnergyCutHigh = { 70 * keV };

#ifdef ARTIE_ROOT
        G4bool mUseLANLDistribution = { true };
        G4String mLANLDistributionFileName = {"resolution13a.root"};
        G4String mLANLDistributionName = {"tally5"};
        TFile* mLANLDistributionFile = {0};
        TH1D* mLANLDistribution = {0};
#endif

    };
}