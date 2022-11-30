/**
 * @file Kapton.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Kapton.hh"

namespace Artie
{
    Kapton::Kapton(G4String name)
    : Material(name)
    {
        DefineMaterials();
    }

    Kapton::~Kapton()
    {
    }


    void Kapton::DefineMaterials()
    {
        mMaterial.reset(
            G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON")
        );
        mMaterial->SetName(mName);
    }
}