/**
 * @file ArtieIPrimaryGeneratorAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "EventManager.hh"

#ifdef ARTIE_ROOT
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMath.h"
#endif

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

namespace Artie
{
    class ArtieIPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {   
    public:
        ArtieIPrimaryGeneratorAction();
        ~ArtieIPrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event* event);
        void ConstructEnergyDistribution();
        G4double SampleBeamEnergy();
        G4double SampleModeratorFunction(G4double beam_energy);

#ifdef ARTIE_YAML
        ArtieIPrimaryGeneratorAction(YAML::Node config);
        YAML::Node Config() const { return mConfig; }
#endif

    private:
        G4ParticleGun* mParticleGun;

        G4String mParticleName;
        G4double mParticleMomentum;
        G4double mParticleEnergy;
        G4ThreeVector mParticlePosition;
        G4ThreeVector mParticleMomentumDirection;

        G4double mFlightPath = { 64.11 * m };
        G4double mTZeroLocation = { -1.0 * m };
        G4double mEnergyCutLow = { 40 * keV };
        G4double mEnergyCutHigh = { 70 * keV };

        G4bool mUseLANLDistribution = { false };

#ifdef ARTIE_ROOT
        TH1D* mLANLEnergyDistribution = {0};
        TH2D* mLANLBeamProfile = {0};
#endif

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif

    };
}