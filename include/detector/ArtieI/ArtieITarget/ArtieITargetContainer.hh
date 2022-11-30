/**
 * @file ArtieITargetContainer.hh
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

#include "StainlessSteel.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetContainer : public DetectorComponent
    {
    public:
        ArtieITargetContainer(
            G4double TargetRadius,
            G4double TargetLength,
            G4double ContainerRadius
        );
        ~ArtieITargetContainer();

        void Construct();

    private:
        G4double mTargetRadius = {2.5 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};
        G4double mContainerRadius = {3.49 / 2.0 * cm};

        std::shared_ptr<StainlessSteel> mStainlessSteel = {nullptr};

    };
}