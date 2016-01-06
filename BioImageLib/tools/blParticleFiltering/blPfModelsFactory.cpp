#include "blPfModelsFactory.h"

blPfModelsFactory::blPfModelsFactory(blProcessParameters* parameters){
    m_parameters = parameters;
    m_observer = new blProcessObserverCommandeLine();
}

blPfModelsFactory::blPfModelsFactory(std::string parameterFile){
    m_parameters = new blProcessParameters(parameterFile);
    m_parameters->load();
    m_observer = new blProcessObserverCommandeLine();
}

blPfModelsFactory::~blPfModelsFactory(){
    delete m_observer;
}

blPfModelInterface* blPfModelsFactory::model(blPfDataManager* dataManager,  std::string name){

    if (name == "blPfModelDisk"){
        blPfModelDisk* model = new blPfModelDisk(dataManager);
        model->setSigmaPredictionX(m_parameters->getValueOfKey<float>("blPfModelDisk_VarPreditionNoiseX", 2.0));
        model->setSigmaPredictionY(m_parameters->getValueOfKey<float>("blPfModelDisk_VarPreditionNoiseY", 2.0));
        model->setObjectRadius(m_parameters->getValueOfKey<int>("blPfModelDisk_ObjectRadius", 4));
        model->setMaxMoveX(m_parameters->getValueOfKey<float>("blPfModelDisk_maxMoveX", 20.0));
        model->setMaxMoveY(m_parameters->getValueOfKey<float>("blPfModelDisk_maxMoveY", 20.0));

        return model;
    }
    if (name == "blPfModelAxonBaseRetract"){
        blPfModelAxonBaseRetract* model = new blPfModelAxonBaseRetract(dataManager);
        model->setSigmaPredictionNoise(m_parameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseX", 5.0),
                                     m_parameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseY", 5.0),
                                     m_parameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseZ", 0.5) );
        model->setModelParameters( m_parameters->getValueOfKey<float>("blPfModelAxon_alpha", 0.7),
                                 m_parameters->getValueOfKey<float>("blPfModelAxon_beta", 2.5),
                                 m_parameters->getValueOfKey<float>("blPfModelAxon_gamma", 0.25));
        model->setPredictionParameters(m_parameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionStatic", 0.5),
                                       m_parameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDynamic", 0.25),
                                       m_parameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDetection", 0.0),
                                       m_parameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDynamicReverse", 0.0));
        model->setPredictionWindowSize(m_parameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeX", 5),
                          m_parameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeY", 5),
                          m_parameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeZ", 2));
        model->setMaxMove(m_parameters->getValueOfKey<int>("blPfModelAxon_MaxMoveX", 15),
                          m_parameters->getValueOfKey<int>("blPfModelAxon_MaxMoveY", 15),
                          m_parameters->getValueOfKey<int>("blPfModelAxon_MaxMoveZ", 2));
        model->setVesselnessImagesPath(m_parameters->getValueOfKey<string>("blPfModelAxon_VesselnessPath", ""));
        return model;
    }
    if (name == "blPfModelLinearMotion"){
        blPfModelLinearMotion* model = new blPfModelLinearMotion(dataManager);
        model->setSigmaPredictionNoise(m_parameters->getValueOfKey<float>("blPfModelLinearMotion_SigmaPredictionNoiseX", 5.0),
                                     m_parameters->getValueOfKey<float>("blPfModelLinearMotion_SigmaPredictionNoiseY", 5.0));
        model->setModelParameters( m_parameters->getValueOfKey<float>("blPfModelLinearMotion_alpha", 0.7),
                                 m_parameters->getValueOfKey<float>("blPfModelLinearMotion_beta", 2.5),
                                 m_parameters->getValueOfKey<float>("blPfModelLinearMotion_gamma", 0.25),
                                   m_parameters->getValueOfKey<float>("blPfModelLinearMotion_delta", 0.25),
                                   m_parameters->getValueOfKey<float>("blPfModelLinearMotion_psi", 0.25)
                                   );
        model->setPredictionParameters(m_parameters->getValueOfKey<float>("blPfModelLinearMotion_PredictionProportionStatic", 0.25),
                                       m_parameters->getValueOfKey<float>("blPfModelLinearMotion_PredictionProportionDynamic", 0.50),
                                       m_parameters->getValueOfKey<float>("blPfModelLinearMotion_PredictionProportionDetection", 0.25));
        model->setMaxMove(m_parameters->getValueOfKey<int>("blPfModelLinearMotion_MaxMoveX", 15),
                          m_parameters->getValueOfKey<int>("blPfModelLinearMotion_MaxMoveY", 15));
        model->setAppearanceSizes(m_parameters->getValueOfKey<int>("blPfModelLinearMotion_minMinor", 3),
                                  m_parameters->getValueOfKey<int>("blPfModelLinearMotion_maxMinor", 5),
                                  m_parameters->getValueOfKey<int>("blPfModelLinearMotion_minMajor", 7),
                                  m_parameters->getValueOfKey<int>("blPfModelLinearMotion_maxMajor", 12));
        model->setVesselnessImagesPath(m_parameters->getValueOfKey<string>("blPfModelLinearMotion_VesselnessPath", ""));
        return model;
    }
    throw blException(("Cannot find the model " + name).c_str());
}
