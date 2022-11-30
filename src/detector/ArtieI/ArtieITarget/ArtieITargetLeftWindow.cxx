/**
 * @file ArtieITargetLeftWindow.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetLeftWindow.hh"

namespace Artie
{
    ArtieITargetLeftWindow::ArtieITargetLeftWindow(
        G4double TargetLength,
        G4double ContainerRadius,
        G4double WindowThickness
    )
    : DetectorComponent("ArtieITargetLeftWindow", false)
    , mTargetLength(TargetLength)
    , mContainerRadius(ContainerRadius)
    , mWindowThickness(WindowThickness)
    {
    }

    void ArtieITargetLeftWindow::Construct()
    {
        /// create the kapton
        mKapton.reset(
            new Kapton("Kapton_" + GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetLeftWindow", 
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
                mKapton->GetMaterial().get(), 
                "Logical_ArtieITargetLeftWindow"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    -(mTargetLength + mWindowThickness) * 0.5),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetLeftWindow", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetLeftWindow::~ArtieITargetLeftWindow()
    {
    }
}