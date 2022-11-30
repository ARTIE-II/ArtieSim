/**
 * @file Water.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Water.hh"

namespace Artie
{
    Water::Water(G4String name)
    : mName(name)
    {
        DefineMaterials();
    }

    Water::~Water()
    {
    }


    void Water::DefineMaterials()
    {
        mMaterial.reset(
            G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER")
        );
        mMaterial->SetName(mName);
    }
}