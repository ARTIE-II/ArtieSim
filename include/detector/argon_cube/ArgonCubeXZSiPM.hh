/**
 * @file ArgonCubeXZSiPM.hh
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
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include "Argon.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArgonCubeXZSiPM : public DetectorComponent
    {
    public:
        ArgonCubeXZSiPM(
            G4int SiPMID,
            G4double CubeX,
            G4double CubeY,
            G4double CubeZ,
            G4ThreeVector Position
        );
        ~ArgonCubeXZSiPM();

        void Construct();

        G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    private:
        G4int mSiPMID = {0};
        G4double mCubeX = {5 * mm};
        G4double mCubeY = {5 * mm};
        G4double mCubeZ = {5 * mm};
        G4ThreeVector mPosition = {0.,0.,0.};

        std::shared_ptr<G4Material> mMaterial;
        G4PhysicsOrderedFreeVector* mQuantumEfficiency;

    };
}