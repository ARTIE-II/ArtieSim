/**
 * @file Air.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Air.hh"

namespace Artie
{
    Air::Air(G4String name)
    : Material(name)
    {
        DefineMaterials();
    }

    Air::~Air()
    {
    }


    void Air::DefineMaterials()
    {
        mMaterial.reset(
            G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR")
        );
        mMaterial->SetName(mName);
    }
}