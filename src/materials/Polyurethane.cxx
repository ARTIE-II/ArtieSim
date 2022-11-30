/**
 * @file Polyurethane.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Polyurethane.hh"

namespace Artie
{
    Polyurethane::Polyurethane(G4String name)
    : Material(name)
    {
        DefineMaterials();
    }

    Polyurethane::~Polyurethane()
    {
    }


    void Polyurethane::DefineMaterials()
    {
        G4NistManager* man = G4NistManager::Instance();
        mMaterial.reset(
            new G4Material(mName, 1.005*g/cm3, 4)
        );
        G4Element* C = man->FindOrBuildElement("C");
        G4Element* H = man->FindOrBuildElement("H");
        G4Element* N = man->FindOrBuildElement("N");
        G4Element* O = man->FindOrBuildElement("O");
        mMaterial->AddElement(C, 3);
        mMaterial->AddElement(H, 8);
        mMaterial->AddElement(N, 2);
        mMaterial->AddElement(O, 1);
    }
}