/**
 * @file NeutronHPPhysics.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "NeutronHPPhysics.hh"

namespace Artie
{
    NeutronHPPhysics::NeutronHPPhysics(const G4String& name)
    :  G4VPhysicsConstructor(name)
    , mThermal(true)
    {
    }

    NeutronHPPhysics::~NeutronHPPhysics()
    {
    }

    void NeutronHPPhysics::ConstructParticle()
    {
        G4Neutron::Neutron();
    }

    void NeutronHPPhysics::ConstructProcess()
    {
        G4ParticleDefinition* neutron = G4Neutron::Neutron();
        G4ProcessManager* pManager = neutron->GetProcessManager();
        G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();

        // delete all neutron processes if already registered
        G4VProcess* process = 0;
        process = processTable->FindProcess("hadElastic", neutron);      
        if (process) {
            pManager->RemoveProcess(process);
        }
        process = processTable->FindProcess("neutronInelastic", neutron);
        if (process) {
            pManager->RemoveProcess(process);
        }
        process = processTable->FindProcess("nCapture", neutron);      
        if (process) {
            pManager->RemoveProcess(process);
        }
        process = processTable->FindProcess("nFission", neutron);      
        if (process) {
            pManager->RemoveProcess(process);
        }
                        
        // Elastic Processes
        G4HadronElasticProcess* process1 = new G4HadronElasticProcess(
            //"neutronElastic"
        );
        G4ParticleHPElastic* model1a = new G4ParticleHPElastic();
        G4ParticleHPElasticData* dataSet1a = new G4ParticleHPElasticData();

        G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
        G4ParticleHPThermalScatteringData* dataSet1b = new G4ParticleHPThermalScatteringData();
        
        pManager->AddDiscreteProcess(process1);   
        process1->RegisterMe(model1a);
        process1->AddDataSet(dataSet1a);
        if (mThermal) 
        {
            model1a->SetMinEnergy(4*eV);
            process1->RegisterMe(model1b);
            process1->AddDataSet(dataSet1b);
        }

        // Inelastic Processes
        G4HadronInelasticProcess* process2 = new G4HadronInelasticProcess(
            "neutronInelastic"
        );
        G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
        G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
        pManager->AddDiscreteProcess(process2);   
        process2->AddDataSet(dataSet2);  
        process2->RegisterMe(model2);    

        // Neutron Capture   
        G4NeutronCaptureProcess* process3 = new G4NeutronCaptureProcess();
        G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
        G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
        pManager->AddDiscreteProcess(process3);  
        process3->AddDataSet(dataSet3);
        process3->RegisterMe(model3);
        
        // Neutron Fission  
        G4NeutronFissionProcess* process4 = new G4NeutronFissionProcess();
        G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
        G4ParticleHPFission* model4 = new G4ParticleHPFission();
        pManager->AddDiscreteProcess(process4);
        process4->AddDataSet(dataSet4);     
        process4->RegisterMe(model4);
    }
}