/**
 * @file ArtieIInTOFSCUBADetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIInTOFSCUBADetectorConstruction.hh"

namespace Artie
{
    ArtieIInTOFSCUBADetectorConstruction::ArtieIInTOFSCUBADetectorConstruction()
    : G4VUserDetectorConstruction()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    ArtieIInTOFSCUBADetectorConstruction::ArtieIInTOFSCUBADetectorConstruction(YAML::Node config)
    : G4VUserDetectorConstruction()
    , mConfig(config)
    {
        if(mConfig["target"]["target_entrance"]) { mTargetEntrance = mConfig["target"]["target_entrance"].as<G4double>() * m; }
        if(mConfig["target"]["target_x"]) { mTargetX = mConfig["target"]["target_x"].as<G4double>() * cm; }
        if(mConfig["target"]["target_y"]) { mTargetY = mConfig["target"]["target_y"].as<G4double>() * cm; }
        if(mConfig["target"]["active_volume_material"]) { mActiveVolumeMaterialName = mConfig["target"]["active_volume_material"].as<std::string>(); }
        if(mConfig["target"]["target_radius"])          { mTargetRadius = mConfig["target"]["target_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["target_length"])          { mTargetLength = mConfig["target"]["target_length"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_scuba_tank"])  { mConstructSCUBATank = mConfig["target"]["construct_scuba_tank"].as<G4bool>(); }
        if(mConfig["target"]["scuba_tank_material"])   { mSCUBATankMaterialName = mConfig["target"]["scuba_tank_material"].as<std::string>(); }
        if(mConfig["target"]["scuba_tank_radius"])     { mSCUBATankRadius = mConfig["target"]["scuba_tank_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["scuba_tank_length"])     { mSCUBATankLength = mConfig["target"]["scuba_tank_length"].as<G4double>() * cm; }

        if(mConfig["detector"]["construct_detector"])   { mConstructDetector = mConfig["detector"]["construct_detector"].as<G4bool>(); }
        if(mConfig["detector"]["detector_radius"])      { mDetectorRadius = mConfig["detector"]["detector_radius"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_length"])      { mDetectorLength = mConfig["detector"]["detector_length"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_entrance"])    { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }   

        if(mConfig["hall"]["world_material"])   { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }
        if(mConfig["hall"]["world_x"])          { mWorldX = mConfig["hall"]["world_x"].as<G4double>() * m; }
        if(mConfig["hall"]["world_y"])          { mWorldY = mConfig["hall"]["world_y"].as<G4double>() * m; }
        if(mConfig["hall"]["world_z"])          { mWorldZ = mConfig["hall"]["world_z"].as<G4double>() * m; }

        // filters
        if(mConfig["filters"]["construct_filters"])    { mConstructFilters = mConfig["filters"]["construct_filters"].as<G4bool>(); }
        if(mConfig["filters"]["filter_materials"])     { mFilterMaterialNames = mConfig["filters"]["filter_materials"].as<std::vector<std::string>>(); }
        if(mConfig["filters"]["filter_z_position"])    { mFilterZPos = mConfig["filters"]["filter_z_position"].as<std::vector<G4double>>(); }
        if(mConfig["filters"]["filter_thickness"])     { mFilterThickness = mConfig["filters"]["filter_thickness"].as<std::vector<G4double>>(); }
        if(mConfig["filters"]["filter_radius"])        { mFilterRadius = mConfig["filters"]["filter_radius"].as<std::vector<G4double>>(); }

        if(mConfig["generator"]["t_zero_location"])     { mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m;}

        DefineMaterials();
    }
#endif

    void ArtieIInTOFSCUBADetectorConstruction::DefineMaterials()
    {
        mActiveVolumeMaterial = CreateMaterial(mActiveVolumeMaterialName, "ActiveVolume");
        mWorldMaterial = CreateMaterial(mWorldMaterialName, "World");

        mSCUBATankMaterial = CreateMaterial(mSCUBATankMaterialName, "SCUBATank");

        mDetectorMaterial = CreateMaterial(mDetectorMaterialName, "Detector");

        for (G4int i = 0; i < mFilterMaterialNames.size(); i++)
        {
            mFilterMaterials.push_back(CreateMaterial(mFilterMaterialNames[i], "Filter"+mFilterMaterialNames[i]));;
        }
    }

    ArtieIInTOFSCUBADetectorConstruction::~ArtieIInTOFSCUBADetectorConstruction()
    {
    }

    G4VPhysicalVolume* ArtieIInTOFSCUBADetectorConstruction::Construct()
    {
        DefineMaterials();
        G4double mTargetZ = mTZeroLocation + mTargetEntrance + 0.5 * mSCUBATankLength;

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

        // Construct outer container mother volume
        mSolidSCUBATank = new G4Tubs(
            "Solid_ArtieIITargetSCUBATank", 
            0,
            mSCUBATankRadius, 
            0.5 * mSCUBATankLength, 
            0,
            2*M_PI
        );
        mLogicalSCUBATank = new G4LogicalVolume(
            mSolidSCUBATank, 
            mSCUBATankMaterial, 
            "Logical_ArtieIITargetSCUBATank"
        );
        mPhysicalSCUBATank = new G4PVPlacement(
            0, 
            G4ThreeVector(mTargetX, mTargetY, mTargetZ),
            mLogicalSCUBATank, 
            "Physical_ArtieIITargetSCUBATank", 
            mLogicalExperimentalHall, 
            false, 
            0, 
            true
        );

        // Construct the active volume
        mSolidActiveVolume = new G4Tubs(
            "Solid_ArtieIIActiveVolume", 
            0,
            mTargetRadius, 
            0.5 * mTargetLength, 
            0,
            2*M_PI
        );
        mLogicalActiveVolume = new G4LogicalVolume(
            mSolidActiveVolume, 
            mActiveVolumeMaterial, 
            "Logical_ArtieIIActiveVolume"
        );
        mPhysicalActiveVolume = new G4PVPlacement(
            0, 
            G4ThreeVector(0.,0.,0.), 
            mLogicalActiveVolume, 
            "Physical_ArtieIIActiveVolume", 
            mLogicalSCUBATank, 
            false, 
            0, 
            true
        );

        // Construct the detector
        if(mConstructDetector)
        {
            mSolidDetector = new G4Tubs(
                "Solid_ArtieIITargetDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            );
            mLogicalDetector = new G4LogicalVolume(
                mSolidDetector, 
                mDetectorMaterial, 
                "Logical_ArtieIITargetDetector"
            );
            mPhysicalDetector = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    + mTZeroLocation + mDetectorEntrance + mDetectorLength * 0.5),
                mLogicalDetector, 
                "Physical_ArtieIITargetDetector", 
                mLogicalExperimentalHall, 
                false, 
                0, 
                true
            );
        }

        // Construct Filters
        if(mConstructFilters)
        {
            for (G4int i = 0; i < mFilterMaterialNames.size(); i++)
            {
                // Solid
                G4Tubs* solidFilter = new G4Tubs(
                    "Solid_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i),
                    0,
                    mFilterRadius[i] * cm, 
                    0.5 * mFilterThickness[i] * cm, 
                    0,
                    2*M_PI
                );
                mSolidFilters.push_back(solidFilter);
                // Logical Volume
                G4LogicalVolume* LogicalFilter = new G4LogicalVolume(
                    mSolidFilters[i], 
                    mFilterMaterials[i], 
                    "Logical_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i)
                );
                mLogicalFilters.push_back(LogicalFilter);

                // Physical Volume
                G4VPhysicalVolume* PhysicalFilter = new G4PVPlacement(
                    0, 
                    G4ThreeVector(
                        0.,
                        0., 
                        mFilterZPos[i] * cm),
                    mLogicalFilters[i], 
                    "Physical_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i), 
                    mLogicalExperimentalHall, 
                    false, 
                    0, 
                    true
                );
                mPhysicalFilters.push_back(PhysicalFilter);
            }
        }

        // // Construct the hall
        // if(mConstructHall)
        // {
        //     G4Box* OuterWall = new G4Box(
        //         "Solid_ArtieIHall_OuterWall",
        //         mWorldX / 2.0,
        //         mWorldY / 2.0,
        //         mWorldZ / 2.0
        //     );
        //     G4Box* InnerWall = new G4Box(
        //         "Solid_ArtieIHall_InnerWall",
        //         mWorldX / 2.0 - mWallThickness,
        //         mWorldY / 2.0 - mWallThickness,
        //         mWorldZ / 2.0 - mWallThickness
        //     );
        //     mSolidHall = new G4SubtractionSolid(
        //         "Solid_ArtieIHall", 
        //         OuterWall,
        //         InnerWall
        //     );
        //     mLogicalHall = new G4LogicalVolume(
        //         mSolidHall, 
        //         mHallMaterial, 
        //         "Logical_ArtieIHall"
        //     );
        //     mPhysicalHall = new G4PVPlacement(
        //         0, 
        //         G4ThreeVector(), 
        //         mLogicalHall, 
        //         "Physical_ArtieIHall", 
        //         mLogicalExperimentalHall, 
        //         false, 
        //         0, 
        //         true
        //     );
        // }       
        // // Construct the beampipe
        // if(mConstructBeamPipe)
        // {
        //     mBeamPipeLeftHalfLength = (-mGap / 2.0 + mWorldZ / 2.0) / 2.0;
        //     mBeamPipeRightHalfLength = (mDetectorEntrance - mGap / 2.0) / 2.0;
        //     mBeamPipeLeftPosition = {
        //         0., 
        //         0., 
        //         (-mWorldZ / 2.0 - mGap / 2.0) / 2.0
        //     };
        //     mBeamPipeRightPosition = {
        //         0., 
        //         0., 
        //         (mDetectorEntrance + mGap / 2.0) / 2.0
        //     };

        //     // Left Beam
        //     mSolidBeamPipeLeftBeam = new G4Tubs(
        //         "Solid_ArtieIBeamPipeLeftBeam", 
        //         0,
        //         mBeamPipeInnerRadius, 
        //         mBeamPipeLeftHalfLength, 
        //         0,
        //         2*M_PI
        //     );
        //     mLogicalBeamPipeLeftBeam = new G4LogicalVolume(
        //         mSolidBeamPipeLeftBeam, 
        //         mBeamPipeLeftBeamMaterial, 
        //         "Logical_ArtieIBeamPipeLeftBeam"
        //     );
        //     mPhysicalBeamPipeLeftBeam = new G4PVPlacement(
        //         0, 
        //         mBeamPipeLeftPosition, 
        //         mLogicalBeamPipeLeftBeam, 
        //         "Physical_ArtieIBeamPipeLeftBeam", 
        //         mLogicalExperimentalHall, 
        //         false, 
        //         0, 
        //         true
        //     );

        //     // Right Beam
        //     mSolidBeamPipeRightBeam = new G4Tubs(
        //         "Solid_ArtieIBeamPipeRightBeam", 
        //         0,
        //         mBeamPipeInnerRadius, 
        //         mBeamPipeRightHalfLength, 
        //         0,
        //         2*M_PI
        //     );
        //     mLogicalBeamPipeRightBeam = new G4LogicalVolume(
        //         mSolidBeamPipeRightBeam, 
        //         mBeamPipeRightBeamMaterial, 
        //         "Logical_ArtieIBeamPipeRightBeam"
        //     );
        //     mPhysicalBeamPipeRightBeam = new G4PVPlacement(
        //         0, 
        //         mBeamPipeRightPosition, 
        //         mLogicalBeamPipeRightBeam, 
        //         "Physical_ArtieIBeamPipeRightBeam", 
        //         mLogicalExperimentalHall, 
        //         false, 
        //         0, 
        //         true
        //     );

        //     // Left Pipe
        //     mSolidBeamPipeLeftPipe = new G4Tubs(
        //         "Solid_ArtieIBeamPipeLeftPipe", 
        //         mBeamPipeInnerRadius,
        //         mBeamPipeOuterRadius, 
        //         mBeamPipeLeftHalfLength, 
        //         0,
        //         2*M_PI
        //     );
        //     mLogicalBeamPipeLeftPipe = new G4LogicalVolume(
        //         mSolidBeamPipeLeftPipe, 
        //         mBeamPipeLeftPipeMaterial, 
        //         "Logical_ArtieIBeamPipeLeftPipe"
        //     );
        //     mPhysicalBeamPipeLeftPipe = new G4PVPlacement(
        //         0, 
        //         mBeamPipeLeftPosition, 
        //         mLogicalBeamPipeLeftPipe, 
        //         "Physical_ArtieIBeamPipeLeftPipe", 
        //         mLogicalExperimentalHall, 
        //         false, 
        //         0, 
        //         true
        //     );

        //     // Right Pipe
        //     mSolidBeamPipeRightPipe = new G4Tubs(
        //         "Solid_ArtieIBeamPipeRightPipe", 
        //         mBeamPipeInnerRadius,
        //         mBeamPipeOuterRadius, 
        //         mBeamPipeRightHalfLength, 
        //         0,
        //         2*M_PI
        //     );
        //     mLogicalBeamPipeRightPipe = new G4LogicalVolume(
        //         mSolidBeamPipeRightPipe, 
        //         mBeamPipeRightPipeMaterial, 
        //         "Logical_ArtieIBeamPipeRightPipe"
        //     );
        //     mPhysicalBeamPipeRightPipe = new G4PVPlacement(
        //         0, 
        //         mBeamPipeRightPosition, 
        //         mLogicalBeamPipeRightPipe, 
        //         "Physical_ArtieIBeamPipeRightPipe", 
        //         mLogicalExperimentalHall, 
        //         false, 
        //         0, 
        //         true
        //     );
        // } 



        return mPhysicalExperimentalHall;
    }

    void ArtieIInTOFSCUBADetectorConstruction::ConstructSDandField()
    {
    }
}