/**
 * @file Kapton.hh
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
    class Kapton : public Material
    {
    public:
        /**
         * @brief Construct a new Kapton object
         * 
         */
        Kapton(G4String name);
        ~Kapton();

        void DefineMaterials();

    private:
    
    };
}