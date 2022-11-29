/**
 * @file Analysis.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "Analysis.hh"

namespace Artie
{
    void ExampleAnalysisFunction()
    {
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GetPrimaries();
        G4cout << "Event: " << Manager->EventID();
        G4cout << " - Number of primaries: " << primaries.size() << G4endl;
    }
}