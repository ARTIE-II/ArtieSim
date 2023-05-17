/**
 * @file DICERDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "DICERDetector.hh"

namespace Artie
{
    DICERDetector::DICERDetector()
    {
        DefineMaterials();
    }

#ifdef ARTIE_YAML
    DICERDetector::DICERDetector(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["detector"]["construct_detector"])   { mConstructDetector = mConfig["detector"]["construct_detector"].as<G4bool>(); }
        if(mConfig["detector"]["detector_material"])    { mDetectorMaterialName = mConfig["detector"]["detector_material"].as<std::string>(); }
        if(mConfig["detector"]["detector_radius"])      { mDetectorRadius = mConfig["detector"]["detector_radius"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_length"])      { mDetectorLength = mConfig["detector"]["detector_length"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_entrance"])    { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; } 
        if(mConfig["detector"]["detector_separation"])  { mDetectorSeparation = mConfig["detector"]["detector_separation"].as<G4double>() * cm; }     

        if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void DICERDetector::DefineMaterials()
    {
        mDetectorMaterial = CreateMaterial(mDetectorMaterialName, "Detector");
    }

    DICERDetector::~DICERDetector()
    {
    }

    void DICERDetector::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();
        // Construct the detector
        if(mConstructDetector)
        {
            mSolidLeftDetector = new G4Tubs(
                "Solid_ArtieIILeftDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            );
            mLogicalLeftDetector = new G4LogicalVolume(
                mSolidLeftDetector, 
                mDetectorMaterial, 
                "Logical_ArtieIILeftDetector"
            );
            mPhysicalLeftDetector = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    + mDetectorSeparation * 0.5,
                    0., 
                    + mDetectorEntrance + mDetectorLength * 0.5),
                mLogicalLeftDetector, 
                "Physical_ArtieIILeftDetector", 
                logicalWorld, 
                false, 
                0, 
                true
            );

            mSolidRightDetector = new G4Tubs(
                "Solid_ArtieIIRightDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            );
            mLogicalRightDetector = new G4LogicalVolume(
                mSolidRightDetector, 
                mDetectorMaterial, 
                "Logical_ArtieIIRightDetector"
            );
            mPhysicalRightDetector = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    - mDetectorSeparation * 0.5,
                    0., 
                    + mDetectorEntrance + mDetectorLength * 0.5),
                mLogicalRightDetector, 
                "Physical_ArtieIIRightDetector", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }
    }
}