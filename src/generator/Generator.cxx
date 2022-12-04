/**
 * @file Generator.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "Generator.hh"

namespace Artie
{
    Generator::Generator()
    {
    }

    Generator::~Generator()
    {
    }

#ifdef ARTIE_YAML
    Generator::Generator(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    std::vector<PrimaryGeneration> Generator::GeneratePrimaryList()
    {
    }
}