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
            G4double WorldZ,
            G4double DetectorRadius,
            G4double DetectorLength,
            G4double DetectorEntrance
        );
        ~ArtieIDetector();

    private:
        G4double mTargetRadius = {2.5 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};
        G4double mContainerRadius = {3.49 / 2.0 * cm};
        G4double mInsulationThickness = {10.0 * cm};
        G4double mWindowThickness = {0.00762 * cm};
        G4double mBufferLength = {5.0 * cm};
        G4double mBeamPipeInnerRadius = {18.0 * cm};
        G4double mBeamPipeOuterRadius = {20.0 * cm};
        G4double mGap = {2.5 * m};
        G4double mWorldZ = {200 * m};
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {69.0 * m};
        
        G4double mBeamPipeLeftHalfLength = 0.;
        G4double mBeamPipeRightHalfLength = 0.;
        G4ThreeVector mBeamPipeLeftPosition = {0., 0., 0.};
        G4ThreeVector mBeamPipeRightPosition = {0., 0., 0.};

        G4double mCapR = {.025 * m};
        G4double mCapL = {.050 * m};
        G4ThreeVector mCapPosition = {0.,0.,(1.68/2 + 0.025) * m};
    };
}