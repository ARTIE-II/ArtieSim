/**
 * @file Detector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "Detector.hh"

namespace Artie
{
    Detector::Detector()
    {
    }

    Detector::~Detector()
    {
    }

    std::shared_ptr<DetectorComponent> Detector::GetDetectorComponent(G4int ii)
    {
        if (ii < mDetectorComponents.size()) {
            return mDetectorComponents[ii];
        }
        else {
            return nullptr;
        }
    }

    DetectorComponent* Detector::GetDetectorComponentPointer(G4int ii)
    {
        if (ii < mDetectorComponents.size()) {
            return mDetectorComponents[ii].get();
        }
        else {
            return nullptr;
        }
    }

    void Detector::AddDetectorComponent(std::shared_ptr<DetectorComponent> component)
    {
        mDetectorComponents.push_back(component);
    }
    void Detector::SetSensitiveDetector(std::shared_ptr<SensitiveDetector> sensitive)
    {
        mSensitiveDetector = sensitive;
    }
}