/**
 * @file ArtieIInTOFActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ArtieIInTOFActionInitialization.hh"

namespace Artie
{
    ArtieIInTOFActionInitialization::ArtieIInTOFActionInitialization()
    {
    }

    ArtieIInTOFActionInitialization::~ArtieIInTOFActionInitialization()
    {
    }

#ifdef ARTIE_YAML
    ArtieIInTOFActionInitialization::ArtieIInTOFActionInitialization(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    void ArtieIInTOFActionInitialization::Build() const
    {
#ifdef ARTIE_YAML
        SetUserAction(new ArtieIInTOFPrimaryGeneratorAction(mConfig));
#else
        SetUserAction(new ArtieIInTOFPrimaryGeneratorAction());
#endif 
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ArtieIInTOFActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}