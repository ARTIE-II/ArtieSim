/**
 * @file ActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ActionInitialization.hh"

namespace Artie
{
    ActionInitialization::ActionInitialization(
        PrimaryGeneratorAction* primaryGeneratorAction
    )
    {
        mGenerator = std::make_shared<PrimaryGeneratorAction>(*primaryGeneratorAction);
    }

    ActionInitialization::~ActionInitialization()
    {
    }

    void ActionInitialization::Build() const
    {
        SetUserAction(mGenerator.get());
        
        auto runAction = std::make_shared<RunAction>();
        SetUserAction(runAction.get());

        auto eventAction = std::make_shared<EventAction>(runAction);
        SetUserAction(eventAction.get());

        SetUserAction(new SteppingAction(eventAction));
        SetUserAction(new TrackingAction(eventAction));
    }

    void ActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}