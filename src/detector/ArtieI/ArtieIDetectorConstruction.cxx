/**
 * @file ArtieIDetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIDetectorConstruction.hh"

namespace Artie
{
    ArtieIDetectorConstruction::ArtieIDetectorConstruction()
    : G4VUserDetectorConstruction()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    ArtieIDetectorConstruction::ArtieIDetectorConstruction(YAML::Node config)
    : G4VUserDetectorConstruction()
    , mConfig(config["detector"])
    {
        if(mConfig["active_volume_material"]) { mActiveVolumeMaterialName = mConfig["active_volume_material"].as<std::string>(); }
        if(mConfig["target_radius"]) { mTargetRadius = mConfig["target_radius"].as<G4double>() * cm; }
        if(mConfig["target_length"]) { mTargetLength = mConfig["target_length"].as<G4double>() * cm; }
        if(mConfig["construct_container"])  { mConstructContainer = mConfig["construct_container"].as<G4bool>(); }
        if(mConfig["container_material"])   { mContainerMaterialName = mConfig["container_material"].as<std::string>(); }
        if(mConfig["container_radius"])     { mContainerRadius = mConfig["container_radius"].as<G4double>() * cm; }
        if(mConfig["insulation_thickness"]) { mInsulationThickness = mConfig["insulation_thickness"].as<G4double>() * cm; }
        if(mConfig["window_thickness"])     { mWindowThickness = mConfig["window_thickness"].as<G4double>() * cm; }
        if(mConfig["buffer_length"])        { mBufferLength = mConfig["buffer_length"].as<G4double>() * cm; }
        if(mConfig["construct_beam_pipe"])   { mConstructBeamPipe = mConfig["construct_beam_pipe"].as<G4bool>(); }
        if(mConfig["beam_pipe_inner_radius"]) {mBeamPipeInnerRadius = mConfig["beam_pipe_inner_radius"].as<G4double>() * cm; }
        if(mConfig["beam_pipe_outer_radius"]) {mBeamPipeOuterRadius = mConfig["beam_pipe_outer_radius"].as<G4double>() * cm; }
        if(mConfig["gap"])     { mGap = mConfig["gap"].as<G4double>() * m; }
        if(mConfig["construct_hall"])   { mConstructHall = mConfig["construct_hall"].as<G4bool>(); }
        if(mConfig["world_x"]) { mWorldX = mConfig["world_x"].as<G4double>() * m; }
        if(mConfig["world_y"]) { mWorldY = mConfig["world_y"].as<G4double>() * m; }
        if(mConfig["world_z"]) { mWorldZ = mConfig["world_z"].as<G4double>() * m; }
        if(mConfig["wall_thickness"]) { mWallThickness = mConfig["wall_thickness"].as<G4double>() * m; }
        if(mConfig["construct_detector"])   { mConstructDetector = mConfig["construct_detector"].as<G4bool>(); }
        if(mConfig["detector_radius"]) { mDetectorRadius = mConfig["detector_radius"].as<G4double>() * cm; }
        if(mConfig["detector_length"]) { mDetectorLength = mConfig["detector_length"].as<G4double>() * cm; }
        if(mConfig["detector_entrance"]) { mDetectorEntrance = mConfig["detector_entrance"].as<G4double>() * m; }   
        DefineMaterials();
    }
#endif

    void ArtieIDetectorConstruction::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();

        mIAr36 = new G4Isotope(
            "Ar36",        // name
            18.,            // atomic number
            36,             // atomic mass
            mAr36MassMol
        );  
        
        mIAr38 = new G4Isotope(
            "Ar38",        // name
            18.,            // atomic number
            38,             // atomic mass
            mAr38MassMol
        );  

        mIAr40 = new G4Isotope(
            "Ar40",        // name
            18.,            // atomic number
            40,             // atomic mass
            mAr40MassMol
        );  

        // add the isotopes to the definition of Ar
        mArIsotopes = new G4Element("ArIsotopes", "Ar", 3);
        mArIsotopes->AddIsotope(mIAr36, mAr36Ratio*perCent);
        mArIsotopes->AddIsotope(mIAr38, mAr38Ratio*perCent);
        mArIsotopes->AddIsotope(mIAr40, mAr40Ratio*perCent);

        // need now the definition of LAr with the composition
        mLAr = new G4Material(
            "LAr",          // name
            mAverageDensity,// density
            1,              // # om components
            kStateLiquid,   // state
            mTemperature,   // temperature
            mPressure);     // pressure
        
        mLAr->AddElement(mArIsotopes, 1);

        mWorldMaterial = CreateMaterial(mWorldMaterialName);
        mContainerMaterial = CreateMaterial(mContainerMaterialName); 
        mInsulationMaterial = CreateMaterial(mInsulationMaterialName); 
        mLeftWindowMaterial = CreateMaterial(mLeftWindowMaterialName); 
        mRightWindowMaterial = CreateMaterial(mRightWindowMaterialName); 
        mLeftBufferMaterial = CreateMaterial(mLeftBufferMaterialName); 
        mRightBufferMaterial = CreateMaterial(mRightBufferMaterialName); 
        mBeamPipeLeftBeamMaterial = CreateMaterial(mBeamPipeLeftBeamMaterialName); 
        mBeamPipeRightBeamMaterial = CreateMaterial(mBeamPipeRightBeamMaterialName); 
        mBeamPipeLeftPipeMaterial = CreateMaterial(mBeamPipeLeftPipeMaterialName); 
        mBeamPipeRightPipeMaterial = CreateMaterial(mBeamPipeRightPipeMaterialName); 
        mDetectorMaterial = CreateMaterial(mDetectorMaterialName);
        mHallMaterial = CreateMaterial(mHallMaterialName);
    }

    ArtieIDetectorConstruction::~ArtieIDetectorConstruction()
    {
    }

    G4VPhysicalVolume* ArtieIDetectorConstruction::Construct()
    {
        DefineMaterials();
        // create the world volume
        mSolidExperimentalHall = new G4Box(
            "Solid_ArtieIExperimentalHall", 
            mWorldX, 
            mWorldY, 
            mWorldZ
        );
        mLogicalExperimentalHall = new G4LogicalVolume(
            mSolidExperimentalHall, 
            mWorldMaterial, 
            "Logical_ArtieIExperimentalHall"
        );
        mPhysicalExperimentalHall = new G4PVPlacement(
            0, 
            G4ThreeVector(0., 0., 0.),
            mLogicalExperimentalHall,
            "Physical_ArtieIExperimentalHall",
            0, 
            false,
            0
        );

        // Construct the active volume
        mSolidActiveVolume = new G4Tubs(
            "Solid_ArtieIActiveVolume", 
            0,
            mTargetRadius, 
            0.5 * mTargetLength, 
            0,
            2*M_PI
        );
        mLogicalActiveVolume = new G4LogicalVolume(
            mSolidActiveVolume, 
            mLAr, 
            "Logical_ArtieIActiveVolume"
        );
        mPhysicalActiveVolume = new G4PVPlacement(
            0, 
            G4ThreeVector(0., 0., 0.), 
            mLogicalActiveVolume, 
            "Physical_ArtieIActiveVolume", 
            mLogicalExperimentalHall, 
            false, 
            0, 
            true
        );

        // Construct the container
        if(mConstructContainer)
        {
            // Container
            mSolidContainer = new G4Tubs(
                "Solid_ArtieITargetContainer", 
                mTargetRadius,
                mContainerRadius, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            );
            mLogicalContainer = new G4LogicalVolume(
                mSolidContainer, 
                mContainerMaterial, 
                "Logical_ArtieITargetContainer"
            );
            mPhysicalContainer = new G4PVPlacement(
                0, 
                G4ThreeVector(0.,0.,0.),
                mLogicalContainer, 
                "Physical_ArtieITargetContainer", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Insulation
            mSolidInsulation = new G4Tubs(
                "Solid_ArtieITargetInsulation", 
                mContainerRadius,
                mContainerRadius + mInsulationThickness, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            );
            mLogicalInsulation = new G4LogicalVolume(
                mSolidInsulation, 
                mInsulationMaterial, 
                "Logical_ArtieITargetInsulation"
            );
            mPhysicalInsulation = new G4PVPlacement(
                0, 
                G4ThreeVector(0.,0.,0.),
                mLogicalInsulation, 
                "Physical_ArtieITargetInsulation", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Left Window
            mSolidLeftWindow = new G4Tubs(
                "Solid_ArtieITargetLeftWindow", 
                0,
                mContainerRadius, 
                0.5 * mWindowThickness, 
                0,
                2*M_PI
            );
            mLogicalLeftWindow = new G4LogicalVolume(
                mSolidLeftWindow, 
                mLeftWindowMaterial, 
                "Logical_ArtieITargetLeftWindow"
            );
            mPhysicalLeftWindow = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    -(mTargetLength + mWindowThickness) * 0.5),
                mLogicalLeftWindow, 
                "Physical_ArtieITargetLeftWindow", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
            
            // Right Window
            mSolidRightWindow = new G4Tubs(
                "Solid_ArtieITargetRightWindow", 
                0,
                mContainerRadius, 
                0.5 * mWindowThickness, 
                0,
                2*M_PI
            );
            mLogicalRightWindow = new G4LogicalVolume(
                mSolidRightWindow, 
                mRightWindowMaterial, 
                "Logical_ArtieITargetRightWindow"
            );
            mPhysicalRightWindow = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    +(mTargetLength + mWindowThickness) * 0.5),
                mLogicalRightWindow, 
                "Physical_ArtieITargetRightWindow", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Left Buffer
            mSolidLeftBuffer = new G4Tubs(
                "Solid_ArtieITargetLeftBuffer", 
                0,
                mTargetRadius, 
                0.5 * mBufferLength, 
                0,
                2*M_PI
            );
            mLogicalLeftBuffer = new G4LogicalVolume(
                mSolidLeftBuffer, 
                mLeftBufferMaterial, 
                "Logical_ArtieITargetLeftBuffer"
            );
            mPhysicalLeftBuffer = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    -(mTargetLength + 2 * mWindowThickness + mBufferLength) * 0.5),
                mLogicalLeftBuffer, 
                "Physical_ArtieITargetLeftBuffer", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Right Buffer
            mSolidRightBuffer = new G4Tubs(
                "Solid_ArtieITargetRightBuffer", 
                0,
                mTargetRadius, 
                0.5 * mBufferLength, 
                0,
                2*M_PI
            );
            mLogicalRightBuffer = new G4LogicalVolume(
                mSolidRightBuffer, 
                mRightBufferMaterial, 
                "Logical_ArtieITargetRightBuffer"
            );
            mPhysicalRightBuffer = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    +(mTargetLength + 2 * mWindowThickness + mBufferLength) * 0.5),
                mLogicalRightBuffer, 
                "Physical_ArtieITargetRightBuffer", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
        }

        // Construct the beampipe
        if(mConstructBeamPipe)
        {
            mBeamPipeLeftHalfLength = (-mGap / 2.0 + mWorldZ / 2.0) / 2.0;
            mBeamPipeRightHalfLength = (mDetectorEntrance - mGap / 2.0) / 2.0;
            mBeamPipeLeftPosition = {
                0., 
                0., 
                (-mWorldZ / 2.0 - mGap / 2.0) / 2.0
            };
            mBeamPipeRightPosition = {
                0., 
                0., 
                (mDetectorEntrance + mGap / 2.0) / 2.0
            };

            // Left Beam
            mSolidBeamPipeLeftBeam = new G4Tubs(
                "Solid_ArtieIBeamPipeLeftBeam", 
                0,
                mBeamPipeInnerRadius, 
                mBeamPipeLeftHalfLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeLeftBeam = new G4LogicalVolume(
                mSolidBeamPipeLeftBeam, 
                mBeamPipeLeftBeamMaterial, 
                "Logical_ArtieIBeamPipeLeftBeam"
            );
            mPhysicalBeamPipeLeftBeam = new G4PVPlacement(
                0, 
                mBeamPipeLeftPosition, 
                mLogicalBeamPipeLeftBeam, 
                "Physical_ArtieIBeamPipeLeftBeam", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Right Beam
            mSolidBeamPipeRightBeam = new G4Tubs(
                "Solid_ArtieIBeamPipeRightBeam", 
                0,
                mBeamPipeInnerRadius, 
                mBeamPipeRightHalfLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeRightBeam = new G4LogicalVolume(
                mSolidBeamPipeRightBeam, 
                mBeamPipeRightBeamMaterial, 
                "Logical_ArtieIBeamPipeRightBeam"
            );
            mPhysicalBeamPipeRightBeam = new G4PVPlacement(
                0, 
                mBeamPipeRightPosition, 
                mLogicalBeamPipeRightBeam, 
                "Physical_ArtieIBeamPipeRightBeam", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Left Pipe
            mSolidBeamPipeLeftPipe = new G4Tubs(
                "Solid_ArtieIBeamPipeLeftPipe", 
                mBeamPipeInnerRadius,
                mBeamPipeOuterRadius, 
                mBeamPipeLeftHalfLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeLeftPipe = new G4LogicalVolume(
                mSolidBeamPipeLeftPipe, 
                mBeamPipeLeftPipeMaterial, 
                "Logical_ArtieIBeamPipeLeftPipe"
            );
            mPhysicalBeamPipeLeftPipe = new G4PVPlacement(
                0, 
                mBeamPipeLeftPosition, 
                mLogicalBeamPipeLeftPipe, 
                "Physical_ArtieIBeamPipeLeftPipe", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );

            // Right Pipe
            mSolidBeamPipeRightPipe = new G4Tubs(
                "Solid_ArtieIBeamPipeRightPipe", 
                mBeamPipeInnerRadius,
                mBeamPipeOuterRadius, 
                mBeamPipeRightHalfLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeRightPipe = new G4LogicalVolume(
                mSolidBeamPipeRightPipe, 
                mBeamPipeRightPipeMaterial, 
                "Logical_ArtieIBeamPipeRightPipe"
            );
            mPhysicalBeamPipeRightPipe = new G4PVPlacement(
                0, 
                mBeamPipeRightPosition, 
                mLogicalBeamPipeRightPipe, 
                "Physical_ArtieIBeamPipeRightPipe", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
        }

        // Construct the detector
        if(mConstructDetector)
        {
            mSolidDetector = new G4Tubs(
                "Solid_ArtieITargetDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            );
            mLogicalDetector = new G4LogicalVolume(
                mSolidDetector, 
                mDetectorMaterial, 
                "Logical_ArtieITargetDetector"
            );
            mPhysicalDetector = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    + mDetectorEntrance + mDetectorLength * 0.5),
                mLogicalDetector, 
                "Physical_ArtieITargetDetector", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
        }

        // Construct the hall
        if(mConstructHall)
        {
            G4Box* OuterWall = new G4Box(
                "Solid_ArtieIHall_OuterWall",
                mWorldX / 2.0,
                mWorldY / 2.0,
                mWorldZ / 2.0
            );
            G4Box* InnerWall = new G4Box(
                "Solid_ArtieIHall_InnerWall",
                mWorldX / 2.0 - mWallThickness,
                mWorldY / 2.0 - mWallThickness,
                mWorldZ / 2.0 - mWallThickness
            );
            mSolidHall = new G4SubtractionSolid(
                "Solid_ArtieIHall", 
                OuterWall,
                InnerWall
            );
            mLogicalHall = new G4LogicalVolume(
                mSolidHall, 
                mHallMaterial, 
                "Logical_ArtieIHall"
            );
            mPhysicalHall = new G4PVPlacement(
                0, 
                G4ThreeVector(), 
                mLogicalHall, 
                "Physical_ArtieIHall", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
        }        

        return mPhysicalExperimentalHall;
    }

    void ArtieIDetectorConstruction::ConstructSDandField()
    {
    }
}