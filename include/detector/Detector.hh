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
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3Vector.hh"

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "SensitiveDetector.hh"

namespace Artie
{
    class SensitiveDetector;
    
    class Detector : public G4UImessenger
    {
    public:
        Detector();
        ~Detector();

#ifdef ARTIE_YAML
        Detector(YAML::Node config);
        YAML::Node Config() const { return mConfig; }
#endif

        G4int GetNumberOfComponents() { return mDetectorComponents.size(); }
        std::shared_ptr<DetectorComponent> GetDetectorComponent(G4int ii);
        DetectorComponent* GetDetectorComponentPointer(G4int ii);

        void AddDetectorComponent(std::shared_ptr<DetectorComponent>);
        void SetSensitiveDetector(std::shared_ptr<SensitiveDetector>);

        std::shared_ptr<SensitiveDetector> GetSensitiveDetector() { return mSensitiveDetector; }
        SensitiveDetector* GetSensitiveDetectorPointer() { return mSensitiveDetector.get(); }

        virtual void ProcessHits(G4Step*, G4TouchableHistory*) { return; }

        virtual void SetNewValue(G4UIcommand* command, G4String arg) { return; }

    private:
        std::vector<std::shared_ptr<DetectorComponent>> mDetectorComponents;
        std::shared_ptr<SensitiveDetector> mSensitiveDetector;

        // UI messenger parameters
        G4bool mBroadcast = {false};
        G4String mUIDirectory = {"/artie/detector/"};
#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}