/**
 * @file DetectorConstruction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4Cache.hh"
#include "G4AutoDelete.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"

#include "Detector.hh"
#include "SensitiveDetector.hh"
#include "EventManager.hh"

namespace Artie
{
    class DetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        DetectorConstruction(
            G4double ExperimentalHallX = 500 * m,
            G4double ExperimentalHallY = 500 * m,
            G4double ExperimentalHallZ = 500 * m,
            Detector* detector = nullptr
        );
        ~DetectorConstruction();

        void DefineMaterials();

        void SetDetector(Detector*);

        virtual G4VPhysicalVolume* Construct();
    
    private:
        virtual void ConstructSDandField();

    private:
        G4double mExperimentalHallX = {500 * m};
        G4double mExperimentalHallY = {500 * m};
        G4double mExperimentalHallZ = {500 * m};

        std::shared_ptr<G4Box> mSolidExperimentalHall = {nullptr};
        std::shared_ptr<G4LogicalVolume> mLogicalExperimentalHall = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalExperimentalHall = {nullptr};

        std::shared_ptr<G4Material> mExperimentalHallMaterial = {nullptr};

        std::shared_ptr<Detector> mDetector = {nullptr};

    };
}