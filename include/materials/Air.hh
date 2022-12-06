/**
 * @file Air.hh
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
    class Air : public Material
    {
    public:
        /**
         * @brief Construct a new Air object
         * 
         */
        Air(G4String name);
        ~Air();

        void DefineMaterials();

    private:
    
    };
}