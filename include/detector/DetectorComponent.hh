/**
 * @file DetectorComponent.hh
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
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4StepPoint.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "Randomize.hh"

#include "MaterialFactory.hh"

namespace Artie
{
    class DetectorComponent
    {
    public:
        DetectorComponent(
            G4String name, 
            G4bool isSensitive = false,
            G4bool isRadioactive = false
        );
        ~DetectorComponent();

        virtual void Construct() = 0;
        virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*){ return false; }

        void SetName(G4String name);
        void SetSensitive(G4bool isSensitive);
        void SetElectricField(G4bool isElectricField);
        void SetRadioactive(G4bool isRadioactive);

        void SetElectricFieldVector(G4ThreeVector);

        void SetSolidVolume(G4VSolid* solid);
        void SetLogicalVolume(G4LogicalVolume* logical);
        void SetPhysicalVolume(G4VPhysicalVolume* physical);

        void SetMotherLogical(G4LogicalVolume* mother) { mMotherLogicalVolume.reset(mother); }
        G4LogicalVolume* GetMotherLogical() { return mMotherLogicalVolume.get(); }

        G4String GetName() { return mName; }
        G4bool GetSensitive() { return mIsSensitive; }
        G4bool GetElectricField() { return mIsElectricField; }

        G4ThreeVector GetElectricFieldVector() { return mElectricFieldVector; }

        std::shared_ptr<G4VSolid> GetSolidVolume() { return mSolidVolume; }
        std::shared_ptr<G4LogicalVolume> GetLogicalVolume() { return mLogicalVolume; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalVolume() { return mPhysicalVolume; }

        G4VSolid* GetSolidVolumePointer() { return mSolidVolume.get(); }
        G4LogicalVolume* GetLogicalVolumePointer() { return mLogicalVolume.get(); }
        G4VPhysicalVolume* GetPhysicalVolumePointer() { return mPhysicalVolume.get(); }

        G4String GetPhysicalVolumeName() { return mPhysicalVolume->GetName(); }


    private:
        G4String mName = {"default"};
        G4bool mIsSensitive = {false};
        G4bool mIsRadioactive = {false};
        G4bool mIsElectricField = {false};

        G4ThreeVector mElectricFieldVector = {0.0,0.0,0.0};

        std::shared_ptr<G4VSolid> mSolidVolume = {nullptr};
        std::shared_ptr<G4LogicalVolume> mLogicalVolume = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalVolume = {nullptr};

        std::shared_ptr<G4LogicalVolume> mMotherLogicalVolume = {nullptr};

    };
}