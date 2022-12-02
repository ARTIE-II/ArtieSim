/**
 * @file Core.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-12
 */
#include "Core.hh"

namespace Artie
{

    ArrayManager::ArrayManager()
    {
    }

    ArrayManager* ArrayManager::sInstance = nullptr;

    ArrayManager* ArrayManager::GetInstance()
    {
        if(sInstance == nullptr) {
            sInstance = new ArrayManager();
        }
        return sInstance;
    }

    G4String GetVolumeName(const G4Step* step)
    {
        G4String name = "none";
        const G4StepPoint *preStepPoint = step->GetPreStepPoint();
        const G4StepPoint *postStepPoint = step->GetPostStepPoint();
        
        G4LogicalVolume* preVolume = 0;
        G4VPhysicalVolume* prePhysical = 0;
        const G4TouchableHandle& preTouchable = preStepPoint->GetTouchableHandle();
        if(preTouchable) {
            prePhysical = preTouchable->GetVolume();
        }
        if(prePhysical) {
            preVolume = prePhysical->GetLogicalVolume();
        }
        if(preVolume) { 
            return preVolume->GetName();
        }

        G4LogicalVolume* postVolume = 0;
        G4VPhysicalVolume* postPhysical = 0;
        const G4TouchableHandle& postTouchable = postStepPoint->GetTouchableHandle();
        if(postTouchable) {
            postPhysical = postTouchable->GetVolume();
        }
        if(postPhysical) {
            postVolume = postPhysical->GetLogicalVolume();
        }
        if(postVolume) { 
            return postVolume->GetName();
        }

        return name;
    }

    G4String GetPostProcessName(const G4Step* step)
    {
        G4String name = "none";
        const G4StepPoint *postStepPoint = step->GetPostStepPoint();
        const G4VProcess* process = postStepPoint->GetProcessDefinedStep();
        if(process) {
            return process->GetProcessName();
        }
        return name;
    }
}