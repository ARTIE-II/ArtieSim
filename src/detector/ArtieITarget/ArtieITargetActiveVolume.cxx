/**
 * @file ArtieITargetActiveVolume.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetActiveVolume.hh"

namespace Artie
{
    ArtieITargetActiveVolume::ArtieITargetActiveVolume(
        G4double TargetR, 
        G4double TargetL
    )
    : DetectorComponent("ArtieITargetActiveVolume", false)
    , mTargetR(TargetR)
    , mTargetL(TargetL)
    {
    }

    void ArtieITargetActiveVolume::Construct()
    {
        /// create the argon object
        mArgon.reset(
            new Argon(
                83.0 * kelvin,      /// temperature
                0.952 * atmosphere, /// pressure
                0.334,              /// ratio of Ar36
                0.063,              /// ratio of Ar38
                99.603              /// ratio of Ar40
            )
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetActiveVolume", 
                0,
                mTargetR, 
                mTargetL, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mArgon->GetLAr().get(), 
                "Logical_ArtieITargetActiveVolume"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetActiveVolume", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetActiveVolume::~ArtieITargetActiveVolume()
    {
    }
}