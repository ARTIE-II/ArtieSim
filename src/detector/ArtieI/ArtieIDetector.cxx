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
        G4double WorldX,
        G4double WorldY,
        G4double WorldZ,
        G4double WallThickness,
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
    , mWorldX(WorldX)
    , mWorldY(WorldY)
    , mWorldZ(WorldZ)
    , mWallThickness(WallThickness)
    , mDetectorRadius(DetectorRadius)
    , mDetectorLength(DetectorLength)
    , mDetectorEntrance(DetectorEntrance)
    {
        ConstructDetector();
        ConstructMessengers();
    }

#ifdef ARTIE_YAML
    ArtieIDetector::ArtieIDetector(YAML::Node config)
    :Detector(config)
    {
        if(Config()["target_radius"]) { mTargetRadius = Config()["target_radius"].as<G4double>() * cm; }
        if(Config()["target_length"]) { mTargetLength = Config()["target_length"].as<G4double>() * cm; }
        if(Config()["construct_container"])  { mConstructContainer = Config()["construct_container"].as<G4bool>(); }
        if(Config()["container_radius"])     { mContainerRadius = Config()["container_radius"].as<G4double>() * cm; }
        if(Config()["insulation_thickness"]) { mInsulationThickness = Config()["insulation_thickness"].as<G4double>() * cm; }
        if(Config()["window_thickness"])     { mWindowThickness = Config()["window_thickness"].as<G4double>() * cm; }
        if(Config()["buffer_length"])        { mBufferLength = Config()["buffer_length"].as<G4double>() * cm; }
        if(Config()["gap"])     { mGap = Config()["gap"].as<G4double>() * m; }
        if(Config()["world_x"]) { mWorldX = Config()["world_x"].as<G4double>() * m; }
        if(Config()["world_y"]) { mWorldY = Config()["world_y"].as<G4double>() * m; }
        if(Config()["world_z"]) { mWorldZ = Config()["world_z"].as<G4double>() * m; }
        if(Config()["wall_thickness"]) { mWallThickness = Config()["wall_thickness"].as<G4double>() * m; }
        if(Config()["detector_radius"]) { mDetectorRadius = Config()["detector_radius"].as<G4double>() * cm; }
        if(Config()["detector_length"]) { mDetectorLength = Config()["detector_length"].as<G4double>() * cm; }
        if(Config()["detector_entrance"]) { mDetectorEntrance = Config()["detector_entrance"].as<G4double>() * m; }
        ConstructDetector();
        ConstructMessengers();    
    }
#endif

    void ArtieIDetector::ConstructDetector()
    {
        // create the target
        std::shared_ptr<ArtieITargetActiveVolume> ActiveVolume 
            = std::make_shared<ArtieITargetActiveVolume>(
            mTargetRadius, 
            mTargetLength
        );
        AddDetectorComponent(ActiveVolume);

        if(mConstructContainer)
        {
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
        }

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

        // add the hall
        std::shared_ptr<ArtieIHall> Hall
            = std::make_shared<ArtieIHall>(
            mWorldX,
            mWorldY,
            mWorldZ,
            mWallThickness
        );
        AddDetectorComponent(Hall);
    }

    ArtieIDetector::~ArtieIDetector()
    {
    }

    void ArtieIDetector::ConstructMessengers()
    {
        // Target
        mTargetRadiusMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/target_radius",
                this
            )
        );
        mTargetRadiusMessenger->SetGuidance(
            "Specify the radius of the target in [cm]."
        );
        mTargetRadiusMessenger->SetParameterName(
            "radius", false
        );

        mTargetLengthMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/target_length",
                this
            )
        );
        mTargetLengthMessenger->SetGuidance(
            "Specify the length of the target in [cm]."
        );
        mTargetLengthMessenger->SetParameterName(
            "length", false
        );

        // Container
        mConstructContainerMessenger.reset(
            new G4UIcmdWithABool(
                "/artie/detector/construct_container",
                this
            )
        );
        mConstructContainerMessenger->SetGuidance(
            "Specify whether to construct the container."
        );
        mConstructContainerMessenger->SetParameterName(
            "yes/no", false
        );

        mContainerRadiusMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/container_radius",
                this
            )
        );
        mContainerRadiusMessenger->SetGuidance(
            "Specify the radius of the container [cm]"
        );
        mContainerRadiusMessenger->SetParameterName(
            "radius", false
        );

        mInsulationThicknessMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/insulation_thickness",
                this
            )
        );
        mInsulationThicknessMessenger->SetGuidance(
            "Specify the insulation thickness in [cm]."
        );
        mInsulationThicknessMessenger->SetParameterName(
            "thickness", false
        );

        mWindowThicknessMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/window_thickness",
                this
            )
        );
        mWindowThicknessMessenger->SetGuidance(
            "Specify the thickness of the kapton window [cm]."
        );
        mWindowThicknessMessenger->SetParameterName(
            "thickness", false
        );

        mBufferLengthMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/buffer_length",
                this
            )
        );
        mBufferLengthMessenger->SetGuidance(
            "Specify the length of the buffer [cm]."
        );
        mBufferLengthMessenger->SetParameterName(
            "length", false
        );

        // Beam pipe
        mBeamPipeInnerRadiusMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/beam_pipe_inner_radius",
                this
            )
        );
        mBeamPipeInnerRadiusMessenger->SetGuidance(
            "Specify the inner radius of the beam pipe [cm]."
        );
        mBeamPipeInnerRadiusMessenger->SetParameterName(
            "radius", false
        );

        mBeamPipeOuterRadiusMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/beam_pipe_outer_radius",
                this
            )
        );
        mBeamPipeOuterRadiusMessenger->SetGuidance(
            "Specify the outer radius of the beam pipe [cm]."
        );
        mBeamPipeOuterRadiusMessenger->SetParameterName(
            "radius", false
        );

        mBeamPipeLeftHalfLengthMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/beam_pipe_left_half_length",
                this
            )
        );
        mBeamPipeLeftHalfLengthMessenger->SetGuidance(
            "Specify the left half length of the beam pipe [cm]."
        );
        mBeamPipeLeftHalfLengthMessenger->SetParameterName(
            "length", false
        );

        mBeamPipeRightHalfLengthMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/beam_pipe_right_half_length",
                this
            )
        );
        mBeamPipeRightHalfLengthMessenger->SetGuidance(
            "Specify the right half length of the beam pipe [cm]."
        );
        mBeamPipeRightHalfLengthMessenger->SetParameterName(
            "length", false
        );

        mBeamPipeLeftPositionMessenger.reset(
            new G4UIcmdWith3Vector(
                "/artie/detector/beam_pipe_left_position",
                this
            )
        );
        mBeamPipeLeftPositionMessenger->SetGuidance(
            "Specify the position of the left beam pipe [cm] [cm] [cm]."
        );
        mBeamPipeLeftPositionMessenger->SetParameterName(
            "x", "y", "z", false
        );

        mBeamPipeRightPositionMessenger.reset(
            new G4UIcmdWith3Vector(
                "/artie/detector/beam_pipe_right_position",
                this
            )
        );
        mBeamPipeRightPositionMessenger->SetGuidance(
            "Specify the position of the right beam pipe [cm] [cm] [cm]."
        );
        mBeamPipeRightPositionMessenger->SetParameterName(
            "x", "y", "z", false
        );

        mGapMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/gap",
                this
            )
        );
        mGapMessenger->SetGuidance(
            "Specify the gap between the beam pipe and the target [cm]"
        );
        mGapMessenger->SetParameterName(
            "length", false
        );

        // Hall
        mWorldXMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/world_x",
                this
            )
        );
        mWorldXMessenger->SetGuidance(
            "Specify the half length of the world x coordinate [cm]."
        );
        mWorldXMessenger->SetParameterName(
            "length", false
        );

        mWorldYMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/world_y",
                this
            )
        );
        mWorldYMessenger->SetGuidance(
            "Specify the half length of the world y coordinate [cm]."
        );
        mWorldYMessenger->SetParameterName(
            "length", false
        );

        mWorldZMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/world_z",
                this
            )
        );
        mWorldZMessenger->SetGuidance(
            "Specify the half length of the world z coordinate [cm]."
        );
        mWorldZMessenger->SetParameterName(
            "length", false
        );

        mWallThicknessMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/wall_thickness",
                this
            )
        );
        mWallThicknessMessenger->SetGuidance(
            "Specify the thickness of the concrete wall [cm]."
        );
        mWallThicknessMessenger->SetParameterName(
            "thickness", false
        );

        // Detector
        mDetectorRadiusMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/detector_radius",
                this
            )
        );
        mDetectorRadiusMessenger->SetGuidance(
            "Specify the radius of the detector [cm]."
        );
        mDetectorRadiusMessenger->SetParameterName(
            "radius", false
        );

        mDetectorLengthMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/detector_length",
                this
            )
        );
        mDetectorLengthMessenger->SetGuidance(
            "Specify the length of the detector [cm]."
        );
        mDetectorLengthMessenger->SetParameterName(
            "length", false
        );

        mDetectorEntranceMessenger.reset(
            new G4UIcmdWithADouble(
                "/artie/detector/detector_entrance",
                this
            )
        );
        mDetectorEntranceMessenger->SetGuidance(
            "Specify the position along the z axis of the detector entrance [cm]."
        );
        mDetectorEntranceMessenger->SetParameterName(
            "position", false
        );

    }

    void ArtieIDetector::SetNewValue(G4UIcommand* command, G4String arg)
    {
        std::istringstream is((const char *) arg);
        if(command == mTargetRadiusMessenger.get())
        {
            G4cout << "Setting target radius to: " << arg << G4endl;
            is >> mTargetRadius;
            return;
        }
        else if(command == mTargetLengthMessenger.get())
        {
            G4cout << "Setting target length to: " << arg << G4endl;
            is >> mTargetLength;
            return;
        }
        else if(command == mConstructContainerMessenger.get())
        {
            G4cout << "Setting construct container to: " << arg << G4endl;
            is >> mConstructContainer;
            return;
        }
        else if(command == mContainerRadiusMessenger.get())
        {
            G4cout << "Setting container radius to: " << arg << G4endl;
            is >> mContainerRadius;
            return;
        }
        else if(command == mInsulationThicknessMessenger.get())
        {
            G4cout << "Setting insulation thickness to: " << arg << G4endl;
            is >> mInsulationThickness;
            return;
        }
        else if(command == mWindowThicknessMessenger.get())
        {
            G4cout << "Setting window thickness to: " << arg << G4endl;
            is >> mWindowThickness;
            return;
        }
        else if(command == mBufferLengthMessenger.get())
        {
            G4cout << "Setting buffer length to: " << arg << G4endl;
            is >> mBufferLength;
            return;
        }
        else if(command == mBeamPipeInnerRadiusMessenger.get())
        {
            G4cout << "Setting beam pipe inner radius to: " << arg << G4endl;
            is >> mBeamPipeInnerRadius;
            return;
        }
        else if(command == mBeamPipeOuterRadiusMessenger.get())
        {
            G4cout << "Setting beam pipe outer radius to: " << arg << G4endl;
            is >> mBeamPipeOuterRadius;
            return;
        }
        else if(command == mBeamPipeLeftHalfLengthMessenger.get())
        {
            G4cout << "Setting beam pipe left half length to: " << arg << G4endl;
            is >> mBeamPipeLeftHalfLength;
            return;
        }
        else if(command == mBeamPipeRightHalfLengthMessenger.get())
        {
            G4cout << "Setting beam pipe right half lengthto: " << arg << G4endl;
            is >> mBeamPipeRightHalfLength;
            return;
        }
        else if(command == mBeamPipeLeftPositionMessenger.get())
        {
            G4cout << "Setting beam pipe left position to: " << arg << G4endl;
            is >> mBeamPipeLeftPosition;
            return;
        }
        else if(command == mBeamPipeRightPositionMessenger.get())
        {
            G4cout << "Setting beam pipe right position to: " << arg << G4endl;
            is >> mBeamPipeRightPosition;
            return;
        }
        else if(command == mGapMessenger.get())
        {
            G4cout << "Setting gap to: " << arg << G4endl;
            is >> mGap;
            return;
        }
        else if(command == mWorldXMessenger.get())
        {
            G4cout << "Setting world x to: " << arg << G4endl;
            is >> mWorldX;
            return;
        }
        else if(command == mWorldYMessenger.get())
        {
            G4cout << "Setting world y to: " << arg << G4endl;
            is >> mWorldY; 
            return;
        }
        else if(command == mWorldZMessenger.get())
        {
            G4cout << "Setting world zto: " << arg << G4endl;
            is >> mWorldZ;
            return;
        }
        else if(command == mWallThicknessMessenger.get())
        {
            G4cout << "Setting wall thickness to: " << arg << G4endl;
            is >> mWallThickness;
            return;
        }
        else if(command == mDetectorRadiusMessenger.get())
        {
            G4cout << "Setting detector radius to: " << arg << G4endl;
            is >> mDetectorRadius;
            return;
        }
        else if(command == mDetectorLengthMessenger.get())
        {
            G4cout << "Setting detector length to: " << arg << G4endl;
            is >> mDetectorLength;
            return;
        }
        else if(command == mDetectorEntranceMessenger.get())
        {
            G4cout << "Setting detector entrance to: " << arg << G4endl;
            is >> mDetectorEntrance;
            return;
        }
        else
        {
            G4cout << "Command not recognized!" << G4endl;
            return;
        }
    }
}