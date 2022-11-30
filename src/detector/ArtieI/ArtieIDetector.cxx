/**
 * @file ArtieIDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIDetector.hh"

namespace Artie
{
    ArtieIDetector::ArtieIDetector(
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
    )
    : Detector()
    , mTargetRadius(TargetRadius)
    , mTargetLength(TargetLength)
    , mContainerRadius(ContainerRadius)
    , mInsulationThickness(InsulationThickness)
    , mWindowThickness(WindowThickness)
    , mBufferLength(BufferLength)
    , mBeamPipeInnerRadius(BeamPipeInnerRadius)
    , mBeamPipeOuterRadius(BeamPipeOuterRadius)
    , mGap(Gap)
    , mWorldZ(WorldZ)
    , mDetectorRadius(DetectorRadius)
    , mDetectorLength(DetectorLength)
    , mDetectorEntrance(DetectorEntrance)
    {
        // create the target
        std::shared_ptr<ArtieITargetActiveVolume> ActiveVolume 
            = std::make_shared<ArtieITargetActiveVolume>(
            mTargetRadius, 
            mTargetLength
        );
        AddDetectorComponent(ActiveVolume);

        std::shared_ptr<ArtieITargetContainer> Container 
            = std::make_shared<ArtieITargetContainer>(
            mTargetRadius, 
            mTargetLength,
            mContainerRadius
        );
        AddDetectorComponent(Container);

        std::shared_ptr<ArtieITargetInsulation> Insulation 
            = std::make_shared<ArtieITargetInsulation>(
            mTargetLength, 
            mContainerRadius,
            mInsulationThickness
        );
        AddDetectorComponent(Insulation);

        std::shared_ptr<ArtieITargetLeftWindow> LeftWindow 
            = std::make_shared<ArtieITargetLeftWindow>(
            mTargetLength,
            mContainerRadius,
            mWindowThickness
        );
        AddDetectorComponent(LeftWindow);
        std::shared_ptr<ArtieITargetRightWindow> RightWindow 
            = std::make_shared<ArtieITargetRightWindow>(
            mTargetLength,
            mContainerRadius,
            mWindowThickness
        );
        AddDetectorComponent(RightWindow);

        std::shared_ptr<ArtieITargetLeftBuffer> LeftBuffer 
            = std::make_shared<ArtieITargetLeftBuffer>(
            mTargetRadius, 
            mTargetLength,
            mWindowThickness,
            mBufferLength
        );
        AddDetectorComponent(LeftBuffer);
        std::shared_ptr<ArtieITargetRightBuffer> RightBuffer 
            = std::make_shared<ArtieITargetRightBuffer>(
            mTargetRadius, 
            mTargetLength,
            mWindowThickness,
            mBufferLength
        );
        AddDetectorComponent(RightBuffer);

        // create the beampipe
        mBeamPipeLeftHalfLength = (-mGap / 2.0 + mWorldZ / 2.0) / 2.0;
        mBeamPipeRightHalfLength = (mDetectorEntrance - mGap / 2.0) / 2.0;
        mBeamPipeLeftPosition = {
            0., 
            0., 
            (-mWorldZ / 2.0 - mGap / 2.0) / 2.0
        };
        mBeamPipeRightPosition = {
            0., 
            0., 
            (mDetectorEntrance + mGap / 2.0) / 2.0
        };
        std::shared_ptr<ArtieIBeamPipeLeftBeam> BeamPipeLeftBeam 
            = std::make_shared<ArtieIBeamPipeLeftBeam>(
            mBeamPipeInnerRadius, 
            mBeamPipeLeftHalfLength,
            mBeamPipeLeftPosition
        );
        AddDetectorComponent(BeamPipeLeftBeam);
        std::shared_ptr<ArtieIBeamPipeRightBeam> BeamPipeRightBeam
            = std::make_shared<ArtieIBeamPipeRightBeam>(
            mBeamPipeInnerRadius, 
            mBeamPipeRightHalfLength,
            mBeamPipeRightPosition
        );
        AddDetectorComponent(BeamPipeRightBeam);

        std::shared_ptr<ArtieIBeamPipeLeftPipe> BeamPipeLeftPipe
            = std::make_shared<ArtieIBeamPipeLeftPipe>(
            mBeamPipeInnerRadius, 
            mBeamPipeOuterRadius,
            mBeamPipeLeftHalfLength,
            mBeamPipeLeftPosition
        );
        AddDetectorComponent(BeamPipeLeftPipe);
        std::shared_ptr<ArtieIBeamPipeRightPipe> BeamPipeRightPipe 
            = std::make_shared<ArtieIBeamPipeRightPipe>(
            mBeamPipeInnerRadius, 
            mBeamPipeOuterRadius,
            mBeamPipeRightHalfLength,
            mBeamPipeRightPosition
        );
        AddDetectorComponent(BeamPipeRightPipe);

        // construct the detector
        std::shared_ptr<ArtieITargetDetector> TargetDetector
            = std::make_shared<ArtieITargetDetector>(
            mDetectorRadius, 
            mDetectorLength,
            mDetectorEntrance
        );
        AddDetectorComponent(TargetDetector);

    }

    ArtieIDetector::~ArtieIDetector()
    {
    }
}