/**
 * @file ArgonCubeDetector.hh
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
#include "ArgonCubeActiveVolume.hh"
#include "ArgonCubeXZSiPM.hh"

namespace Artie
{
    class ArgonCubeDetector : public Detector
    {
    public:
        ArgonCubeDetector(
            G4double CubeX,
            G4double CubeY,
            G4double CubeZ,
            G4int BottomNumberX,
            G4int BottomNumberZ,
            G4double BottomThicknessY
        );
        ~ArgonCubeDetector();

    private:
        G4double mCubeX = {50 * m};
        G4double mCubeY = {50 * m};
        G4double mCubeZ = {50 * m};
        G4int mBottomNumberX = {8};
        G4int mBottomNumberZ = {8};
        G4double mBottomThicknessY = {5};
    };
}