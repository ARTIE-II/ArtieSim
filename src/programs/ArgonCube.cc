/**
 * @file ArgonSphere.cc
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

#include "Argon.hh"
#include "Analysis.hh"
#include "ActionInitialization.hh"
#include "ArgonCubeDetector.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* UIExecutive = 0;
    

    // create the run manager
#ifdef ARTIE_MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(8);
#else
    G4RunManager* RunManager = new G4RunManager();
#endif

    // create the physics list
    auto PhysicsList = new Artie::PhysicsList();
    RunManager->SetUserInitialization(PhysicsList);

    // create the argon cube detector
    auto detector = new Artie::ArgonCubeDetector(
        10 * cm, 10 * cm, 10 * cm,
        8, 8, 1 * mm
    );
    auto detectorConstruction = new Artie::DetectorConstruction(
        100 * cm, 100 * cm, 100 * cm, detector
    );
    RunManager->SetUserInitialization(detectorConstruction);

    // create the action initialization
    auto PrimaryGeneratorAction = new Artie::PrimaryGeneratorAction();
    auto ActionInitialization = new Artie::ActionInitialization(PrimaryGeneratorAction);
    RunManager->SetUserInitialization(ActionInitialization);

    // add analysis functions
    Artie::EventManager::GetEventManager()->AddAnalysisFunction(Artie::ExampleAnalysisFunction);

    // start the session
    if (argc == 1)
    {
        UIExecutive = new G4UIExecutive(argc, argv);
    }
#ifdef ARTIE_USE_VIS
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
#endif
        RunManager->Initialize();
#ifdef ARTIE_USE_UI
    G4UImanager* UIManager = G4UImanager::GetUIpointer();

    // start the session
    if (argc == 1)
    {
        UIManager->ApplyCommand("/control/execute vis.mac");
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        UIExecutive->SessionStart();
    }
    else
    {
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UIManager->ApplyCommand(command+fileName);
    }
#endif

#ifdef ARTIE_USE_UI_TCSH
        auto UITerminal = std::make_shared<G4UIterminal>(new G4UItcsh);
#else   
        auto UITerminal = std::make_shared<G4UIterminal>();
#endif
        UITerminal->SessionStart();

    return 0;
}