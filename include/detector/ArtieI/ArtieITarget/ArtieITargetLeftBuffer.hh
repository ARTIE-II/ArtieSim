/**
 * @file ArtieITargetLeftBuffer.hh
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

#include "Argon.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetLeftBuffer : public DetectorComponent
    {
    public:
        ArtieITargetLeftBuffer(
            G4double TargetRadius,
            G4double TargetLength,
            G4double WindowThickness,
            G4double BufferLength
        );
        ~ArtieITargetLeftBuffer();

        void Construct();

    private:
        G4double mTargetRadius = {2.5 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};
        G4double mWindowThickness = {0.00762 * cm};
        G4double mBufferLength = {5.0 * cm};

        std::shared_ptr<Material> mMaterial = {nullptr};

    };
}