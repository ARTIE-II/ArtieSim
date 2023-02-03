/**
 * @file Generator.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

namespace Artie
{
    struct Generator
    {
        G4double energy;
        G4double length;
        G4double nominal_tof;
        G4double delta_tof;

        Generator(
            G4double _energy,
            G4double _length,
            G4double _nominal_tof,
            G4double _delta_tof
        )
        : energy(_energy), length(_length)
        , nominal_tof(_nominal_tof), delta_tof(_delta_tof)
        {
        }
    };
}