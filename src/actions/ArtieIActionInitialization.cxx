/**
 * @file ArtieIActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ArtieIActionInitialization.hh"

namespace Artie
{
    ArtieIActionInitialization::ArtieIActionInitialization()
    {
    }

    ArtieIActionInitialization::~ArtieIActionInitialization()
    {
    }

#ifdef ARTIE_YAML
    ArtieIActionInitialization::ArtieIActionInitialization(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    void ArtieIActionInitialization::Build() const
    {
#ifdef ARTIE_YAML
        SetUserAction(new ArtieIPrimaryGeneratorAction(mConfig["generator"]));
#else
        SetUserAction(new ArtieIPrimaryGeneratorAction());
#endif 
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ArtieIActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}