/**
 * @file ArtieITargetDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieITargetDetector.hh"
#include "ArtieITargetCapDetector.hh"

namespace Artie
{
    ArtieITargetDetector::ArtieITargetDetector(
        G4double TargetR,
        G4double TargetL,
        G4double CapR,
        G4double CapL,
        G4ThreeVector CapPosition
    )
    : Detector()
    , mTargetR(TargetR)
    , mTargetL(TargetL)
    , mCapR(CapR)
    , mCapL(CapL)
    , mCapPosition(CapPosition)
    {
        /// create the argon cube active volume
        std::shared_ptr<ArtieITargetActiveVolume> ActiveVolume 
            = std::make_shared<ArtieITargetActiveVolume>(
            mTargetR, 
            mTargetL
        );
        AddDetectorComponent(ActiveVolume);

        std::shared_ptr<ArtieITargetCapDetector> Cap = std::make_shared<ArtieITargetCapDetector>(
            mCapR, mCapL, mCapPosition
        );
        AddDetectorComponent(Cap);
    }

    ArtieITargetDetector::~ArtieITargetDetector()
    {
    }
}