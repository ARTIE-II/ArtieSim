/**
 * @file TrackingAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4UserTrackingAction.hh"
#include "G4Run.hh"
#include "G4GenericMessenger.hh"
#include "G4AnalysisManager.hh"

#include "EventAction.hh"
#include "Core.hh"

namespace Artie
{
    class TrackingAction : public G4UserTrackingAction
    {
    public:
        TrackingAction(std::shared_ptr<EventAction> EventAction);
        ~TrackingAction();
        
        virtual void PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);
        
    private:
        std::shared_ptr<EventAction> mEventAction;

    };
}