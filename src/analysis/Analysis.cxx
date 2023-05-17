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
    AnalysisTuple MyTuple;

    void AnalysisFunctionRunBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        G4int index = Manager->GetIndex("Analysis");
        AnalysisManager->CreateNtuple("Analysis", "Analysis");
        AnalysisManager->CreateNtupleIColumn("event_id");
        AnalysisManager->CreateNtupleIColumn("number_of_primaries");
        AnalysisManager->FinishNtuple(index);
    }
    void AnalysisFunctionRunEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
    }
    void AnalysisFunctionEventBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();        
    }
    void AnalysisFunctionEventEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GetPrimaries();
        MyTuple.event_id = Manager->EventID();
        MyTuple.number_of_primaries = primaries.size();

        G4int index = Manager->GetIndex("Analysis");
        AnalysisManager->FillNtupleIColumn(index, 0, MyTuple.event_id);
        AnalysisManager->FillNtupleIColumn(index, 1, MyTuple.number_of_primaries);
        AnalysisManager->AddNtupleRow(index);
    }
}