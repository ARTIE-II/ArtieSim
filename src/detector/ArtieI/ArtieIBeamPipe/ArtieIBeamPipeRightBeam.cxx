/**
 * @file ArtieIBeamPipeRightBeam.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIBeamPipeRightBeam.hh"

namespace Artie
{
    ArtieIBeamPipeRightBeam::ArtieIBeamPipeRightBeam(
        G4double BeamPipeInnerRadius,
        G4double BeamPipeRightHalfLength,
        G4ThreeVector BeamPipeRightPosition
    )
    : DetectorComponent("ArtieIBeamPipeRightBeam", false)
    , mBeamPipeInnerRadius(BeamPipeInnerRadius)
    , mBeamPipeRightHalfLength(BeamPipeRightHalfLength)
    , mBeamPipeRightPosition(BeamPipeRightPosition)
    {
    }

    void ArtieIBeamPipeRightBeam::Construct()
    {
        /// create the argon object
        mMaterial.reset(
            CreateMaterial("high_vacuum", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieIBeamPipeRightBeam", 
                0,
                mBeamPipeInnerRadius, 
                mBeamPipeRightHalfLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieIBeamPipeRightBeam"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mBeamPipeRightPosition, 
                GetLogicalVolumePointer(), 
                "Physical_ArtieIBeamPipeRightBeam", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieIBeamPipeRightBeam::~ArtieIBeamPipeRightBeam()
    {
    }
}