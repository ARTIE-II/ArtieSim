/**
 * @file ArtieIBeamPipeLeftPipe.hh
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

#include "Material.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieIBeamPipeLeftPipe : public DetectorComponent
    {
    public:
        ArtieIBeamPipeLeftPipe(
            G4double BeamPipeInnerRadius,
            G4double BeamPipeOuterRadius,
            G4double BeamPipeLeftHalfLength,
            G4ThreeVector BeamPipeRightPosition
        );
        ~ArtieIBeamPipeLeftPipe();

        void Construct();

    private:
        G4double mBeamPipeInnerRadius = {18.0 * cm};
        G4double mBeamPipeOuterRadius = {20.0 * cm};
        G4double mBeamPipeLeftHalfLength = {1.68 * m};
        G4ThreeVector mBeamPipeLeftPosition = {0.,0.,0.};

        std::shared_ptr<Material> mMaterial = {nullptr};

    };
}