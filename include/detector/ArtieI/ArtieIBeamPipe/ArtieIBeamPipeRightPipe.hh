/**
 * @file ArtieIBeamPipeRightPipe.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Vacuum.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieIBeamPipeRightPipe : public DetectorComponent
    {
    public:
        ArtieIBeamPipeRightPipe(
            G4double BeamPipeInnerRadius,
            G4double BeamPipeOuterRadius,
            G4double BeamPipeRightHalfLength,
            G4ThreeVector BeamPipeRightPosition
        );
        ~ArtieIBeamPipeRightPipe();

        void Construct();

    private:
        G4double mBeamPipeInnerRadius = {18.0 * cm};
        G4double mBeamPipeOuterRadius = {20.0 * cm};
        G4double mBeamPipeRightHalfLength = {1.68 * m};
        G4ThreeVector mBeamPipeRightPosition = {0.,0.,0.};

        std::shared_ptr<Vacuum> mVacuum = {nullptr};

    };
}