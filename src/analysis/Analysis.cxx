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
        AnalysisManager->CreateNtupleDColumn("alpha_energy");
        AnalysisManager->CreateNtupleDColumn("triton_energy");
        AnalysisManager->CreateNtupleDColumn("neutron_init_energy");
        AnalysisManager->CreateNtupleDColumn("neutron_tof");
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
        auto edps = Manager->GetEnergyDeposits();
        MyTuple.event_id = Manager->EventID();
        MyTuple.number_of_primaries = primaries.size();
        auto particles = Manager->GetParticles();
        auto particle_map = Manager->GetParticleTrackIDMap();
        auto pdg_map = Manager->GetParticlePDGMap();
        auto parent_map = Manager->GetParticleParentTrackIDMap();
        auto neutrons = Manager->GetNeutronEventData();
        G4int neutron_trk_id = -1000;
        MyTuple.alpha_energy = 0;
        MyTuple.triton_energy = 0;

        for (auto edep : edps) {
            //auto particle = particles[Manager->GetParticleTrackID(edep.track_id)];
            //auto parent = particles[particle.ParentTrackID()];
            G4int trk_id = edep.track_id;
            G4int pdg = pdg_map[trk_id];
            G4int parent = parent_map[trk_id];
            G4int parent_pdg = pdg_map[parent];
            G4int particle_idx = particle_map[trk_id];
            if (pdg == 1000020040 && parent_pdg == 2112) {
                MyTuple.alpha_energy += edep.energy;
                neutron_trk_id = parent;
                MyTuple.neutron_tof = particles[particle_idx].GetGlobalCreationTime();
                //std::cout<<"tof: "<<particles[particle_idx].GetGlobalCreationTime()<<std::endl;
                //MyTuple.neutron_init_energy = std::max(MyTuple.neutron_init_energy, parent.GetE());
            }
            if (pdg == 1000010030 && parent_pdg == 2112) {
                MyTuple.triton_energy += edep.energy;
                neutron_trk_id = parent;
                MyTuple.neutron_tof = particles[particle_idx].GetGlobalCreationTime();
                //MyTuple.neutron_init_energy = std::max(MyTuple.neutron_init_energy, parent.GetE());
            }
            //add gamma pion electron
        }

        for (auto particle : particles) {
            if (particle.TrackID() == neutron_trk_id) {
                MyTuple.neutron_init_energy = particle.GetE();
                break;
            }
        }

        //std::cout<<"Neutron: "<<std::endl;

        /*for (auto neutron : neutrons) {
            std::cout<<"event: "<<neutron.event<<" track: "<<neutron.track_id<<" energy: "<<neutron.neutron_energy<<" start: "<<neutron.start_time<<" end: "<<neutron.arrival_time<<std::endl;    
        }*/

        G4int index = Manager->GetIndex("Analysis");
        AnalysisManager->FillNtupleIColumn(index, 0, MyTuple.event_id);
        AnalysisManager->FillNtupleIColumn(index, 1, MyTuple.number_of_primaries);
        AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.alpha_energy);
        AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.triton_energy);
        AnalysisManager->FillNtupleDColumn(index, 4, MyTuple.neutron_init_energy);
        AnalysisManager->FillNtupleDColumn(index, 5, MyTuple.neutron_tof);
        AnalysisManager->AddNtupleRow(index);
    }
}