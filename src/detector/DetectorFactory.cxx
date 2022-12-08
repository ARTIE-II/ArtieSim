/**
 * @file DetectorFactory.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "DetectorFactory.hh"

namespace Artie
{
#ifdef ARTIE_YAML
    Detector* CreateDetector(G4String material, YAML::Node config)
    {
        if(material == "artieI") {
            return new ArtieIDetector(config);
        }
        else 
        {
            std::cout << "Detector: " << material << " not an available geometry!" << std::endl;
            exit(0);
        }
    }
#endif
}