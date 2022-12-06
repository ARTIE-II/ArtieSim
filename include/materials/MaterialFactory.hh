/**
 * @file MaterialFactory.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>

#include "Material.hh"
#include "Air.hh"
#include "Argon.hh"
#include "Concrete.hh"
#include "Kapton.hh"
#include "Polyurethane.hh"
#include "StainlessSteel.hh"
#include "Vacuum.hh"
#include "Water.hh"

namespace Artie
{
    Material* CreateMaterial(G4String material, G4String name);
}
