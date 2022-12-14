/**
 * @file RunAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "RunAction.hh"

namespace Artie
{
    RunAction::RunAction()
    : G4UserRunAction()
    {
    }

    RunAction::~RunAction()
    {}

    void RunAction::BeginOfRunAction(const G4Run* run)
    {
    }

    void RunAction::EndOfRunAction(const G4Run* run)
    {
    }
}