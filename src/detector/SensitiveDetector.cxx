/**
 * @file SensitiveDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "SensitiveDetector.hh"

namespace Artie
{
    SensitiveDetector::SensitiveDetector(G4String name)
    : G4VSensitiveDetector(name)
    , mName(name)
    {
    }

    SensitiveDetector::~SensitiveDetector()
    {
    }

    G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
    { 
        auto Manager = EventManager::GetEventManager();
        Manager->AddHitInfoFromStep(step, history);
        return true;
    }
}