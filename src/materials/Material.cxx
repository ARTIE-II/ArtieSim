/**
 * @file Material.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Material.hh"

namespace Artie
{
    Material::Material(G4String name)
    : mName(name)
    {
    }

    Material::~Material()
    {
    }

    void Material::PrintProperties()
    {
        mMaterial->GetMaterialPropertiesTable()->DumpTable();
        G4cout << "Material: " << mMaterial->GetName() << G4endl;
        G4cout << "Nuclear interaction length: " << mMaterial->GetNuclearInterLength() << G4endl;
        G4cout << "ElectronDensity: " << mMaterial->GetElectronDensity() << G4endl;
        G4cout << "Radiation lenth: " << mMaterial->GetRadlen() << G4endl;
        G4cout << "Temperature:     " << mMaterial->GetTemperature() << G4endl;
        G4cout << "Pressure:        " << mMaterial->GetPressure() << G4endl;
    }
}