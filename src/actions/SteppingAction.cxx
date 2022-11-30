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
    SteppingAction::SteppingAction(std::shared_ptr<EventAction> eventAction)
    : G4UserSteppingAction()
    {
        mEventAction = eventAction;
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