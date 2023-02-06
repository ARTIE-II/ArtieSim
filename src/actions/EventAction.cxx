/**
 * @file EventAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "EventAction.hh"

namespace Artie
{
    EventAction::EventAction()
    : G4UserEventAction()
    {
        auto Manager = EventManager::GetEventManager();
        // Manager->ClearEventData();
    }

    EventAction::~EventAction()
    {}

    void EventAction::BeginOfEventAction(const G4Event* event)
    {
        auto Manager = EventManager::GetEventManager();
        // Manager->ClearEventData();
    }

    void EventAction::EndOfEventAction(const G4Event* event)
    {
        auto Manager = EventManager::GetEventManager();
        Manager->FillParticleMaps(event->GetEventID());
        Manager->FillPrimaryInfo(event->GetEventID());
        Manager->FillHits(event->GetEventID());
        Manager->FillNeutronEventData(event->GetEventID());
        
        // Send out tuples to analysis functions
        Manager->EvaluateEvent();
        Manager->ClearEventData();
    }
}