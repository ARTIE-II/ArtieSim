/**
 * @file DetectorFactory.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>

#include "Detector.hh"
#include "ArtieIDetector.hh"

namespace Artie
{
#ifdef ARTIE_YAML
    Detector* CreateDetector(G4String material, YAML::Node config);
#endif
}
