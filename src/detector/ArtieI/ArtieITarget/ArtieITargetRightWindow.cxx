/**
 * @file ArtieITargetRightWindow.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetRightWindow.hh"

namespace Artie
{
    ArtieITargetRightWindow::ArtieITargetRightWindow(
        G4double TargetLength,
        G4double ContainerRadius,
        G4double WindowThickness
    )
    : DetectorComponent("ArtieITargetRightWindow", false)
    , mTargetLength(TargetLength)
    , mContainerRadius(ContainerRadius)
    , mWindowThickness(WindowThickness)
    {
    }

    void ArtieITargetRightWindow::Construct()
    {
        /// create the kapton
        mMaterial.reset(
            CreateMaterial("kapton", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetRightWindow", 
                0,
                mContainerRadius, 
                0.5 * mWindowThickness, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieITargetRightWindow"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    +(mTargetLength + mWindowThickness) * 0.5),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetRightWindow", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetRightWindow::~ArtieITargetRightWindow()
    {
    }
}