/**
 * @file ArtieITargetDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetDetector.hh"
#include "ArtieITargetCapDetector.hh"

namespace Artie
{
    ArtieITargetDetector::ArtieITargetDetector(
        G4double DetectorRadius,
        G4double DetectorLength,
        G4double DetectorEntrance
    )
    : DetectorComponent("ArtieITargetDetector", true)
    , mDetectorRadius(DetectorRadius)
    , mDetectorLength(DetectorLength)
    , mDetectorEntrance(DetectorEntrance)
    {
    }

    void ArtieITargetDetector::Construct()
    {
        /// create the Water
        mWater.reset(
            new Water("Water_" + GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mWater->GetMaterial().get(), 
                "Logical_ArtieITargetDetector"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    + mDetectorEntrance + mDetectorLength * 0.5),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetDetector", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetDetector::~ArtieITargetDetector()
    {
    }
}