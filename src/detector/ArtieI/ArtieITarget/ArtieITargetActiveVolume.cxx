/**
 * @file ArtieITargetActiveVolume.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetActiveVolume.hh"

namespace Artie
{
    ArtieITargetActiveVolume::ArtieITargetActiveVolume(
        G4String ActiveVolumeMaterial,
        G4double TargetRadius, 
        G4double TargetLength
    )
    : DetectorComponent("ArtieITargetActiveVolume", false)
    , mActiveVolumeMaterial(ActiveVolumeMaterial)
    , mTargetRadius(TargetRadius)
    , mTargetLength(TargetLength)
    {
    }

    void ArtieITargetActiveVolume::Construct()
    {
        /// create the argon object
        mMaterial.reset(
            CreateMaterial(mActiveVolumeMaterial, GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetActiveVolume", 
                0,
                mTargetRadius, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieITargetActiveVolume"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetActiveVolume", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetActiveVolume::~ArtieITargetActiveVolume()
    {
    }
}