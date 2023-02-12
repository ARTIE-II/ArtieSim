/**
 * @file ArtieIIDICERActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ArtieIIDICERActionInitialization.hh"

namespace Artie
{
    ArtieIIDICERActionInitialization::ArtieIIDICERActionInitialization()
    {
    }

    ArtieIIDICERActionInitialization::~ArtieIIDICERActionInitialization()
    {
    }

#ifdef ARTIE_YAML
    ArtieIIDICERActionInitialization::ArtieIIDICERActionInitialization(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    void ArtieIIDICERActionInitialization::Build() const
    {
#ifdef ARTIE_YAML
        SetUserAction(new ArtieIIDICERPrimaryGeneratorAction(mConfig));
#else
        SetUserAction(new ArtieIIDICERPrimaryGeneratorAction());
#endif 
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ArtieIIDICERActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}