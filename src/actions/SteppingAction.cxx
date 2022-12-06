/**
 * @file SteppingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "SteppingAction.hh"

namespace Artie
{
    SteppingAction::SteppingAction()
    : G4UserSteppingAction()
    {
    }

    SteppingAction::~SteppingAction()
    {}

    void SteppingAction::UserSteppingAction(const G4Step *step)
    {   
        if(step->GetTrack()->GetGlobalTime() >= EventManager::GetEventManager()->EventMaxTime()) {
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        auto Manager = EventManager::GetEventManager();
        Manager->AddNeutronInfoFromStep(step);
    }
}