/**
 * @file ArtieITargetContainer.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetContainer.hh"

namespace Artie
{
    ArtieITargetContainer::ArtieITargetContainer(
        G4double TargetRadius,
        G4double TargetLength,
        G4double ContainerRadius
    )
    : DetectorComponent("ArtieITargetContainer", false)
    , mTargetRadius(TargetRadius)
    , mTargetLength(TargetLength)
    , mContainerRadius(ContainerRadius)
    {
    }

    void ArtieITargetContainer::Construct()
    {
        /// create the StainlessSteel
        mStainlessSteel.reset(
            new StainlessSteel("StainlessSteel_" + GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetContainer", 
                mTargetRadius,
                mContainerRadius, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mStainlessSteel->GetMaterial().get(), 
                "Logical_ArtieITargetContainer"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0.,0.,0.),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetContainer", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetContainer::~ArtieITargetContainer()
    {
    }
}