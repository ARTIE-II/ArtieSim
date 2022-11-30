/**
 * @file SimplePhysicsList.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "NeutronHPPhysics.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

namespace Artie
{
    class SimplePhysicsList: public G4VModularPhysicsList
    {
    public:
        SimplePhysicsList();
        ~SimplePhysicsList();

    public:
        virtual void ConstructParticle();
        virtual void SetCuts();
    };
}