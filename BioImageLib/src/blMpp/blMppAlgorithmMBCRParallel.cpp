/// \file blMppAlgorithmMBCRParallel.cpp
/// \brief blMppAlgorithmMBCRParallel class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppAlgorithmMBCRParallel.h"
#include "blMppAlgorithmMBCR.h"

using namespace std;

blMppAlgorithmMBCRParallel::blMppAlgorithmMBCRParallel()
    : blMppAlgorithm(){
    initAttributs();
}

blMppAlgorithmMBCRParallel::blMppAlgorithmMBCRParallel(blMppGeneratorShape* shapeGenerator,
                                                           blMppDataTerm* dataTerm,
                                                           blMppInteraction* interaction)
    : blMppAlgorithm(shapeGenerator, dataTerm, interaction){
    initAttributs();
}

blMppAlgorithmMBCRParallel::~blMppAlgorithmMBCRParallel(){
    delete m_config;
}

void blMppAlgorithmMBCRParallel::initAttributs(){
    m_maxIter = 1000;
    m_PerturbationProba = 0;
    m_nPerIter = 100;
    m_integerCoeff = 1000;
    m_iterationsVerboseRatio = 100;
    m_ProbaPerutbation1 = 0.3;
    m_ProbaPerutbation2 = 0.6;
    m_threadNumber = 2;

    m_config = NULL;

}

// parameters setters
std::string blMppAlgorithmMBCRParallel::name(){
    return "blMppAlgorithmMBCRParallel";
}

// setters parameters
void blMppAlgorithmMBCRParallel::setMaxIterations(int value){
    m_maxIter = value;
    if (m_maxIter < 1){
        throw( blException("blMppAlgorithmMBCRParallel: maxIterations must be at least one"));
    }
}

void blMppAlgorithmMBCRParallel::setPerturbationProba(float value){
    m_PerturbationProba = value;
    if (m_PerturbationProba > 1 || m_PerturbationProba < 0){
        throw( blException("blMppAlgorithmMBCRParallel: perturbationProba must be in [0,1]"));
    }
}

void blMppAlgorithmMBCRParallel::setNewShapesPerIterations(int value){
    m_nPerIter = value;
    if (m_nPerIter < 1 ){
        throw( blException("blMppAlgorithmMBCRParallel: newShapesPerIterations must be at least one"));
    }
}

void blMppAlgorithmMBCRParallel::setIntegerCoef(int value){
    m_integerCoeff = value;
    if (m_integerCoeff < 1 ){
        throw( blException("blMppAlgorithmMBCRParallel: integerCoef must be at least one"));
    }
}

void blMppAlgorithmMBCRParallel::setIterationsVerboseRatio(int value){
    m_iterationsVerboseRatio = value;
    if (m_iterationsVerboseRatio < 0 ){
        throw( blException("blMppAlgorithmMBCRParallel: iterationsVerboseRatio must be positive"));
    }
}

void blMppAlgorithmMBCRParallel::setNumberOfThread(int threadNumber){
    m_threadNumber = threadNumber;
    if (m_threadNumber < 0 ){
        throw( blException("blMppAlgorithmMBCRParallel: threadNumber must be positive"));
    }
}

// virtuals
std::vector<blMppShape*> blMppAlgorithmMBCRParallel::GetShapes(){

    std::vector<blMppShape*> shapes;
    vector<vector<blMppShape*> >*data = m_config->configData();
    for (int i = 0 ; i < data->size() ; ++i){
            for (int j = 0 ; j < data->operator [](i).size() ; ++j){
                blMppShape* copied = data->operator [](i)[j]->copy();
                shapes.push_back(copied);
            }
    }
    return shapes;
}

void blMppAlgorithmMBCRParallel::compute(){

    // run m_threadNumber mpp in parallel
    m_configurations.resize(m_threadNumber);
    //#if defined(_OPENMP)
    //#pragma omp parallel for
    //#endif
    for (int thread = 0 ; thread < m_threadNumber ; ++thread){
        //srand( time( NULL ) ); // initialize the random
        cout << "compute thread " << thread << endl;

        blMppAlgorithmMBCR *mpp = new blMppAlgorithmMBCR(m_shapeGenerator, m_dataTerm, m_interaction);
        mpp->setMaxIterations(m_maxIter/m_threadNumber);
        mpp->setNewShapesPerIterations(m_nPerIter);
        mpp->setPerturbationProba(m_PerturbationProba);
        mpp->setIterationsVerboseRatio(m_iterationsVerboseRatio);
        mpp->setIntegerCoef(m_integerCoeff);
        mpp->addObservers(this->list_observers);
        mpp->freeConfig(false);

        mpp->compute();
        m_configurations[thread] = mpp->configuration();
        delete mpp;
        cout << "delete thread " << thread << " done" << endl;
    }

    // competition rounds
    bool final = false;
    while (!final){
        cout << "do round" << endl;
        do_round();
        cout << "m_configurations.size() = " << m_configurations.size() << endl;
        if (m_configurations.size() == 2){
            final = true;
        }
    }

    // Final
    cout << "do final round" << endl;
    final_round();
}

void  blMppAlgorithmMBCRParallel::do_round(){


    blMppAlgorithmMBCR *mpp = new blMppAlgorithmMBCR(m_shapeGenerator, m_dataTerm, m_interaction);
    mpp->setMaxIterations(m_maxIter);
    mpp->setNewShapesPerIterations(m_nPerIter);
    mpp->setPerturbationProba(m_PerturbationProba);
    mpp->setIterationsVerboseRatio(m_iterationsVerboseRatio);
    mpp->setIntegerCoef(m_integerCoeff);
    mpp->addObservers(this->list_observers);
    mpp->freeConfig(false);

    mpp->setConfig(m_configurations[m_configurations.size()-1]);
    mpp->setConfigPrim(m_configurations[m_configurations.size()-2]);

    mpp->initFunctionsPointers();
    mpp->cut(1);

    m_configurations.erase(m_configurations.begin() + m_configurations.size()-1);
    m_configurations.erase(m_configurations.begin() + m_configurations.size()-1);
    m_configurations.push_back(mpp->configuration());

    delete mpp;


/*
    int configSize = m_configurations.size();
    vector<blMppContainerConfigShape2D*> configurationsInter; configurationsInter.resize(configSize/2);
    for (int i = 0 ; i < configSize/2 ; i++){
        blMppAlgorithmMBCR *mpp = new blMppAlgorithmMBCR(m_shapeGenerator, m_dataTerm, m_interaction);
        mpp->setMaxIterations(m_maxIter);
        mpp->setNewShapesPerIterations(m_nPerIter);
        mpp->setPerturbationProba(m_PerturbationProba);
        mpp->setIterationsVerboseRatio(m_iterationsVerboseRatio);
        mpp->setIntegerCoef(m_integerCoeff);
        mpp->addObservers(this->list_observers);
        mpp->freeConfig(false);

        mpp->setConfig(m_configurations[i]);
        mpp->setConfigPrim(m_configurations[configSize-i-1]);

        mpp->initFunctionsPointers();
        mpp->cut(1);

        configurationsInter[i] = mpp->configuration();
        delete mpp;
    }

    m_configurations = configurationsInter;
    */
}

void blMppAlgorithmMBCRParallel::final_round(){

    blMppAlgorithmMBCR *mpp = new blMppAlgorithmMBCR(m_shapeGenerator, m_dataTerm, m_interaction);
    mpp->setMaxIterations(m_maxIter);
    mpp->setNewShapesPerIterations(m_nPerIter);
    mpp->setPerturbationProba(m_PerturbationProba);
    mpp->setIterationsVerboseRatio(m_iterationsVerboseRatio);
    mpp->setIntegerCoef(m_integerCoeff);
    mpp->addObservers(this->list_observers);
    mpp->freeConfig(false);

    mpp->setConfig(m_configurations[0]);
    mpp->setConfigPrim(m_configurations[1]);

    mpp->initFunctionsPointers();
    mpp->cut(1);

    m_config = mpp->configuration();

    delete mpp;
}
