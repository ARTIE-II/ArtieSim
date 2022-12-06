/**
 * @file ArtieIBeamPipeLeftPipe.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIBeamPipeLeftPipe.hh"

namespace Artie
{
    ArtieIBeamPipeLeftPipe::ArtieIBeamPipeLeftPipe(
        G4double BeamPipeInnerRadius,
        G4double BeamPipeOuterRadius,
        G4double BeamPipeLeftHalfLength,
        G4ThreeVector BeamPipeLeftPosition
    )
    : DetectorComponent("ArtieIBeamPipeLeftPipe", false)
    , mBeamPipeInnerRadius(BeamPipeInnerRadius)
    , mBeamPipeOuterRadius(BeamPipeOuterRadius)
    , mBeamPipeLeftHalfLength(BeamPipeLeftHalfLength)
    , mBeamPipeLeftPosition(BeamPipeLeftPosition)
    {
    }

    void ArtieIBeamPipeLeftPipe::Construct()
    {
        /// create the argon object
        mMaterial.reset(
            CreateMaterial("rough_vacuum", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieIBeamPipeLeftPipe", 
                mBeamPipeInnerRadius,
                mBeamPipeOuterRadius, 
                mBeamPipeLeftHalfLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieIBeamPipeLeftPipe"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mBeamPipeLeftPosition, 
                GetLogicalVolumePointer(), 
                "Physical_ArtieIBeamPipeLeftPipe", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieIBeamPipeLeftPipe::~ArtieIBeamPipeLeftPipe()
    {
    }
}