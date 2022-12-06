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
    ActionInitialization::ActionInitialization()
    {
    }

    ActionInitialization::~ActionInitialization()
    {
    }

    void ActionInitialization::Build() const
    {
        SetUserAction(new PrimaryGeneratorAction());
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}