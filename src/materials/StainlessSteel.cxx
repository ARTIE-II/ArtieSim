/**
 * @file StainlessSteel.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "StainlessSteel.hh"

namespace Artie
{
    StainlessSteel::StainlessSteel(G4String name)
    : mName(name)
    {
        DefineMaterials();
    }

    StainlessSteel::~StainlessSteel()
    {
    }


    void StainlessSteel::DefineMaterials()
    {
        mMaterial.reset(
            G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL")
        );
        mMaterial->SetName(mName);
    }
}