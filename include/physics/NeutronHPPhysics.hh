/**
 * @file NeutronHPPhysics.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticProcess.hh"
#include "G4NeutronFissionProcess.hh"
#include "G4NeutronCaptureProcess.hh"
#include "G4HadronInelasticProcess.hh"

#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4PhysListFactory.hh"
#include "globals.hh"

namespace Artie
{
    class NeutronHPPhysics : public G4VPhysicsConstructor
    {
    public:
        NeutronHPPhysics(const G4String& name = "neutronHP", G4bool thermal = true);
        ~NeutronHPPhysics();

        virtual void ConstructParticle() {};
        virtual void ConstructProcess();

        void PrintNeutronPhysicsLists();
        
        void SetThermalPhysics(G4bool flag) { mThermalNeutrons = flag; }
        G4bool GetThermalPhysics() const { return mThermalNeutrons; } 
        
    private:
        G4bool mThermalNeutrons;
        std::shared_ptr<G4ProcessManager> mPhysicsManager = {nullptr};
        std::shared_ptr<G4ProcessVector> mProcesses = {nullptr};

        std::shared_ptr<G4PhysListFactory> mPhysListFactory;
    };
}