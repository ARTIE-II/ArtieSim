/**
 * @file ArtieIHall.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIHall.hh"

namespace Artie
{
    ArtieIHall::ArtieIHall(
        G4double WallOuterX,
        G4double WallOuterY,
        G4double WallOuterZ,
        G4double WallThickness
    )
    : DetectorComponent("ArtieIHall", false)
    , mWallOuterX(WallOuterX)
    , mWallOuterY(WallOuterY)
    , mWallOuterZ(WallOuterZ)
    , mWallThickness(WallThickness)
    {
    }

    void ArtieIHall::Construct()
    {
        /// create the argon object
        mConcrete.reset(
            new Concrete("Concrete_" + GetName())
        );

        // create the argon Cube volume
        G4Box* OuterWall = new G4Box(
            "Solid_ArtieIHall_OuterWall",
            mWallOuterX / 2.0,
            mWallOuterY / 2.0,
            mWallOuterZ / 2.0
        );
        G4Box* InnerWall = new G4Box(
            "Solid_ArtieIHall_InnerWall",
            mWallOuterX / 2.0 - mWallThickness,
            mWallOuterY / 2.0 - mWallThickness,
            mWallOuterZ / 2.0 - mWallThickness
        );
        SetSolidVolume(
            new G4SubtractionSolid(
                "Solid_ArtieIHall", 
                OuterWall,
                InnerWall
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mConcrete->GetMaterial().get(), 
                "Logical_ArtieIHall"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(), 
                GetLogicalVolumePointer(), 
                "Physical_ArtieIHall", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieIHall::~ArtieIHall()
    {
    }
}