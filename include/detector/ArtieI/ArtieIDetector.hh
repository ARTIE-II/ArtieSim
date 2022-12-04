/**
 * @file ArtieIDetector.hh
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

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Detector.hh"
#include "ArtieITargetActiveVolume.hh"
#include "ArtieITargetContainer.hh"
#include "ArtieITargetInsulation.hh"
#include "ArtieITargetLeftWindow.hh"
#include "ArtieITargetRightWindow.hh"
#include "ArtieITargetLeftBuffer.hh"
#include "ArtieITargetRightBuffer.hh"
#include "ArtieIBeamPipeLeftBeam.hh"
#include "ArtieIBeamPipeRightBeam.hh"
#include "ArtieIBeamPipeLeftPipe.hh"
#include "ArtieIBeamPipeRightPipe.hh"
#include "ArtieITargetDetector.hh"
#include "ArtieIHall.hh"

namespace Artie
{
    class ArtieIDetector : public Detector
    {
    public:
        ArtieIDetector(
            G4double TargetRadius,
            G4double TargetLength,
            G4double ContainerRadius,
            G4double InsulationThickness,
            G4double WindowThickness,
            G4double BufferLength,
            G4double BeamPipeInnerRadius,
            G4double BeamPipeOuterRadius,
            G4double Gap,
            G4double WorldX,
            G4double WorldY,
            G4double WorldZ,
            G4double WallThickness,
            G4double DetectorRadius,
            G4double DetectorLength,
            G4double DetectorEntrance
        );
        ~ArtieIDetector();

#ifdef ARTIE_YAML
        ArtieIDetector(YAML::Node config);
#endif

        void ConstructDetector();
        void ConstructMessengers();
        void SetNewValue(G4UIcommand* command, G4String arg);

    private:
        // Artie-I target
        G4double mTargetRadius = {2.5 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};

        // Container
        G4bool mConstructContainer = {true};
        G4double mContainerRadius = {3.49 / 2.0 * cm};
        G4double mInsulationThickness = {10.0 * cm};
        G4double mWindowThickness = {0.00762 * cm};
        G4double mBufferLength = {5.0 * cm};

        // Beam pipes
        G4double mBeamPipeInnerRadius = {18.0 * cm};
        G4double mBeamPipeOuterRadius = {20.0 * cm};
        G4double mBeamPipeLeftHalfLength = 0.;
        G4double mBeamPipeRightHalfLength = 0.;
        G4ThreeVector mBeamPipeLeftPosition = {0., 0., 0.};
        G4ThreeVector mBeamPipeRightPosition = {0., 0., 0.};
        G4double mGap = {2.5 * m};

        // Hall
        G4double mWorldX = {4 * m};
        G4double mWorldY = {4 * m};
        G4double mWorldZ = {200 * m};
        G4double mWallThickness = {1 * m};

        // Detector
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {69.0 * m};

        // messengers
        std::shared_ptr<G4UIcmdWithADouble> mTargetRadiusMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mTargetLengthMessenger;
        
        std::shared_ptr<G4UIcmdWithABool> mConstructContainerMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mContainerRadiusMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mInsulationThicknessMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mWindowThicknessMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mBufferLengthMessenger;

        std::shared_ptr<G4UIcmdWithADouble> mBeamPipeInnerRadiusMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mBeamPipeOuterRadiusMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mBeamPipeLeftHalfLengthMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mBeamPipeRightHalfLengthMessenger;
        std::shared_ptr<G4UIcmdWith3Vector> mBeamPipeLeftPositionMessenger;
        std::shared_ptr<G4UIcmdWith3Vector> mBeamPipeRightPositionMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mGapMessenger;

        std::shared_ptr<G4UIcmdWithADouble> mWorldXMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mWorldYMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mWorldZMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mWallThicknessMessenger;

        std::shared_ptr<G4UIcmdWithADouble> mDetectorRadiusMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mDetectorLengthMessenger;
        std::shared_ptr<G4UIcmdWithADouble> mDetectorEntranceMessenger;

    };
}