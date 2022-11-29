/**
 * @file ArtieITargetActiveVolume.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Argon.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetActiveVolume : public DetectorComponent
    {
    public:
        ArtieITargetActiveVolume(
            G4double TargetR,
            G4double TargetL
        );
        ~ArtieITargetActiveVolume();

        void Construct();

    private:
        G4double mTargetR = {.025 * m};
        G4double mTargetL = {1.68 * m};

        std::shared_ptr<Argon> mArgon = {nullptr};

    };
}