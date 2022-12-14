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
#include "G4GDMLParser.hh"

#include "ArtieIDetectorConstruction.hh"
#include "PhysicsList.hh"
#include "EventManager.hh"
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
    else 
    {
        std::cout << "ERROR! This program takes only one";
        std::cout << " command line argument for now.";
        std::cout << "  Must pass in a config file!" << std::endl;
        exit(0);
    }
    auto Config = Artie::ConfigParser(ConfigFile);
    Artie::EventManager::GetEventManager()->SetConfig(Config.GetConfig());

    // choose the Random engine
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // create a user session
    G4UIExecutive* UIExecutive = 0;

#ifdef G4MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(
        Artie::EventManager::GetEventManager()->NumberOfThreads()
    );
#else
    G4RunManager* RunManager = new G4RunManager;
#endif
    
    // apply the detector, physics list and initialization
    RunManager->SetUserInitialization(new Artie::ArtieIDetectorConstruction(Config.GetConfig()));
    RunManager->SetUserInitialization(new Artie::PhysicsList());
    RunManager->SetUserInitialization(new Artie::ArtieIActionInitialization(Config.GetConfig()));
    
    // Replaced HP environmental variables with C++ calls                                                                                     
    G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
    G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
    // G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
    // G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
    // G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
    // G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
    // G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

    if(Config.GetConfig()["manager"]["mode"].as<std::string>() == "interactive")
    {
        UIExecutive = new G4UIExecutive(argc, argv);
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
        RunManager->Initialize();
        G4UImanager* UIManager = G4UImanager::GetUIpointer();
        UIManager->ApplyCommand("/control/execute macros/vis.mac");
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
        for(G4int run = 0; run < Config.GetConfig()["manager"]["number_of_runs"].as<G4int>(); run++)
        {
            UIManager->ApplyCommand(
                command + Config.GetConfig()["manager"]["number_of_events"].as<std::string>()
            );
        }
    }
    Artie::EventManager::GetEventManager()->SaveGDML();

    return 0;
}