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
    NeutronHPPhysics::NeutronHPPhysics(
        const G4String& name, G4bool thermal
    )
    : G4VPhysicsConstructor(name)
    , mThermalNeutrons(true)
    {
    }

    NeutronHPPhysics::~NeutronHPPhysics()
    {
    }

    void NeutronHPPhysics::ConstructProcess()
    {
        G4ParticleDefinition* neutron = G4Neutron::Neutron();
        mPhysicsManager.reset(neutron->GetProcessManager());
        mProcesses.reset(mPhysicsManager->GetProcessList());
        
        // delete all neutron processes if already registered
        G4VProcess* process = 0;
        process = mPhysicsManager->GetProcess("hadElastic");
        if (process) mPhysicsManager->RemoveProcess(process);
        process = mPhysicsManager->GetProcess("neutronInelastic");
        if (process) mPhysicsManager->RemoveProcess(process);
        process = mPhysicsManager->GetProcess("nCapture");      
        if (process) mPhysicsManager->RemoveProcess(process);
        process = mPhysicsManager->GetProcess("nFission");      
        if (process) mPhysicsManager->RemoveProcess(process);      
                
        // (re) create process: elastic
        G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
        mPhysicsManager->AddDiscreteProcess(process1);

        // model1a
        G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
        process1->RegisterMe(model1a);
        process1->AddDataSet(new G4ParticleHPElasticData());

        // model1b
        if (mThermalNeutrons) 
        {
            model1a->SetMinEnergy(4*eV);   
            G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
            process1->RegisterMe(model1b);
            process1->AddDataSet(new G4ParticleHPThermalScatteringData());
        }

        // (re) create process: inelastic
        G4HadronInelasticProcess* process2 = new G4HadronInelasticProcess(
            "neutronInelastic"
        );
        mPhysicsManager->AddDiscreteProcess(process2);   

        // cross section data set
        G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
        process2->AddDataSet(dataSet2);  

        // models
        G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
        process2->RegisterMe(model2);    

        // (re) create process: nCapture   
        G4NeutronCaptureProcess* process3 = new G4NeutronCaptureProcess();
        mPhysicsManager->AddDiscreteProcess(process3);  

        // cross section data set
        G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
        process3->AddDataSet(dataSet3);

        // models
        G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
        process3->RegisterMe(model3);
        
        // (re) create process: nFission   
        G4NeutronFissionProcess* process4 = new G4NeutronFissionProcess();
        mPhysicsManager->AddDiscreteProcess(process4);

        // cross section data set
        G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
        process4->AddDataSet(dataSet4);     

        // models
        G4ParticleHPFission* model4 = new G4ParticleHPFission();
        process4->RegisterMe(model4);
    }

    void NeutronHPPhysics::PrintNeutronPhysicsLists()
    {
        // print out all processes for neutrons
        G4cout << "Enabled Neutron HP Physics Processes:" << G4endl;
        for(size_t ii = 0; ii < mProcesses->size(); ii++)
        {
            G4cout << "\t[" << ii << "]: " << (*mProcesses)[ii]->GetProcessName() << G4endl;
        }
    }
}