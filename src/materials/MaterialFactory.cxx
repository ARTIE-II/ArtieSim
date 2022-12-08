/**
 * @file MaterialFactory.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "MaterialFactory.hh"

namespace Artie
{
    Material* CreateMaterial(G4String material, G4String name)
    {
        if(material == "liquid_argon")
        {
            return new Argon(
                "LAr_" + name, 
                83.0 * kelvin,      /// temperature
                0.952 * atmosphere, /// pressure
                kStateLiquid,       /// liquid
                0.334,              /// ratio of Ar36
                0.063,              /// ratio of Ar38
                99.603              /// ratio of Ar40
            );
        }
        else if(material == "gaseous_argon")
        {
            return new Argon(
                "GAr_" + name, 
                87.3 * kelvin,      /// temperature
                0.952 * atmosphere, /// pressure
                kStateGas,          /// gas
                0.334,              /// ratio of Ar36
                0.063,              /// ratio of Ar38
                99.603              /// ratio of Ar40
            );
        }
        else if(material == "concrete")
        {
            return new Concrete("Concrete_" + name);
        }
        else if(material == "kapton")
        {
            return new Kapton("Kapton_" + name);
        }
        else if(material == "polyurethane")
        {
            return new Polyurethane("Polyurethane_" + name);
        }
        else if(material == "stainless_steel")
        {
            return new StainlessSteel("StainlessSteel_" + name);
        }
        else if(material == "rough_vacuum")
        {
            return new Vacuum("RoughVacuum_" + name, 1.0E-1);
        }
        else if(material == "high_vacuum")
        {
            return new Vacuum("HighVacuum_" + name, 1.0E-11);
        }
        else if(material == "water")
        {
            return new Water("Water_" + name);
        }
        else if(material == "air")
        {
            return new Air("Air_" + name);
        }
        else
        {
            std::cerr << "ERROR! Material (" + material + ") not recognized!" << G4endl;
            exit(0);
        }
    }
}