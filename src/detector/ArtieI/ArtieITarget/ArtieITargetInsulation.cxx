/**
 * @file ArtieITargetInsulation.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetInsulation.hh"

namespace Artie
{
    ArtieITargetInsulation::ArtieITargetInsulation(
        G4double TargetLength,
        G4double ContainerRadius,
        G4double InsulationThickness
    )
    : DetectorComponent("ArtieITargetInsulation", false)
    , mTargetLength(TargetLength)
    , mContainerRadius(ContainerRadius)
    , mInsulationThickness(InsulationThickness)
    {
    }

    void ArtieITargetInsulation::Construct()
    {
        /// create the polyurethane
        mMaterial.reset(
             CreateMaterial("polyurethane", GetName())
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Tubs(
                "Solid_ArtieITargetInsulation", 
                mContainerRadius,
                mContainerRadius + mInsulationThickness, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial->GetMaterial().get(), 
                "Logical_ArtieITargetInsulation"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0.,0.,0.),
                GetLogicalVolumePointer(), 
                "Physical_ArtieITargetInsulation", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArtieITargetInsulation::~ArtieITargetInsulation()
    {
    }
}