/**
 * @file PhysicsList.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "PhysicsList.hh"

namespace Artie
{
    PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
    {   
        // Standard EM Physics
        RegisterPhysics(new G4EmStandardPhysics());

        // Synchroton Radiation & GN Physics
        RegisterPhysics(new G4EmExtraPhysics());

        // Optical Physics
        RegisterPhysics(new G4OpticalPhysics());

        // Decay Physics
        RegisterPhysics(new G4DecayPhysics());
        RegisterPhysics(new G4RadioactiveDecayPhysics());

        // Stopping Physics
        RegisterPhysics(new G4StoppingPhysics());

        // Ion Physics
        RegisterPhysics(new G4IonPhysics());

        // Hadron Elastic scattering
        RegisterPhysics(new G4HadronElasticPhysicsHP());

        // Hadron Physics
        RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());
        RegisterPhysics(new G4StepLimiterPhysics());
        RegisterPhysics(new G4FastSimulationPhysics("fastSimPhys"));

        // Neutron Physics
        //RegisterPhysics(new NeutronHPPhysics("neutronHP"));

        // Get the list of physics lists
        mPhysListFactory = std::make_shared<G4PhysListFactory>();
        mPhysicsLists = mPhysListFactory->AvailablePhysLists();
    }

    PhysicsList::~PhysicsList()
    {
    }

    void PhysicsList::SetCuts()
    {
        SetCutValue(1 * mm, "proton");
        SetCutValue(1 * um, "gamma");
        SetCutValue(1 * um, "e-");
        SetCutValue(1 * um, "e+");
    }

    void PhysicsList::PrintPhysicsLists()
    {
        G4cout << "Enabled Physics Lists:" << G4endl;
        for(size_t ii = 0; ii < mPhysicsLists.size(); ii++)
        {
            G4cout << "\t[" << ii << "]: " << mPhysicsLists[ii] << G4endl;
        }
    }
}