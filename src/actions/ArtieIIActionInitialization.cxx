/**
 * @file ArtieIIActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ArtieIIActionInitialization.hh"

namespace Artie
{
    ArtieIIActionInitialization::ArtieIIActionInitialization()
    {
    }

    ArtieIIActionInitialization::~ArtieIIActionInitialization()
    {
    }

#ifdef ARTIE_YAML
    ArtieIIActionInitialization::ArtieIIActionInitialization(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    void ArtieIIActionInitialization::Build() const
    {
#ifdef ARTIE_YAML
        SetUserAction(new ArtieIIPrimaryGeneratorAction(mConfig));
#else
        SetUserAction(new ArtieIIPrimaryGeneratorAction());
#endif 
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ArtieIIActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}