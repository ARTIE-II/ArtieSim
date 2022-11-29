/**
 * @file Detector.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <vector>
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"

#include "SensitiveDetector.hh"

namespace Artie
{
    class SensitiveDetector;
    
    class Detector
    {
    public:
        Detector();
        ~Detector();

        G4int GetNumberOfComponents() { return mDetectorComponents.size(); }
        std::shared_ptr<DetectorComponent> GetDetectorComponent(G4int ii);
        DetectorComponent* GetDetectorComponentPointer(G4int ii);

        void AddDetectorComponent(std::shared_ptr<DetectorComponent>);
        void SetSensitiveDetector(std::shared_ptr<SensitiveDetector>);

        std::shared_ptr<SensitiveDetector> GetSensitiveDetector() { return mSensitiveDetector; }
        SensitiveDetector* GetSensitiveDetectorPointer() { return mSensitiveDetector.get(); }

        virtual void ProcessHits(G4Step*, G4TouchableHistory*) { return; };

    private:
        std::vector<std::shared_ptr<DetectorComponent>> mDetectorComponents;
        std::shared_ptr<SensitiveDetector> mSensitiveDetector;

    };
}