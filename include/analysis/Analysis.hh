/**
 * @file Analysis.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "EventManager.hh"

namespace Artie
{
    struct AnalysisTuple
    {
        G4int event_id = 0;
        G4int number_of_primaries = 0;
    };
    void AnalysisFunctionRunBegin();
    void AnalysisFunctionRunEnd();
    void AnalysisFunctionEventBegin();
    void AnalysisFunctionEventEnd();
}