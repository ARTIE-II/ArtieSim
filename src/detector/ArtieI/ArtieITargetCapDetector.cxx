/**
 * @file ArtieITargetCapDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetCapDetector.hh"

namespace Artie
{
    ArtieITargetCapDetector::ArtieITargetCapDetector(
        G4double CapR, 
        G4double CapL, 
        G4ThreeVector CapPosition
    )
    : DetectorComponent("ArtieITargetCapDetector", true)
    , mCapR(CapR)
    , mCapL(CapL)
    , mCapPosition(CapPosition)
    {
        G4Material* aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
        mMaterial.reset(aluminum);
    }

    void ArtieITargetCapDetector::Construct()
    {
        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_Cap", 
                0,
                mCapR, 
                mCapL, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial.get(), 
                "Logical_Cap"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mCapPosition, 
                GetLogicalVolumePointer(), 
                "Physical_Cap", 
                GetMotherLogical(), 
                false, 
                1, 
                true
            )
        );
    }

    ArtieITargetCapDetector::~ArtieITargetCapDetector()
    {
    }

    G4bool ArtieITargetCapDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
    {
        G4cout << "HIT" << G4endl;
        return true;
    }
}