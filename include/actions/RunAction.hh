/**
 * @file RunAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4GenericMessenger.hh"
#include "G4AnalysisManager.hh"

#include "EventManager.hh"
#include "PrimaryGeneratorAction.hh"

namespace Artie
{
    class RunAction : public G4UserRunAction
    {
    public:
        RunAction();
        ~RunAction();

        void ConstructRootTrees();
        
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

    private:
    
    };
}