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

#include "Analysis.hh"
#include "ActionInitialization.hh"
#include "ArtieIDetector.hh"
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

    G4double TargetRadius =     2.5 / 2.0 * cm;
    G4double TargetLength =     168 * cm;
    G4double ContainerRadius =  3.49 / 2.0 * cm;
    G4double InsulationThickness = 10.0 * cm;
    G4double WindowThickness =  0.00762 * cm;
    G4double BufferLength =     5.0 * cm;
    G4double BeamPipeInnerRadius = 18.0 * cm;
    G4double BeamPipeOuterRadius = 20.0 * cm;

    G4double Gap =              2.5 * m;
    G4double WorldX =           4 * m;
    G4double WorldY =           4 * m;
    G4double WorldZ =           200 * m;
    G4double DetectorRadius =   2.0 * cm;
    G4double DetectorLength =   20.0 * cm;
    G4double DetectorEntrance = 69.0 * m;

    // create the argon cube detector
    auto detector = new Artie::ArtieIDetector(
        TargetRadius,
        TargetLength,
        ContainerRadius,
        InsulationThickness,
        WindowThickness,
        BufferLength,
        BeamPipeInnerRadius,
        BeamPipeOuterRadius,
        Gap,
        WorldZ,
        DetectorRadius,
        DetectorLength,
        DetectorEntrance
    );

    auto detectorConstruction = new Artie::DetectorConstruction(
        WorldX, 
        WorldY, 
        WorldZ, 
        detector
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