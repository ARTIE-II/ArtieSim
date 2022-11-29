/**
 * @file EventAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include <map>
#include "globals.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "EventManager.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

namespace Artie
{
    /**
     * @brief All detector MC must be managed here at the
     * event level!
     * 
     * @details
     * 	Change log:
     * 		2022-09-22 - Initial creation of file.
     */
    class EventAction : public G4UserEventAction
    {
    public:
        EventAction(std::shared_ptr<RunAction> RunAction);
        ~EventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

    private:
        std::shared_ptr<RunAction> mRunAction;

    };
}