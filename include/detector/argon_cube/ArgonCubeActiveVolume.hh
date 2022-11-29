/**
 * @file ArgonCubeActiveVolume.hh
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
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Argon.hh"
#include "DetectorComponent.hh"

namespace Artie
{
    class ArgonCubeActiveVolume : public DetectorComponent
    {
    public:
        ArgonCubeActiveVolume(
            G4double CubeX,
            G4double CubeY,
            G4double CubeZ
        );
        ~ArgonCubeActiveVolume();

        void Construct();

    private:
        G4double mCubeX = {50 * m};
        G4double mCubeY = {50 * m};
        G4double mCubeZ = {50 * m};

        std::shared_ptr<Argon> mArgon = {nullptr};

    };
}