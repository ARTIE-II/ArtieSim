/**
 * @file TrackingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "TrackingAction.hh"

namespace Artie
{
    TrackingAction::TrackingAction()
    : G4UserTrackingAction()
    {
    }

    TrackingAction::~TrackingAction()
    {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track)
    {
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track)
    {
    }

}