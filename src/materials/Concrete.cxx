/**
 * @file Concrete.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Concrete.hh"

namespace Artie
{
    Concrete::Concrete(G4String name)
    : Material(name)
    {
        DefineMaterials();
    }

    Concrete::~Concrete()
    {
    }


    void Concrete::DefineMaterials()
    {
        mMaterial.reset(
            G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE")
        );
        mMaterial->SetName(mName);
    }
}