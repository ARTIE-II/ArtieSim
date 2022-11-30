/**
 * @file ArtieIBeamPipeLeftBeam.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIBeamPipeLeftBeam.hh"

namespace Artie
{
    ArtieIBeamPipeLeftBeam::ArtieIBeamPipeLeftBeam(
        G4double BeamPipeInnerRadius,
        G4double BeamPipeLeftHalfLength,
        G4ThreeVector BeamPipeLeftPosition
    )
    : DetectorComponent("ArtieIBeamPipeLeftBeam", false)
    , mBeamPipeInnerRadius(BeamPipeInnerRadius)
    , mBeamPipeLeftHalfLength(BeamPipeLeftHalfLength)
    , mBeamPipeLeftPosition(BeamPipeLeftPosition)
    {
    }

    void ArtieIBeamPipeLeftBeam::Construct()
    {
        /// create the argon object
        mVacuum.reset(
            new Vacuum("Vacuum_" + GetName(), 1.0E-11)
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieIBeamPipeLeftBeam", 
                0,
                mBeamPipeInnerRadius, 
                mBeamPipeLeftHalfLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mVacuum->GetMaterial().get(), 
                "Logical_ArtieIBeamPipeLeftBeam"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mBeamPipeLeftPosition, 
                GetLogicalVolumePointer(), 
                "Physical_ArtieIBeamPipeLeftBeam", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieIBeamPipeLeftBeam::~ArtieIBeamPipeLeftBeam()
    {
    }
}