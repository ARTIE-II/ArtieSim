/**
 * @file RunTest.cc
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief   
 * @version 0.0
 * @date 2022-04-27
 */
#include <iostream>
#include <vector>
#include <string>

// Geant4 includes 
#include "G4UIExecutive.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4ParticleHPManager.hh"

#include "Analysis.hh"
#include "Core.hh"
#include "ActionInitialization.hh"
#include "ArtieIDetector.hh"
#include "DetectorConstruction.hh"
#include "ArtieIGenerator.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventManager.hh"

int main(int argc, char** argv)
{
    // load in the config file
    G4String ConfigFile = "";
    if(argc == 2) {
        ConfigFile = argv[1];
    }
    else if(argc == 1) {
        ConfigFile = "artieI_argon.yaml";
    }
    else {
        std::cout << "ERROR! This program takes only one";
        std::cout << " command line argument for now.";
        std::cout << "  Must pass in a config file!" << std::endl;
        exit(0);
    }
    auto Config = Artie::ConfigParser(ConfigFile);
    Artie::EventManager::GetEventManager()->SetConfig(Config.GetConfig());

    G4UIExecutive* UIExecutive = 0;
    
    // create the run manager
#ifdef ARTIE_MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(
        Artie::EventManager::GetEventManager()->NumberOfThreads()
    );
#else
    G4RunManager* RunManager = new G4RunManager();
#endif

    // create the physics list
    auto PhysicsList = new Artie::PhysicsList();
    RunManager->SetUserInitialization(PhysicsList);
    PhysicsList->PrintPhysicsLists();
    G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
    theParticleIterator->reset();
    //loop on particles and get process manager from there list of processes
    while((*theParticleIterator)())
    {
        G4ParticleDefinition* pd = theParticleIterator->value();
        std::cout << pd->GetParticleName() << std::endl;
        G4ProcessManager* pm = pd->GetProcessManager();
        if(pm)
        {
            G4ProcessVector& procs = *(pm->GetProcessList());
            for ( G4int idx = 0 ; idx<procs.size() ; ++idx)
            {
                const G4VProcess* masterP = procs[idx]->GetMasterProcess();
                std::cout << masterP->GetProcessName() << std::endl;
                if ( ! masterP )
                {
                    //Process does not have an associated shadow master process
                    //We are in master mode or sequential
                    procs[idx]->SetMasterProcess(const_cast<G4VProcess*>(procs[idx]));
                }
            }
        }
        else {
            std::cout << "NONE" << std::endl;
        }
    }

    G4int poop;
    G4cin >> poop;

    // create the detector
    auto Detector = new Artie::ArtieIDetector(
        Config.GetConfig()["detector"]
    );
    auto detectorConstruction = new Artie::DetectorConstruction( 
        Detector
    );
    RunManager->SetUserInitialization(detectorConstruction);


    // create the action initialization
    auto ActionInitialization = new Artie::ActionInitialization();
    RunManager->SetUserInitialization(ActionInitialization);

    // create the generator
    auto Generator = new Artie::ArtieIGenerator(
        Config.GetConfig()["generator"]
    );
    Artie::EventManager::GetEventManager()->SetGenerator(Generator);

    // add analysis functions
    //Artie::EventManager::GetEventManager()->AddAnalysisFunction(Artie::ExampleAnalysisFunction);
    
    // start the session
    UIExecutive = new G4UIExecutive(argc, argv);
    
    if(Artie::EventManager::GetEventManager()->Mode() == "interactive")
    {
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
        RunManager->Initialize();
        G4UImanager* UIManager = G4UImanager::GetUIpointer();
        UIManager->ApplyCommand("/control/execute vis.mac");
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        UIExecutive->SessionStart();
    }
    else
    {
        RunManager->Initialize();
        G4UImanager* UIManager = G4UImanager::GetUIpointer();
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        G4String command = "/run/beamOn ";
        for(G4int run = 0; run < Artie::EventManager::GetEventManager()->NumberOfRuns(); run++)
        {
            UIManager->ApplyCommand(
                command + std::to_string(Artie::EventManager::GetEventManager()->NumberOfEvents())
            );
        }
    }
    delete RunManager;
    return 0;
}