/**
 * @file ArtieITargetDetector.hh
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

#include "Water.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetDetector : public DetectorComponent
    {
    public:
        ArtieITargetDetector(
            G4double DetectorRadius,
            G4double DetectorLength,
            G4double DetectorEntrance
        );
        ~ArtieITargetDetector();

        void Construct();

    private:
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {69.0 * m};

        std::shared_ptr<Material> mMaterial = {nullptr};

    };
}