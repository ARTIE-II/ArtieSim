/**
 * @file Generator.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <vector>

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif 

#include "Core.hh"

namespace Artie
{
    class Generator
    {
    public:
        Generator();
        ~Generator();

#ifdef ARTIE_YAML
        Generator(YAML::Node config);
        YAML::Node Config() const { return mConfig; }
#endif

        virtual std::vector<PrimaryGeneration> GeneratePrimaryList() = 0;

    private:
#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif

    };
}