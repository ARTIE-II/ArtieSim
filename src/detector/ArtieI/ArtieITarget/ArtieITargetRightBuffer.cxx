/**
 * @file ArtieITargetRightBuffer.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetRightBuffer.hh"

namespace Artie
{
    ArtieITargetRightBuffer::ArtieITargetRightBuffer(
        G4double TargetRadius,
        G4double TargetLength,
        G4double WindowThickness,
        G4double BufferLength
    )
    : DetectorComponent("ArtieITargetRightBuffer", false)
    , mTargetRadius(TargetRadius)
    , mTargetLength(TargetLength)
    , mWindowThickness(WindowThickness)
    , mBufferLength(BufferLength)
    {
    }

    void ArtieITargetRightBuffer::Construct()
    {
        /// create the argon object
        mMaterial.reset(
            CreateMaterial("gaseous_argon", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetRightBuffer", 
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
                "Logical_ArtieITargetRightBuffer"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    +(mTargetLength + 2 * mWindowThickness + mBufferLength) * 0.5),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetRightBuffer", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetRightBuffer::~ArtieITargetRightBuffer()
    {
    }
}