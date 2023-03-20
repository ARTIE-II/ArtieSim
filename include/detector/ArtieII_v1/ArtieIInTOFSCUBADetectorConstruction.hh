/**
 * @file ArtieIInTOFSCUBADetectorConstruction.hh
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
#include "G4SubtractionSolid.hh"
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
#include "G4GDMLParser.hh"

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Material.hh"
#include "SensitiveDetector.hh"

namespace Artie
{
    class ArtieIInTOFSCUBADetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        ArtieIInTOFSCUBADetectorConstruction();
        ~ArtieIInTOFSCUBADetectorConstruction();

#ifdef ARTIE_YAML
        ArtieIInTOFSCUBADetectorConstruction(YAML::Node config);
#endif

        void DefineMaterials();

        virtual G4VPhysicalVolume* Construct();
    
    private:
        virtual void ConstructSDandField();

    private:
        G4String mWorldMaterialName = {"high_vacuum"};
        G4double mExperimentalHallX = {75 * m};
        G4double mExperimentalHallY = {75 * m};
        G4double mExperimentalHallZ = {75 * m};

        // main target volume
        G4double mTargetEntrance = {188 * m};
        G4double mTargetX = {0.0 * cm};
        G4double mTargetY = {0.0 * cm};
        G4String mActiveVolumeMaterialName = {"liquid_argon"};
        G4double mTargetRadius = {10.00 * cm};
        G4double mTargetLength = {66.675 * cm};

        // outer container mother volume
        G4bool mConstructSCUBATank = {true};
        G4String mSCUBATankMaterialName = {"aluminum"};
        G4double mSCUBATankRadius = {10.16 * cm};
        G4double mSCUBATankLength = {66.675 * cm};

        // Detector
        G4bool mConstructDetector = {true};
        G4String mDetectorMaterialName = {"water"};
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {200.0};

        //Filters
        G4bool mConstructFilters = {true};
        std::vector<std::string> mFilterMaterialNames = {};
        std::vector<G4double> mFilterZPos = {};
        std::vector<G4double> mFilterThickness = {};
        std::vector<G4double> mFilterRadius = {};

        G4double mWorldX = {4 * m};
        G4double mWorldY = {4 * m};
        G4double mWorldZ = {200 * m};

        G4double mTZeroLocation = {-188.0 * m};

        // Experimental Hall
        G4Material *mWorldMaterial;
        G4Box* mSolidExperimentalHall;
        G4LogicalVolume* mLogicalExperimentalHall;
        G4VPhysicalVolume* mPhysicalExperimentalHall;

        // Active Volume
        G4Material *mActiveVolumeMaterial;
        G4Tubs *mSolidActiveVolume;
        G4LogicalVolume* mLogicalActiveVolume;
        G4VPhysicalVolume* mPhysicalActiveVolume;

        // Active Volume Container
        G4Material* mSCUBATankMaterial;
        G4Tubs *mSolidSCUBATank;
        G4LogicalVolume* mLogicalSCUBATank;
        G4VPhysicalVolume* mPhysicalSCUBATank;

        // Detector
        G4Material* mDetectorMaterial;
        G4Tubs *mSolidDetector;
        G4LogicalVolume* mLogicalDetector;
        G4VPhysicalVolume* mPhysicalDetector;

        // Filters
        std::vector<G4Material*> mFilterMaterials = {};
        std::vector<G4Tubs*> mSolidFilters = {};
        std::vector<G4LogicalVolume*> mLogicalFilters = {};
        std::vector<G4VPhysicalVolume*> mPhysicalFilters = {};

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}