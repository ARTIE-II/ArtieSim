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
    class StainlessSteel : public Material
    {
    public:
        /**
         * @brief Construct a new StainlessSteel object
         * 
         */
        StainlessSteel(G4String name);
        ~StainlessSteel();

        void DefineMaterials();

    private:

    };
}