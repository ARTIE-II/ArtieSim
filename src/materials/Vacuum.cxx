/**
 * @file Vacuum.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Vacuum.hh"

namespace Artie
{
    Vacuum::Vacuum(
        G4String name,
        G4double density
    )
    : mName(name)
    , mDensity(density)
    {
        DefineMaterials();
    }

    Vacuum::~Vacuum()
    {
    }

    void Vacuum::SetDensity(G4double density)
    {
        mDensity = density;
        DefineMaterials();
    }

    void Vacuum::DefineMaterials()
    {
        auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        mMaterial.reset(
            new G4Material(
                mName,
                mDensity * air->GetDensity(),
                1
            )
        );
        mMaterial->AddMaterial(air, 1.0);
    }
}