/**
 * @file ArtieIInTOFDetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIInTOFDetectorConstruction.hh"

namespace Artie
{
    ArtieIInTOFDetectorConstruction::ArtieIInTOFDetectorConstruction()
    : G4VUserDetectorConstruction()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    ArtieIInTOFDetectorConstruction::ArtieIInTOFDetectorConstruction(YAML::Node config)
    : G4VUserDetectorConstruction()
    , mConfig(config)
    {
        if(mConfig["target"]["target_entrance"]) { mTargetEntrance = mConfig["target"]["target_entrance"].as<G4double>() * m; }
        if(mConfig["target"]["target_x"]) { mTargetX = mConfig["target"]["target_x"].as<G4double>() * cm; }
        if(mConfig["target"]["target_y"]) { mTargetY = mConfig["target"]["target_y"].as<G4double>() * cm; }
        if(mConfig["target"]["active_volume_material"]) { mActiveVolumeMaterialName = mConfig["target"]["active_volume_material"].as<std::string>(); }
        if(mConfig["target"]["target_radius"])          { mTargetRadius = mConfig["target"]["target_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["target_length"])          { mTargetLength = mConfig["target"]["target_length"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_container"])  { mConstructContainer = mConfig["target"]["construct_container"].as<G4bool>(); }
        if(mConfig["target"]["container_outer_material"])   { mContainerOuterMaterialName = mConfig["target"]["container_outer_material"].as<std::string>(); }
        if(mConfig["target"]["container_inner_material"])   { mContainerInnerMaterialName = mConfig["target"]["container_inner_material"].as<std::string>(); }
        if(mConfig["target"]["container_outer_radius"])     { mContainerOuterRadius = mConfig["target"]["container_outer_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["container_inner_radius"])     { mContainerInnerRadius = mConfig["target"]["container_inner_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["container_length"])     { mContainerLength = mConfig["target"]["container_length"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_annulus"])          { mConstructAnnulus = mConfig["target"]["construct_annulus"].as<G4bool>(); }
        if(mConfig["target"]["annulus_container_material"]) { mAnnulusContainerMaterialName = mConfig["target"]["annulus_container_material"].as<std::string>(); }
        if(mConfig["target"]["annulus_inner_material"])     { mAnnulusInnerMaterialName = mConfig["target"]["annulus_inner_material"].as<std::string>(); }
        if(mConfig["target"]["annulus_container_radius"])   { mAnnulusContainerRadius = mConfig["target"]["annulus_container_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_inner_radius"])       { mAnnulusInnerRadius = mConfig["target"]["annulus_inner_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_length"])             { mAnnulusLength = mConfig["target"]["annulus_length"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_face_thickness"])     { mAnnulusFaceThickness = mConfig["target"]["annulus_face_thickness"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_active_volume_container"])  { mConstructActiveVolumeContainer = mConfig["target"]["construct_active_volume_container"].as<G4bool>(); }
        if(mConfig["target"]["active_volume_container_material"])   { mActiveVolumeContainerMaterialName = mConfig["target"]["active_volume_container_material"].as<std::string>(); }
        if(mConfig["target"]["active_volume_container_radius"])     { mActiveVolumeContainerRadius = mConfig["target"]["active_volume_container_radius"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_inner_flange_left_side"]) { mConstructInnerFlangeLeftSide = mConfig["target"]["construct_inner_flange_left_side"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_left_side_material"])  { mInnerFlangeLeftSideMaterialName = mConfig["target"]["inner_flange_left_side_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_left_side_radius"])    { mInnerFlangeLeftSideRadius = mConfig["target"]["inner_flange_left_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["inner_flange_left_side_thickness"]) { mInnerFlangeLeftSideThickness = mConfig["target"]["inner_flange_left_side_thickness"].as<G4double>() * cm; } 
        
        if(mConfig["target"]["construct_inner_flange_right_side"]) { mConstructInnerFlangeRightSide = mConfig["target"]["construct_inner_flange_right_side"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_right_side_material"])  { mInnerFlangeRightSideMaterialName = mConfig["target"]["inner_flange_right_side_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_right_side_radius"])    { mInnerFlangeRightSideRadius = mConfig["target"]["inner_flange_right_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["inner_flange_right_side_thickness"]) { mInnerFlangeRightSideThickness = mConfig["target"]["inner_flange_right_side_thickness"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_inner_flange_left_side_gap"]) { mConstructInnerFlangeLeftSideGap = mConfig["target"]["construct_inner_flange_left_side_gap"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_left_side_gap_material"])  { mInnerFlangeLeftSideGapMaterialName = mConfig["target"]["inner_flange_left_side_gap_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_left_side_gap_thickness"]) { mInnerFlangeLeftSideGapThickness = mConfig["target"]["inner_flange_left_side_gap_thickness"].as<G4double>() * cm; } 
        
        if(mConfig["target"]["construct_inner_flange_left_side_gap"]) { mConstructInnerFlangeLeftSideGap = mConfig["target"]["construct_inner_flange_left_side_gap"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_right_side_gap_material"])  { mInnerFlangeRightSideGapMaterialName = mConfig["target"]["inner_flange_right_side_gap_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_right_side_gap_thickness"]) { mInnerFlangeRightSideGapThickness = mConfig["target"]["inner_flange_right_side_gap_thickness"].as<G4double>() * cm; } 

        if(mConfig["target"]["construct_outer_flange_left_side"]) { mConstructOuterFlangeLeftSide = mConfig["target"]["construct_outer_flange_left_side"].as<G4bool>(); }
        if(mConfig["target"]["outer_flange_left_side_material"])  { mOuterFlangeLeftSideMaterialName = mConfig["target"]["outer_flange_left_side_material"].as<std::string>(); }
        if(mConfig["target"]["outer_flange_left_side_radius"])    { mOuterFlangeLeftSideRadius = mConfig["target"]["outer_flange_left_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["outer_flange_left_side_thickness"]) { mOuterFlangeLeftSideThickness = mConfig["target"]["outer_flange_left_side_thickness"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_outer_flange_right_side"]) { mConstructOuterFlangeRightSide = mConfig["target"]["construct_outer_flange_right_side"].as<G4bool>(); }
        if(mConfig["target"]["outer_flange_right_side_material"])  { mOuterFlangeRightSideMaterialName = mConfig["target"]["outer_flange_right_side_material"].as<std::string>(); }
        if(mConfig["target"]["outer_flange_right_side_radius"])    { mOuterFlangeRightSideRadius = mConfig["target"]["outer_flange_right_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["outer_flange_right_side_thickness"]) { mOuterFlangeRightSideThickness = mConfig["target"]["outer_flange_right_side_thickness"].as<G4double>() * cm; }

        if(mConfig["detector"]["construct_detector"])   { mConstructDetector = mConfig["detector"]["construct_detector"].as<G4bool>(); }
        if(mConfig["detector"]["detector_radius"])      { mDetectorRadius = mConfig["detector"]["detector_radius"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_length"])      { mDetectorLength = mConfig["detector"]["detector_length"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_entrance"])    { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }   
        if(mConfig["detector"]["detector_separation"])  { mDetectorSeparation = mConfig["detector"]["detector_separation"].as<G4double>() * cm; }

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

    void ArtieIInTOFDetectorConstruction::DefineMaterials()
    {
        mActiveVolumeMaterial = CreateMaterial(mActiveVolumeMaterialName, "ActiveVolume");
        mWorldMaterial = CreateMaterial(mWorldMaterialName, "World");

        mContainerOuterMaterial = CreateMaterial(mContainerOuterMaterialName, "ContainerOuter");
        mContainerInnerMaterial = CreateMaterial(mContainerInnerMaterialName, "ContainerInner");

        mAnnulusInnerMaterial = CreateMaterial(mAnnulusInnerMaterialName, "AnnulusInner");
        mAnnulusContainerMaterial = CreateMaterial(mAnnulusContainerMaterialName, "AnnulusContainer");
        mAnnulusLeftFaceMaterial = CreateMaterial(mAnnulusLeftFaceMaterialName, "AnnulusLeftFace");
        mAnnulusRightFaceMaterial = CreateMaterial(mAnnulusRightFaceMaterialName, "AnnulusRightFace");

        mActiveVolumeContainerMaterial = CreateMaterial(mActiveVolumeContainerMaterialName, "ActiveVolumeContainer"); 
        mInnerFlangeLeftSideMaterial = CreateMaterial(mInnerFlangeLeftSideMaterialName, "InnerFlangeLeftSide");
        mInnerFlangeRightSideMaterial = CreateMaterial(mInnerFlangeRightSideMaterialName, "InnerFlangeRightSide");
        
        mOuterFlangeLeftSideMaterial = CreateMaterial(mOuterFlangeLeftSideMaterialName, "OuterFlangeLeftSide");
        mOuterFlangeRightSideMaterial = CreateMaterial(mOuterFlangeRightSideMaterialName, "OuterFlangeRightSide");

        mDetectorMaterial = CreateMaterial(mDetectorMaterialName, "Detector");

        for (G4int i = 0; i < mFilterMaterialNames.size(); i++)
        {
            mFilterMaterials.push_back(CreateMaterial(mFilterMaterialNames[i], "Filter"+mFilterMaterialNames[i]));;
        }
    }

    ArtieIInTOFDetectorConstruction::~ArtieIInTOFDetectorConstruction()
    {
    }

    G4VPhysicalVolume* ArtieIInTOFDetectorConstruction::Construct()
    {
        DefineMaterials();
        G4double mTargetZ = mTZeroLocation + mTargetEntrance + 0.5 * mContainerLength;

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
        mSolidContainerInner = new G4Tubs(
            "Solid_ArtieIITargetContainerInner", 
            0,
            mContainerOuterRadius, 
            0.5 * mContainerLength, 
            0,
            2*M_PI
        );
        mLogicalContainerInner = new G4LogicalVolume(
            mSolidContainerInner, 
            mContainerInnerMaterial, 
            "Logical_ArtieIITargetContainerInner"
        );
        mPhysicalContainerInner = new G4PVPlacement(
            0, 
            G4ThreeVector(mTargetX, mTargetY, mTargetZ),
            mLogicalContainerInner, 
            "Physical_ArtieIITargetContainerInner", 
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
            mLogicalContainerInner, 
            false, 
            0, 
            true
        );

        if(mConstructContainer)
        {
            // construct outer container
            mSolidContainerOuter = new G4Tubs(
                "Solid_ArtieIITargetContainerOuter", 
                mContainerInnerRadius,
                mContainerOuterRadius, 
                0.5 * mContainerLength, 
                0,
                2*M_PI
            );
            mLogicalContainerOuter = new G4LogicalVolume(
                mSolidContainerOuter, 
                mContainerOuterMaterial, 
                "Logical_ArtieIITargetContainerOuter"
            );
            mPhysicalContainerOuter = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalContainerOuter, 
                "Physical_ArtieIITargetContainerOuter", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
        }

        if(mConstructAnnulus)
        {
            // construct annulus argon
            mSolidAnnulusInner = new G4Tubs(
                "Solid_ArtieIITargetAnnulusInner", 
                mAnnulusInnerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusLength, 
                0,
                2*M_PI
            );
            mLogicalAnnulusInner = new G4LogicalVolume(
                mSolidAnnulusInner, 
                mAnnulusInnerMaterial, 
                "Logical_ArtieIITargetAnnulusInner"
            );
            mPhysicalAnnulusInner = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalAnnulusInner, 
                "Physical_ArtieIITargetAnnulusInner", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // construct annulus inner container
            mSolidAnnulusContainer = new G4Tubs(
                "Solid_ArtieIITargetAnnulusContainer", 
                mAnnulusContainerRadius,
                mAnnulusInnerRadius, 
                0.5 * mAnnulusLength, 
                0,
                2*M_PI
            );
            mLogicalAnnulusContainer = new G4LogicalVolume(
                mSolidAnnulusContainer, 
                mAnnulusContainerMaterial, 
                "Logical_ArtieIITargetAnnulusContainer"
            );
            mPhysicalAnnulusContainer = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalAnnulusContainer, 
                "Physical_ArtieIITargetAnnulusContainer", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // contruct annulus left face
            mSolidAnnulusLeftFace = new G4Tubs(
                "Solid_ArtieIITargetAnnulusLeftFace", 
                mAnnulusContainerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusFaceThickness, 
                0,
                2*M_PI
            );
            mLogicalAnnulusLeftFace = new G4LogicalVolume(
                mSolidAnnulusLeftFace, 
                mAnnulusLeftFaceMaterial, 
                "Logical_ArtieIITargetAnnulusLeftFace"
            );
            mPhysicalAnnulusLeftFace = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,-(mAnnulusLength + mAnnulusFaceThickness) * 0.5),
                mLogicalAnnulusLeftFace, 
                "Physical_ArtieIITargetAnnulusLeftFace", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
            // contruct annulus left face
            mSolidAnnulusRightFace = new G4Tubs(
                "Solid_ArtieIITargetAnnulusRightFace", 
                mAnnulusContainerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusFaceThickness, 
                0,
                2*M_PI
            );
            mLogicalAnnulusRightFace = new G4LogicalVolume(
                mSolidAnnulusRightFace, 
                mAnnulusRightFaceMaterial, 
                "Logical_ArtieIITargetAnnulusRightFace"
            );
            mPhysicalAnnulusRightFace = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,+(mAnnulusLength + mAnnulusFaceThickness) * 0.5),
                mLogicalAnnulusRightFace, 
                "Physical_ArtieIITargetAnnulusRightFace", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
        }

        // Construct the active volume container
        if(mConstructActiveVolumeContainer)
        {
            // ActiveVolumeContainer
            mSolidActiveVolumeContainer = new G4Tubs(
                "Solid_ArtieIITargetActiveVolumeContainer", 
                mTargetRadius,
                mActiveVolumeContainerRadius, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            );
            mLogicalActiveVolumeContainer = new G4LogicalVolume(
                mSolidActiveVolumeContainer, 
                mActiveVolumeContainerMaterial, 
                "Logical_ArtieIITargetActiveVolumeContainer"
            );
            mPhysicalActiveVolumeContainer = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalActiveVolumeContainer, 
                "Physical_ArtieIITargetActiveVolumeContainer", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // left side inner flange
            if(mConstructInnerFlangeLeftSide)
            {
                mSolidInnerFlangeLeftSide = new G4Tubs(
                    "Solid_ArtieIITargetInnerFlangeLeftSide",
                    0,
                    mInnerFlangeLeftSideRadius,
                    0.5 * mInnerFlangeLeftSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalInnerFlangeLeftSide = new G4LogicalVolume(
                    mSolidInnerFlangeLeftSide,
                    mInnerFlangeLeftSideMaterial,
                    "Logical_ArtieIITargetInnerFlangeLeftSide"
                );
                mPhysicalInnerFlangeLeftSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        0,
                        0,
                        -(mTargetLength + mInnerFlangeLeftSideThickness) * 0.5),
                    mLogicalInnerFlangeLeftSide,
                    "Physical_ArtieIITargetInnerFlangeLeftSide",
                    mLogicalContainerInner,
                    false,
                    0,
                    true 
                );
            }
            // right side inner flange
            if(mConstructInnerFlangeRightSide)
            {
                mSolidInnerFlangeRightSide = new G4Tubs(
                    "Solid_ArtieIITargetInnerFlangeRightSide",
                    0,
                    mInnerFlangeRightSideRadius,
                    0.5 * mInnerFlangeRightSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalInnerFlangeRightSide = new G4LogicalVolume(
                    mSolidInnerFlangeRightSide,
                    mInnerFlangeRightSideMaterial,
                    "Logical_ArtieIITargetInnerFlangeRightSide"
                );
                mPhysicalInnerFlangeRightSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        0,
                        0,
                        + (mTargetLength + mInnerFlangeRightSideThickness) * 0.5),
                    mLogicalInnerFlangeRightSide,
                    "Physical_ArtieIITargetInnerFlangeRightSide",
                    mLogicalContainerInner,
                    false,
                    0,
                    true 
                );
            }
            // left side Outer flange
            if(mConstructOuterFlangeLeftSide)
            {
                mSolidOuterFlangeLeftSide = new G4Tubs(
                    "Solid_ArtieIITargetOuterFlangeLeftSide",
                    0,
                    mOuterFlangeLeftSideRadius,
                    0.5 * mOuterFlangeLeftSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalOuterFlangeLeftSide = new G4LogicalVolume(
                    mSolidOuterFlangeLeftSide,
                    mOuterFlangeLeftSideMaterial,
                    "Logical_ArtieIITargetOuterFlangeLeftSide"
                );
                mPhysicalOuterFlangeLeftSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        mTargetX,
                        mTargetY,
                        mTargetZ - (mContainerLength + mOuterFlangeLeftSideThickness) * 0.5),
                    mLogicalOuterFlangeLeftSide,
                    "Physical_ArtieIITargetOuterFlangeLeftSide",
                    mLogicalExperimentalHall,
                    false,
                    0,
                    true 
                );
            }
            // right side Outer flange
            if(mConstructOuterFlangeRightSide)
            {
                mSolidOuterFlangeRightSide = new G4Tubs(
                    "Solid_ArtieIITargetOuterFlangeRightSide",
                    0,
                    mOuterFlangeRightSideRadius,
                    0.5 * mOuterFlangeRightSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalOuterFlangeRightSide = new G4LogicalVolume(
                    mSolidOuterFlangeRightSide,
                    mOuterFlangeRightSideMaterial,
                    "Logical_ArtieIITargetOuterFlangeRightSide"
                );
                mPhysicalOuterFlangeRightSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        mTargetX,
                        mTargetY,
                        mTargetZ + (mContainerLength + mOuterFlangeRightSideThickness) * 0.5),
                    mLogicalOuterFlangeRightSide,
                    "Physical_ArtieIITargetOuterFlangeRightSide",
                    mLogicalExperimentalHall,
                    false,
                    0,
                    true 
                );
            }
        }

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

    void ArtieIInTOFDetectorConstruction::ConstructSDandField()
    {
    }
}