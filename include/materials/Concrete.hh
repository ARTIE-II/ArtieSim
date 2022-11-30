/**
 * @file Concrete.hh
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
    class Concrete : public Material
    {
    public:
        /**
         * @brief Construct a new Concrete object
         * 
         */
        Concrete(G4String name);
        ~Concrete();

        void DefineMaterials();

    private:
    
    };
}