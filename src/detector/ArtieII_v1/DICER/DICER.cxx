/**
 * @file DICER.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "DICER.hh"

namespace Artie
{
    DICER::DICER()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    DICER::DICER(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["dicer"]["construct_rbb"])   { mConstructRotatingBeamBlocker = mConfig["dicer"]["construct_rbb"].as<G4bool>(); }
        if(mConfig["dicer"]["rbb_material"])    { mRotatingBeamBlockerMaterialName = mConfig["dicer"]["rbb_material"].as<std::string>(); }
        if(mConfig["dicer"]["rbb_entrance"])    { mRotatingBeamBlockerEntrance = mConfig["dicer"]["rbb_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["rbb_diameter"])    { mRotatingBeamBlockerDiameter = mConfig["dicer"]["rbb_diameter"].as<G4double>() * cm; }
        if(mConfig["dicer"]["rbb_length"])      { mRotatingBeamBlockerLength = mConfig["dicer"]["rbb_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["rbb_hole_size"])   { mRotatingBeamBlockerHoleSize = mConfig["dicer"]["rbb_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["rbb_hole_separation"])   { mRotatingBeamBlockerHoleSeparation = mConfig["dicer"]["rbb_hole_separation"].as<G4double>() * cm; }

        if(mConfig["dicer"]["construct_bc"])   { mConstructBinocularCollimator = mConfig["dicer"]["construct_bc"].as<G4bool>(); }
        if(mConfig["dicer"]["bc_material"])    { mBinocularCollimatorMaterialName = mConfig["dicer"]["bc_material"].as<std::string>(); }
        if(mConfig["dicer"]["bc_entrance"])    { mBinocularCollimatorEntrance = mConfig["dicer"]["bc_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["bc_width"])       { mBinocularCollimatorWidth = mConfig["dicer"]["bc_width"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_height"])      { mBinocularCollimatorHeight = mConfig["dicer"]["bc_height"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_length"])      { mBinocularCollimatorLength = mConfig["dicer"]["bc_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_hole_size"])   { mBinocularCollimatorHoleSize = mConfig["dicer"]["bc_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["bc_hole_separation"])   { mBinocularCollimatorHoleSeparation = mConfig["dicer"]["bc_hole_separation"].as<G4double>() * cm; }

        if(mConfig["dicer"]["construct_as"])   { mConstructAperatureStop = mConfig["dicer"]["construct_as"].as<G4bool>(); }
        if(mConfig["dicer"]["as_material"])    { mAperatureStopMaterialName = mConfig["dicer"]["as_material"].as<std::string>(); }
        if(mConfig["dicer"]["as_entrance"])    { mAperatureStopEntrance = mConfig["dicer"]["as_entrance"].as<G4double>() * m; }
        if(mConfig["dicer"]["as_width"])       { mAperatureStopWidth = mConfig["dicer"]["as_width"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_height"])      { mAperatureStopHeight = mConfig["dicer"]["as_height"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_length"])      { mAperatureStopLength = mConfig["dicer"]["as_length"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_hole_size"])   { mAperatureStopHoleSize = mConfig["dicer"]["as_hole_size"].as<G4double>() * cm; }
        if(mConfig["dicer"]["as_hole_separation"])   { mAperatureStopHoleSeparation = mConfig["dicer"]["as_hole_separation"].as<G4double>() * cm; }

        if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void DICER::DefineMaterials()
    {
        mRotatingBeamBlockerMaterial = CreateMaterial(mRotatingBeamBlockerMaterialName, "RotatingBeamBlocker");
        mRotatingBeamBlockerLeftHoleMaterial = CreateMaterial(mWorldMaterialName, "RotatingBeamBlockerLeftHole");
        mRotatingBeamBlockerRightHoleMaterial = CreateMaterial(mWorldMaterialName, "RotatingBeamBlockerRightHole");

        mBinocularCollimatorMaterial = CreateMaterial(mBinocularCollimatorMaterialName, "BinocularCollimator");
        mBinocularCollimatorLeftHoleMaterial = CreateMaterial(mWorldMaterialName, "BinocularCollimatorLeftHole");
        mBinocularCollimatorRightHoleMaterial = CreateMaterial(mWorldMaterialName, "BinocularCollimatorRightHole");
        
        mAperatureStopMaterial = CreateMaterial(mAperatureStopMaterialName, "AperatureStop");
        mAperatureStopLeftHoleMaterial = CreateMaterial(mWorldMaterialName, "AperatureStopLeftHole");
        mAperatureStopRightHoleMaterial = CreateMaterial(mWorldMaterialName, "AperatureStopRightHole");
    }

    DICER::~DICER()
    {
    }

    void DICER::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();

        if(mConstructRotatingBeamBlocker)
        {
            // RBB
            mSolidRotatingBeamBlocker = new G4Tubs(
                "Solid_ArtieIIRotatingBeamBlocker", 
                0.0,
                mRotatingBeamBlockerDiameter / 2.0, 
                0.5 * mRotatingBeamBlockerLength, 
                0,
                2*M_PI
            );
            mLogicalRotatingBeamBlocker = new G4LogicalVolume(
                mSolidRotatingBeamBlocker, 
                mRotatingBeamBlockerMaterial, 
                "Logical_ArtieIIRotatingBeamBlocker"
            );
            mPhysicalRotatingBeamBlocker = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0.,
                    (mRotatingBeamBlockerEntrance + 0.5 * mRotatingBeamBlockerLength)
                ),
                mLogicalRotatingBeamBlocker, 
                "Physical_ArtieIIRotatingBeamBlocker", 
                logicalWorld, 
                false, 
                0, 
                true
            );
            // left hole
            mSolidRotatingBeamBlockerLeftHole = new G4Tubs(
                "Solid_ArtieIIRotatingBeamBlockerLeftHole", 
                0.0,
                mRotatingBeamBlockerHoleSize / 2.0, 
                0.5 * mRotatingBeamBlockerLength, 
                0,
                2*M_PI
            );
            mLogicalRotatingBeamBlockerLeftHole = new G4LogicalVolume(
                mSolidRotatingBeamBlockerLeftHole, 
                mRotatingBeamBlockerLeftHoleMaterial, 
                "Logical_ArtieIIRotatingBeamBlockerLeftHole"
            );
            mPhysicalRotatingBeamBlockerLeftHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    -0.5 * mRotatingBeamBlockerHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalRotatingBeamBlockerLeftHole, 
                "Physical_ArtieIIRotatingBeamBlockerLeftHole", 
                mLogicalRotatingBeamBlocker, 
                false, 
                0, 
                true
            );
            // right hole
            mSolidRotatingBeamBlockerRightHole = new G4Tubs(
                "Solid_ArtieIIRotatingBeamBlockerRightHole", 
                0.0,
                mRotatingBeamBlockerHoleSize / 2.0, 
                0.5 * mRotatingBeamBlockerLength, 
                0,
                2*M_PI
            );
            mLogicalRotatingBeamBlockerRightHole = new G4LogicalVolume(
                mSolidRotatingBeamBlockerRightHole, 
                mRotatingBeamBlockerRightHoleMaterial, 
                "Logical_ArtieIIRotatingBeamBlockerRightHole"
            );
            mPhysicalRotatingBeamBlockerRightHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.5 * mRotatingBeamBlockerHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalRotatingBeamBlockerRightHole, 
                "Physical_ArtieIIRotatingBeamBlockerRightHole", 
                mLogicalRotatingBeamBlocker, 
                false, 
                0, 
                true
            );
        }

        if(mConstructBinocularCollimator)
        {
            // RBB
            mSolidBinocularCollimator = new G4Box(
                "Solid_ArtieIIBinocularCollimator", 
                mBinocularCollimatorWidth / 2.0,
                mBinocularCollimatorHeight / 2.0,
                mBinocularCollimatorLength / 2.0
            );
            mLogicalBinocularCollimator = new G4LogicalVolume(
                mSolidBinocularCollimator, 
                mBinocularCollimatorMaterial, 
                "Logical_ArtieIIBinocularCollimator"
            );
            mPhysicalBinocularCollimator = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0.,
                    (mBinocularCollimatorEntrance + 0.5 * mBinocularCollimatorLength)
                ),
                mLogicalBinocularCollimator, 
                "Physical_ArtieIIBinocularCollimator", 
                logicalWorld, 
                false, 
                0, 
                true
            );
            // left hole
            mSolidBinocularCollimatorLeftHole = new G4Tubs(
                "Solid_ArtieIIBinocularCollimatorLeftHole", 
                0.0,
                mBinocularCollimatorHoleSize / 2.0, 
                0.5 * mBinocularCollimatorLength, 
                0,
                2*M_PI
            );
            mLogicalBinocularCollimatorLeftHole = new G4LogicalVolume(
                mSolidBinocularCollimatorLeftHole, 
                mBinocularCollimatorLeftHoleMaterial, 
                "Logical_ArtieIIBinocularCollimatorLeftHole"
            );
            mPhysicalBinocularCollimatorLeftHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    -0.5 * mBinocularCollimatorHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalBinocularCollimatorLeftHole, 
                "Physical_ArtieIIBinocularCollimatorLeftHole", 
                mLogicalBinocularCollimator, 
                false, 
                0, 
                true
            );
            // right hole
            mSolidBinocularCollimatorRightHole = new G4Tubs(
                "Solid_ArtieIIBinocularCollimatorRightHole", 
                0.0,
                mBinocularCollimatorHoleSize / 2.0, 
                0.5 * mBinocularCollimatorLength, 
                0,
                2*M_PI
            );
            mLogicalBinocularCollimatorRightHole = new G4LogicalVolume(
                mSolidBinocularCollimatorRightHole, 
                mBinocularCollimatorRightHoleMaterial, 
                "Logical_ArtieIIBinocularCollimatorRightHole"
            );
            mPhysicalBinocularCollimatorRightHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.5 * mBinocularCollimatorHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalBinocularCollimatorRightHole, 
                "Physical_ArtieIIBinocularCollimatorRightHole", 
                mLogicalBinocularCollimator, 
                false, 
                0, 
                true
            );
        }

        if(mConstructAperatureStop)
        {
            // RBB
            mSolidAperatureStop = new G4Box(
                "Solid_ArtieIIAperatureStop", 
                mAperatureStopWidth / 2.0,
                mAperatureStopHeight / 2.0,
                mAperatureStopLength / 2.0
            );
            mLogicalAperatureStop = new G4LogicalVolume(
                mSolidAperatureStop, 
                mAperatureStopMaterial, 
                "Logical_ArtieIIAperatureStop"
            );
            mPhysicalAperatureStop = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0.,
                    (mAperatureStopEntrance + 0.5 * mAperatureStopLength)
                ),
                mLogicalAperatureStop, 
                "Physical_ArtieIIAperatureStop", 
                logicalWorld, 
                false, 
                0, 
                true
            );
            // left hole
            mSolidAperatureStopLeftHole = new G4Tubs(
                "Solid_ArtieIIAperatureStopLeftHole", 
                0.0,
                mAperatureStopHoleSize / 2.0, 
                0.5 * mAperatureStopLength, 
                0,
                2*M_PI
            );
            mLogicalAperatureStopLeftHole = new G4LogicalVolume(
                mSolidAperatureStopLeftHole, 
                mAperatureStopLeftHoleMaterial, 
                "Logical_ArtieIIAperatureStopLeftHole"
            );
            mPhysicalAperatureStopLeftHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    -0.5 * mAperatureStopHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalAperatureStopLeftHole, 
                "Physical_ArtieIIAperatureStopLeftHole", 
                mLogicalAperatureStop, 
                false, 
                0, 
                true
            );
            // right hole
            mSolidAperatureStopRightHole = new G4Tubs(
                "Solid_ArtieIIAperatureStopRightHole", 
                0.0,
                mAperatureStopHoleSize / 2.0, 
                0.5 * mAperatureStopLength, 
                0,
                2*M_PI
            );
            mLogicalAperatureStopRightHole = new G4LogicalVolume(
                mSolidAperatureStopRightHole, 
                mAperatureStopRightHoleMaterial, 
                "Logical_ArtieIIAperatureStopRightHole"
            );
            mPhysicalAperatureStopRightHole = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.5 * mAperatureStopHoleSeparation,
                    0.,
                    0.
                ),
                mLogicalAperatureStopRightHole, 
                "Physical_ArtieIIAperatureStopRightHole", 
                mLogicalAperatureStop, 
                false, 
                0, 
                true
            );
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
        //         logicalWorld, 
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
        //         logicalWorld, 
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
        //         logicalWorld, 
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
        //         logicalWorld, 
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
        //         logicalWorld, 
        //         false, 
        //         0, 
        //         true
        //     );
        // } 
    }
}