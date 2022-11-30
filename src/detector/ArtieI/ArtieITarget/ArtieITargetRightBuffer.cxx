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
        mArgon.reset(
            new Argon(
                "LAr_" + GetName(), 
                83.0 * kelvin,      /// temperature
                0.952 * atmosphere, /// pressure
                kStateGas,          /// gas
                0.334,              /// ratio of Ar36
                0.063,              /// ratio of Ar38
                99.603              /// ratio of Ar40
            )
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
                mArgon->GetMaterial().get(), 
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