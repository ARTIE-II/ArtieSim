/**
 * @file ArtieIIDetectorConstruction.hh
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
    class ArtieIIDetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        ArtieIIDetectorConstruction();
        ~ArtieIIDetectorConstruction();

#ifdef ARTIE_YAML
        ArtieIIDetectorConstruction(YAML::Node config);
#endif

        void DefineMaterials();

        virtual G4VPhysicalVolume* Construct();
    
    private:
        virtual void ConstructSDandField();

    private:
        G4String mWorldMaterialName = {"high_vacuum"};
        G4double mExperimentalHallX = {500 * m};
        G4double mExperimentalHallY = {500 * m};
        G4double mExperimentalHallZ = {500 * m};

        // main target volume
        G4String mActiveVolumeMaterialName = {"liquid_argon"};
        G4double mTargetRadius = {2.208 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};

        // stainless steel tube
        G4bool mConstructContainer = {true};
        G4String mContainerMaterialName = {"stainless_steel"};
        G4double mContainerRadius = {2.54 / 2.0 * cm};

        // left side inner flange
        G4bool mConstructInnerFlangeLeftSide = {true};
        G4String mInnerFlangeLeftSideMaterialName = {"aluminum"};
        G4double mInnerFlangeLeftSideRadius = {4.8133 / 2.0 * cm};
        G4double mInnerFlangeLeftSideThickness = {0.20066};

        // right side inner flange
        G4bool mConstructInnerFlangeRightSide = {true};
        G4String mInnerFlangeRightSideMaterialName = {"aluminum"};
        G4double mInnerFlangeRightSideRadius = {4.8133 / 2.0 * cm};
        G4double mInnerFlangeRightSideThickness = {0.20066};

        // inner flange gap
        G4bool mConstructInnerFlangeLeftSideGap = {true};
        G4bool mConstructInnerFlangeRightSideGap = {true};
        G4String mInnerFlangeLeftSideGapMaterialName = {"high_vacuum"};
        G4String mInnerFlangeRightSideGapMaterialName = {"high_vacuum"};
        G4double mInnerFlangeLeftSideGapThickness = {1.87706};
        G4double mInnerFlangeRightSideGapThickness = {1.87706};

        // left side Outer flange
        G4bool mConstructOuterFlangeLeftSide = {true};
        G4String mOuterFlangeLeftSideMaterialName = {"aluminum"};
        G4double mOuterFlangeLeftSideRadius = {17.10436 / 2.0 * cm};
        G4double mOuterFlangeLeftSideThickness = {0.20066};

        // right side Outer flange
        G4bool mConstructOuterFlangeRightSide = {true};
        G4String mOuterFlangeRightSideMaterialName = {"aluminum"};
        G4double mOuterFlangeRightSideRadius = {17.10436 / 2.0 * cm};
        G4double mOuterFlangeRightSideThickness = {0.20066};

        // Detector
        G4bool mConstructDetector = {true};
        G4String mDetectorMaterialName = {"water"};
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {30.0 * m};

        G4double mWorldX = {4 * m};
        G4double mWorldY = {4 * m};
        G4double mWorldZ = {200 * m};

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

        // Container
        G4Material* mContainerMaterial;
        G4Tubs *mSolidContainer;
        G4LogicalVolume* mLogicalContainer;
        G4VPhysicalVolume* mPhysicalContainer;

        // Inner flange left side
        G4Material* mInnerFlangeLeftSideMaterial;
        G4Tubs *mSolidInnerFlangeLeftSide;
        G4LogicalVolume* mLogicalInnerFlangeLeftSide;
        G4VPhysicalVolume* mPhysicalInnerFlangeLeftSide;

        // Inner flange right side
        G4Material* mInnerFlangeRightSideMaterial;
        G4Tubs *mSolidInnerFlangeRightSide;
        G4LogicalVolume* mLogicalInnerFlangeRightSide;
        G4VPhysicalVolume* mPhysicalInnerFlangeRightSide;

        // Inner flange left side gap
        G4Material* mInnerFlangeLeftSideGapMaterial;
        G4Tubs *mSolidInnerFlangeLeftSideGap;
        G4LogicalVolume* mLogicalInnerFlangeLeftSideGap;
        G4VPhysicalVolume* mPhysicalInnerFlangeLeftSideGap;

        // Inner flange right side gap
        G4Material* mInnerFlangeRightSideGapMaterial;
        G4Tubs *mSolidInnerFlangeRightSideGap;
        G4LogicalVolume* mLogicalInnerFlangeRightSideGap;
        G4VPhysicalVolume* mPhysicalInnerFlangeRightSideGap;

        // Outer flange left side
        G4Material* mOuterFlangeLeftSideMaterial;
        G4Tubs *mSolidOuterFlangeLeftSide;
        G4LogicalVolume* mLogicalOuterFlangeLeftSide;
        G4VPhysicalVolume* mPhysicalOuterFlangeLeftSide;

        // Outer flange right side
        G4Material* mOuterFlangeRightSideMaterial;
        G4Tubs *mSolidOuterFlangeRightSide;
        G4LogicalVolume* mLogicalOuterFlangeRightSide;
        G4VPhysicalVolume* mPhysicalOuterFlangeRightSide;

        // Detector
        G4Material* mDetectorMaterial;
        G4Tubs *mSolidDetector;
        G4LogicalVolume* mLogicalDetector;
        G4VPhysicalVolume* mPhysicalDetector;

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}