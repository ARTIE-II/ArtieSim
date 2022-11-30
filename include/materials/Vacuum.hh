/**
 * @file Vacuum.hh
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
    class Vacuum : public Material
    {
    public:
        /**
         * @brief Construct a new Vacuum object
         * 
         */
        Vacuum(G4String name, G4double density);
        ~Vacuum();

        G4double GetDensity() const { return mDensity; }
        void SetDensity(G4double density);

        void DefineMaterials();

    private:
        G4double mDensity = {1.0};

    };
}