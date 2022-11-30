/**
 * @file SimplePhysicsList.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "SimplePhysicsList.hh"

namespace Artie
{
    SimplePhysicsList::SimplePhysicsList()
    :G4VModularPhysicsList()
    {
        SetVerboseLevel(1);
        
        //add new units
        new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);   
        new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
        new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);  
            
        // Neutron Physics
        RegisterPhysics( new NeutronHPPhysics("neutronHP"));  
    }

    SimplePhysicsList::~SimplePhysicsList()
    { 
    }

    void SimplePhysicsList::ConstructParticle()
    {
        G4BosonConstructor  pBosonConstructor;
        pBosonConstructor.ConstructParticle();

        G4LeptonConstructor pLeptonConstructor;
        pLeptonConstructor.ConstructParticle();

        G4MesonConstructor pMesonConstructor;
        pMesonConstructor.ConstructParticle();

        G4BaryonConstructor pBaryonConstructor;
        pBaryonConstructor.ConstructParticle();

        G4IonConstructor pIonConstructor;
        pIonConstructor.ConstructParticle();

        G4ShortLivedConstructor pShortLivedConstructor;
        pShortLivedConstructor.ConstructParticle();  
    }

    void SimplePhysicsList::SetCuts()
    {
        SetCutValue(0*mm, "proton");
    }
}