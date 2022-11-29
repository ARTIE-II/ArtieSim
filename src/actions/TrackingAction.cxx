/**
 * @file TrackingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "TrackingAction.hh"

namespace Artie
{
    TrackingAction::TrackingAction(std::shared_ptr<EventAction> eventAction)
    : G4UserTrackingAction()
    {
        mEventAction = eventAction;
    }

    TrackingAction::~TrackingAction()
    {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track)
    {
        auto Manager = EventManager::GetEventManager();
        Manager->AddParticleMapsFromTrack(track);
        Manager->AddPrimaryInfoFromTrackBegin(track);
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track)
    {
        auto Manager = EventManager::GetEventManager();
        Manager->AddPrimaryInfoFromTrackEnd(track);
    }

}