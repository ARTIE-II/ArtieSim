/**
 * @file DetectorComponent.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "DetectorComponent.hh"

namespace Artie
{
    DetectorComponent::DetectorComponent(
        G4String name, 
        G4bool isSensitive,
        G4bool isRadioactive
    )
    : mName(name)
    , mIsSensitive(isSensitive)
    , mIsRadioactive(isRadioactive)
    {
    }

    DetectorComponent::~DetectorComponent()
    {
    }

    void DetectorComponent::SetName(G4String name)
    {
        mName = name;
    }
    void DetectorComponent::SetSensitive(G4bool isSensitive)
    {
        mIsSensitive = isSensitive;
    }
    void DetectorComponent::SetElectricField(G4bool isElectricField)
    {
        mIsElectricField = isElectricField;
    }
    void DetectorComponent::SetRadioactive(G4bool isRadioactive)
    {
        mIsRadioactive = isRadioactive;
    }

    void DetectorComponent::SetElectricFieldVector(G4ThreeVector electricFieldVector)
    {
        mElectricFieldVector = electricFieldVector;
    }
    
    void DetectorComponent::SetSolidVolume(G4VSolid* solid)
    {
        mSolidVolume.reset(solid);
    }

    void DetectorComponent::SetLogicalVolume(G4LogicalVolume* logical)
    {
        mLogicalVolume.reset(logical);
    }

    void DetectorComponent::SetPhysicalVolume(G4VPhysicalVolume* physical)
    {
        mPhysicalVolume.reset(physical);
    }
}