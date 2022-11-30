/**
 * @file ArtieITargetLeftWindow.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Kapton.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetLeftWindow : public DetectorComponent
    {
    public:
        ArtieITargetLeftWindow(
            G4double TargetLength,
            G4double ContainerRadius,
            G4double WindowThickness
        );
        ~ArtieITargetLeftWindow();

        void Construct();

    private:
        G4double mTargetLength = {168 * cm};
        G4double mContainerRadius = {3.49 / 2.0 * cm};
        G4double mWindowThickness = {0.00762 * cm};

        std::shared_ptr<Kapton> mKapton = {nullptr};

    };
}