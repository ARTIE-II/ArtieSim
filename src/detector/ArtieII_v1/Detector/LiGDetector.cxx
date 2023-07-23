/**
 * @file LiGDetector.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "LiGDetector.hh"

namespace Artie
{
    LiGDetector::LiGDetector()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    LiGDetector::LiGDetector(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["detector"]["construct_detector"])   { mConstructDetector = mConfig["detector"]["construct_detector"].as<G4bool>(); }
        if(mConfig["detector"]["detector_material"])    { mDetectorMaterialName = mConfig["detector"]["detector_material"].as<std::string>(); }
        if(mConfig["detector"]["detector_entrance"])    { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }
        if(mConfig["detector"]["cube"]) {
            if(mConfig["detector"]["cube"]["detector_width"])       { mDetectorWidth = mConfig["detector"]["cube"]["detector_width"].as<G4double>() * cm; }
            if(mConfig["detector"]["cube"]["detector_height"])      { mDetectorHeight = mConfig["detector"]["cube"]["detector_height"].as<G4double>() * cm; }
            if(mConfig["detector"]["cube"]["detector_length"])      { mDetectorLength = mConfig["detector"]["cube"]["detector_length"].as<G4double>() * cm; }
        }
        if(mConfig["detector"]["disk"]) {
            if(mConfig["detector"]["disk"]["detector_radius"])      { mDetectorRadius = mConfig["detector"]["disk"]["detector_radius"].as<G4double>() * cm; }
            if(mConfig["detector"]["disk"]["detector_length"])      { mDetectorLength = mConfig["detector"]["disk"]["detector_length"].as<G4double>() * cm; }
        }
        if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void LiGDetector::DefineMaterials()
    {
        mDetectorMaterial = CreateMaterial(mDetectorMaterialName, "Detector");
    }

    LiGDetector::~LiGDetector()
    {
    }

    void LiGDetector::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();
        // Construct the detector
        if(mConstructDetector)
        {
            if(mConfig["detector"]["disk"]) {
                mSolidDetectorDisk = new G4Tubs(
                    "Solid_MArEXDetector", 
                    0,
                    mDetectorRadius, 
                    0.5 * mDetectorLength, 
                    0,
                    2*M_PI
                );
                mLogicalDetector = new G4LogicalVolume(
                    mSolidDetectorDisk, 
                    mDetectorMaterial, 
                    "Logical_MArEXDetector"
                );
                mPhysicalDetector = new G4PVPlacement(
                    0, 
                    G4ThreeVector(
                        0.,
                        0., 
                        + mDetectorEntrance + mDetectorLength * 0.5),
                    mLogicalDetector, 
                    "Physical_MArEXDetector", 
                    logicalWorld, 
                    false, 
                    0, 
                    true
                );
            }

            if(mConfig["detector"]["cube"]) {
                mSolidDetectorCube = new G4Box(
                    "Solid_MArEXDetector",
                    mDetectorWidth / 2.0,
                    mDetectorHeight / 2.0,
                    mDetectorLength / 2.0
                );
                mLogicalDetector = new G4LogicalVolume(
                    mSolidDetectorCube, 
                    mDetectorMaterial, 
                    "Logical_MArEXDetector"
                );
                mPhysicalDetector = new G4PVPlacement(
                    0, 
                    G4ThreeVector(
                        0.,
                        0., 
                        + mDetectorEntrance + mDetectorLength * 0.5),
                    mLogicalDetector, 
                    "Physical_MArEXDetector", 
                    logicalWorld, 
                    false, 
                    0, 
                    true
                );
            }
        }
    }
}