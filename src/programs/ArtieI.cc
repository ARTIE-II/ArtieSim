/**
 * @file    main.cc
 * @brief   The main function for the neutron simulation
 * @ingroup main
 * @author  Gian Caceres [gcaceres@ucdavis.edu], Nicholas Carrara [nmcarrara@ucdavis.edu],
**/
#include <iostream>
#include <vector>
#include <string>

// Geant4 includes 
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
// Neutron HP
#include "G4ParticleHPManager.hh"
#include "G4Types.hh"

#include "ArtieIDetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ArtieIActionInitialization.hh"
#include "Analysis.hh"
#include "Core.hh"

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

    // create a user session
    G4UIExecutive* UIExecutive = 0;

#ifdef G4MULTITHREADED
      G4MTRunManager* runManager = new G4MTRunManager();
      runManager->SetNumberOfThreads(8);
#else
      G4RunManager* runManager = new G4RunManager;
#endif
    
    // apply the detector, physics list and initialization
    runManager->SetUserInitialization(new Artie::ArtieIDetectorConstruction(Config.GetConfig()));
    runManager->SetUserInitialization(new Artie::PhysicsList());
    runManager->SetUserInitialization(new Artie::ArtieIActionInitialization(Config.GetConfig()));
    
    // Replaced HP environmental variables with C++ calls                                                                                     
    G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
    G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
    G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
    G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
    G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
    G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
    G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

    runManager->Initialize();

    // print out available physics lists
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    const std::vector<G4String> physicsLists = physListFactory->AvailablePhysLists();
    std::cout << "Enabled Physics Lists:" << std::endl;
    for(size_t i = 0; i < physicsLists.size(); i++)
    {
        std::cout << "\t[" << i << "]: " << physicsLists[i] << std::endl;
    }
    // print out all processes for neutrons
    G4ParticleDefinition* neutron = G4Neutron::Neutron();
    G4ProcessManager* pManager = neutron->GetProcessManager();
    G4ProcessVector* processes = pManager->GetProcessList();
    std::cout << "Enabled Neutron HP Physics Processes:" << std::endl;
    for(size_t i = 0; i < processes->size(); i++)
    {
        std::cout << "\t[" << i << "]: " << (*processes)[i]->GetProcessName() << std::endl;
    }

    UIExecutive = new G4UIExecutive(argc, argv);
    if(Config.GetConfig()["manager"]["mode"].as<std::string>() == "interactive")
    {
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
        runManager->Initialize();
        G4UImanager* UIManager = G4UImanager::GetUIpointer();
        UIManager->ApplyCommand("/control/execute vis.mac");
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        UIExecutive->SessionStart();
    }
    else
    {
        runManager->Initialize();
        G4ParticleDefinition* neutron = G4Neutron::Neutron();
        G4ProcessManager* pManager = neutron->GetProcessManager();
        G4ProcessVector* processes = pManager->GetProcessList();
        std::cout << "Enabled Neutron HP Physics Processes:" << std::endl;
        for(size_t i = 0; i < processes->size(); i++)
        {
            std::cout << "\t[" << i << "]: " << (*processes)[i]->GetProcessName() << std::endl;
        }
        G4UImanager* UIManager = G4UImanager::GetUIpointer();
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        G4String command = "/run/beamOn ";
        for(G4int run = 0; run < Config.GetConfig()["manager"]["number_of_runs"].as<G4int>(); run++)
        {
            UIManager->ApplyCommand(
                command + Config.GetConfig()["manager"]["number_of_events"].as<std::string>()
            );
        }
    }

    return 0;
}