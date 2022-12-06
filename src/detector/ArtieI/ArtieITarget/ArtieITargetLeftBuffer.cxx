/**
 * @file ArtieITargetLeftBuffer.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetLeftBuffer.hh"

namespace Artie
{
    ArtieITargetLeftBuffer::ArtieITargetLeftBuffer(
        G4double TargetRadius,
        G4double TargetLength,
        G4double WindowThickness,
        G4double BufferLength
    )
    : DetectorComponent("ArtieITargetLeftBuffer", false)
    , mTargetRadius(TargetRadius)
    , mTargetLength(TargetLength)
    , mWindowThickness(WindowThickness)
    , mBufferLength(BufferLength)
    {
    }

    void ArtieITargetLeftBuffer::Construct()
    {
        /// create the argon object
        mMaterial.reset(
             CreateMaterial("gaseous_argon", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetLeftBuffer", 
                0,
                mTargetRadius, 
                0.5 * mBufferLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieITargetLeftBuffer"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    -(mTargetLength + 2 * mWindowThickness + mBufferLength) * 0.5),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetLeftBuffer", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetLeftBuffer::~ArtieITargetLeftBuffer()
    {
    }
}