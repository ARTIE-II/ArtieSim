/**
 * @file ArtieITargetDetector.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <vector>
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "ArtieITargetActiveVolume.hh"

namespace Artie
{
    class ArtieITargetDetector : public Detector
    {
    public:
        ArtieITargetDetector(
            G4double TargetR,
            G4double TargetL,
            G4double CapR,
            G4double CapL,
            G4ThreeVector CapPosition
        );
        ~ArtieITargetDetector();

    private:
        G4double mTargetR = {.025 * m};
        G4double mTargetL = {1.68 * m};
        G4double mCapR = {.025 * m};
        G4double mCapL = {.050 * m};
        G4ThreeVector mCapPosition = {0.,0.,(1.68/2 + 0.025) * m};
    };
}