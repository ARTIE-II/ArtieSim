/**
 * @file Material.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"

namespace Artie
{
    class Material
    {
    public:
        Material(G4String name);
        ~Material();

        void SetMaterial(std::shared_ptr<G4Material> material) { mMaterial = material; }
        std::shared_ptr<G4Material> GetMaterial() const { return mMaterial; }
        G4Material* GetMaterialPointer() { return mMaterial.get(); }

        virtual void DefineMaterials() = 0;
        void PrintProperties();

        G4String mName;
        std::shared_ptr<G4Material> mMaterial = {nullptr};
    };
}