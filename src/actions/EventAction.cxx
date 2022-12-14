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
    }

    EventAction::~EventAction()
    {}

    void EventAction::BeginOfEventAction(const G4Event*)
    {
    }

    void EventAction::EndOfEventAction(const G4Event* event)
    {
    }
}