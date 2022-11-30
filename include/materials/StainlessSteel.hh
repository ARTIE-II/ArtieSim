/**
 * @file StainlessSteel.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>

#include "Material.hh"

namespace Artie
{
    class StainlessSteel
    {
    public:
        /**
         * @brief Construct a new StainlessSteel object
         * 
         */
        StainlessSteel(G4String name);
        ~StainlessSteel();

        std::shared_ptr<G4Material> GetMaterial() const { return mMaterial; }

        void DefineMaterials();

    private:
        G4String mName;
        std::shared_ptr<G4Material> mMaterial = {nullptr};
    };
}