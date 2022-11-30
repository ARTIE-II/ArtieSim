/**
 * @file ArtieITargetCapDetector.hh
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
#include "G4PhysicsOrderedFreeVector.hh"

#include "Argon.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieITargetCapDetector : public DetectorComponent
    {
    public:
        ArtieITargetCapDetector(
            G4double CapR,
            G4double CapL,
            G4ThreeVector CapPosition
        );
        ~ArtieITargetCapDetector();

        void Construct();

        G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    private:
        G4double mCapR = {25 * mm};
        G4double mCapL = {50 * mm};
        G4ThreeVector mCapPosition = {0.,0.,125.*mm};

        std::shared_ptr<G4Material> mMaterial;

    };
}