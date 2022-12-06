/**
 * @file ArtieIBeamPipeRightPipe.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIBeamPipeRightPipe.hh"

namespace Artie
{
    ArtieIBeamPipeRightPipe::ArtieIBeamPipeRightPipe(
        G4double BeamPipeInnerRadius,
        G4double BeamPipeOuterRadius,
        G4double BeamPipeRightHalfLength,
        G4ThreeVector BeamPipeRightPosition
    )
    : DetectorComponent("ArtieIBeamPipeRightPipe", false)
    , mBeamPipeInnerRadius(BeamPipeInnerRadius)
    , mBeamPipeOuterRadius(BeamPipeOuterRadius)
    , mBeamPipeRightHalfLength(BeamPipeRightHalfLength)
    , mBeamPipeRightPosition(BeamPipeRightPosition)
    {
    }

    void ArtieIBeamPipeRightPipe::Construct()
    {
        /// create the argon object
        mMaterial.reset(
             CreateMaterial("rough_vacuum", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieIBeamPipeRightPipe", 
                mBeamPipeInnerRadius,
                mBeamPipeOuterRadius, 
                mBeamPipeRightHalfLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieIBeamPipeRightPipe"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mBeamPipeRightPosition, 
                GetLogicalVolumePointer(), 
                "Physical_ArtieIBeamPipeRightPipe", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieIBeamPipeRightPipe::~ArtieIBeamPipeRightPipe()
    {
    }
}