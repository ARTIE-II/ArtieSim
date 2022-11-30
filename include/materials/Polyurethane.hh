/**
 * @file Polyurethane.hh
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
    class Polyurethane : public Material
    {
    public:
        /**
         * @brief Construct a new Polyurethane object
         * 
         */
        Polyurethane(G4String name);
        ~Polyurethane();
        
        void DefineMaterials();

    private:

    };
}