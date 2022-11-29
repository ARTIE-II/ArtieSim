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
}