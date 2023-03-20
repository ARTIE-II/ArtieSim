/**
 * @file ArtieIInTOFActionInitialization.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#pragma once
#include "G4VUserActionInitialization.hh"

#include "ArtieIInTOFPrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "Core.hh"

namespace Artie
{
    class ArtieIInTOFActionInitialization : public G4VUserActionInitialization
    {
    public:
        ArtieIInTOFActionInitialization();
        ~ArtieIInTOFActionInitialization();

#ifdef ARTIE_YAML
        ArtieIInTOFActionInitialization(YAML::Node config);
#endif

        virtual void Build() const;
        virtual void BuildForMaster() const;

    private:
#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}