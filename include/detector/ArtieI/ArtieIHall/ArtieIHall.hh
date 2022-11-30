/**
 * @file ArtieIHall.hh
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
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"

#include "Concrete.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArtieIHall : public DetectorComponent
    {
    public:
        ArtieIHall(
            G4double WallOuterX,
            G4double WallOuterY,
            G4double WallOuterZ,
            G4double WallThickness
        );
        ~ArtieIHall();

        void Construct();

    private:
        G4double mWallOuterX = {4 * m};
        G4double mWallOuterY = {4 * m};
        G4double mWallOuterZ = {200 * m};
        G4double mWallThickness = {1 * m};

        std::shared_ptr<Concrete> mConcrete = {nullptr};

    };
}