/**
 * @file ArtieITargetInsulation.hh
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

#include "Polyurethane.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetInsulation : public DetectorComponent
    {
    public:
        ArtieITargetInsulation(
            G4double TargetLength,
            G4double ContainerRadius,
            G4double InsulationThickness
        );
        ~ArtieITargetInsulation();

        void Construct();

    private:
        G4double mTargetLength = {168 * cm};
        G4double mContainerRadius = {3.49 / 2.0 * cm};
        G4double mInsulationThickness = {10.0 * cm};

        std::shared_ptr<Material> mMaterial = {nullptr};

    };
}