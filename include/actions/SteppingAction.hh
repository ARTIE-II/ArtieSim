/**
 * @file SteppingAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "EventAction.hh"
#include "Core.hh"

namespace Artie
{
    class SteppingAction : public G4UserSteppingAction
    {
    public:
        SteppingAction(std::shared_ptr<EventAction> EventAction);
        ~SteppingAction();
        
        virtual void UserSteppingAction(const G4Step*);
        
    private:
        std::shared_ptr<EventAction> mEventAction;

    };
}