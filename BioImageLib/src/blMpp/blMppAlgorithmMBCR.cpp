/// \file blMppAlgorithmMBCR.cpp
/// \brief blMppAlgorithmMBCR class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppAlgorithmMBCR.h"
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/capacity_scaling.h>
#include <lemon/list_graph.h>
#include "blRand/blRandomGeneratorPM.h"

using namespace std;

blMppAlgorithmMBCR::blMppAlgorithmMBCR()
    : blMppAlgorithm(){
    initAttributs();
}

blMppAlgorithmMBCR::blMppAlgorithmMBCR(blMppGeneratorShape* shapeGenerator,
                                       blMppDataTerm* dataTerm,
                                       blMppInteraction* interaction)
    : blMppAlgorithm(shapeGenerator, dataTerm, interaction){
    initAttributs();
}

void blMppAlgorithmMBCR::initAttributs(){

    // parameters
    m_maxIter = 1000;
    m_PerturbationProba = 0;
    m_nPerIter = 100;
    m_integerCoeff = 1000;
    m_iterationsVerboseRatio = 100;
    m_ProbaPerutbation1 = 0.3;
    m_ProbaPerutbation2 = 0.6;

    // internal data
    m_config = NULL;
    m_configPrim = NULL;
    m_configSize = 0;
    m_countorConf = 0;
    m_freeConfig = true;
}

blMppAlgorithmMBCR::~blMppAlgorithmMBCR(){
    if (m_freeConfig)
        delete m_config;
    delete m_configPrim;
}

// parameters setters
std::string blMppAlgorithmMBCR::name(){
    return "blMppAlgorithmMBCR";
}

// setters parameters
void blMppAlgorithmMBCR::setMaxIterations(int value){
    m_maxIter = value;
    if (m_maxIter < 1){
        throw( blException("blMppAlgorithmMBCR: maxIterations must be at least one"));
    }
}

void blMppAlgorithmMBCR::setPerturbationProba(float value){
    m_PerturbationProba = value;
    if (m_PerturbationProba > 1 || m_PerturbationProba < 0){
        throw( blException("blMppAlgorithmMBCR: perturbationProba must be in [0,1]"));
    }
}

void blMppAlgorithmMBCR::setNewShapesPerIterations(int value){
    m_nPerIter = value;
    if (m_nPerIter < 1 ){
        throw( blException("blMppAlgorithmMBCR: newShapesPerIterations must be at least one"));
    }
}

void blMppAlgorithmMBCR::setIntegerCoef(int value){
    m_integerCoeff = value;
    if (m_integerCoeff < 1 ){
        throw( blException("blMppAlgorithmMBCR: integerCoef must be at least one"));
    }
}

void blMppAlgorithmMBCR::setIterationsVerboseRatio(int value){
    m_iterationsVerboseRatio = value;
    if (m_iterationsVerboseRatio < 0 ){
        throw( blException("blMppAlgorithmMBCR: iterationsVerboseRatio must be positive"));
    }
}

void blMppAlgorithmMBCR::freeConfig(bool value){
    m_freeConfig = value;
}

blMppContainerConfigShape2D* blMppAlgorithmMBCR::configuration(){
    return m_config;
}

void blMppAlgorithmMBCR::setConfig(blMppContainerConfigShape2D *config){
    m_config = config;
}

void blMppAlgorithmMBCR::setConfigPrim(blMppContainerConfigShape2D *config){
    m_configPrim = config;
}

// virtuals
std::vector<blMppShape*> blMppAlgorithmMBCR::GetShapes(){

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

void blMppAlgorithmMBCR::compute(){

    // create configurations
    //std::cout << "create configurations" << std::endl;
    int shapeMaxLength = m_shapeGenerator->shapesMaxLength();
    m_config = new blMppContainerConfigShape2D(m_dataTerm->image()->imageSize(), shapeMaxLength);
    m_configPrim = new blMppContainerConfigShape2D(m_dataTerm->image()->imageSize(), shapeMaxLength);

    // initial configuration
    //std::cout << "initial configuration" << std::endl;
    initFunctionsPointers();
    //std::cout << "generate initial configuration" << std::endl;
    generateNewRandConfiguration(m_config);
    //std::cout << "initial configuration done" << std::endl;

    // main loop
    int n = 0;
    while (n < m_maxIter){
        //std::cout << "iter = " << n << std::endl;
        //std::cout << "generate" << std::endl;
        (this->*generateNewConfigurationPtr)(m_configPrim);
        //std::cout << "cut" << std::endl;
        cut(n);
        n++;
    }

    this->notify("Finished");
    this->notifyProgress( 100 );
}


// internal function
void blMppAlgorithmMBCR::initFunctionsPointers(){

    // configuration generation
    if (m_PerturbationProba == 0)
        generateNewConfigurationPtr = &blMppAlgorithmMBCR::generateNewRandConfiguration;
    else
        generateNewConfigurationPtr = &blMppAlgorithmMBCR::generateNewPerturbationConfiguration;
}

void blMppAlgorithmMBCR::generateNewRandConfiguration(blMppContainerConfigShape2D* config){
    int k = 0,n = 0, n_max = m_nPerIter;

    //std::cout << "blMppAlgorithmMBCR::generateNewRandConfiguration begin" << std::endl;
    float dataTerm;
    while (k < m_nPerIter && n<n_max){
        //1- Generate rand shape
        //std::cout << "1- Generate rand shape" << std::endl;
        blMppShape *new_shape = m_shapeGenerator->generateRandom();

        // 2- Calculate data term
        dataTerm = m_dataTerm->compute(new_shape);

        //std::cout << std::endl << "2- Calculate data term done" << std::endl;

        if (dataTerm > 0){
            //std::cout << "data term negative" << std::endl;
            n++;
            delete new_shape;
            //std::cout << "data term negative done" << std::endl;
        }
        else{
            //std::cout << "is interaction" << std::endl;
            if (config->isInteraction(new_shape, m_interaction)){
                //std::cout << "delete" << std::endl;
                n++;
                delete new_shape;
            }
            else{
                //std::cout << "add" << std::endl;
                new_shape->setDataTerm(dataTerm);
                config->addShape(new_shape);
                n=0;
                k++;
            }
        }
    }
    //std::cout << "blMppAlgorithmMBCR::generateNewRandConfiguration end" << std::endl;
}

void blMppAlgorithmMBCR::generateNewPerturbationConfiguration(blMppContainerConfigShape2D *config){
    float r = blRandomGeneratorPM::rand();
    if (r > m_PerturbationProba){
        generateNewRandConfiguration(config);
    }
    else{
        // generate perturbed configuration
        float dataTerm;
        std::vector<std::vector<blMppShape*> >* data = m_config->configData();
        for (int i = 0 ; i < data->size() ; ++i){
            for (int j = 0 ; j < data->operator [](i).size() ; ++j){

                // do perturbation
                blMppShape* new_shape = m_shapeGenerator->generatePerturbation(data->operator [](i)[j]);

                // test acceptance
                if (!config->isInteraction(new_shape, m_interaction)){
                    dataTerm = m_dataTerm->compute(new_shape);
                    new_shape->setDataTerm(dataTerm);
                    config->addShape(new_shape);
                }
                else {
                    delete new_shape;
                }
            }
        }
    }
}

void blMppAlgorithmMBCR::cut(int n){

    // create graph
    lemon::ListDigraph g;
    lemon::ListDigraph::ArcMap<int> cap(g);

    // add source and sink
    vector<vector<int> > configIdxs;
    lemon::ListDigraph::Node source, sink;
    lemon::ListDigraph::Arc arc;
    source = g.addNode();
    sink = g.addNode();
    int countor = -1;

    // Add conf Shapes to graph
    lemon::ListDigraph::Node curentNode;
    vector<vector<blMppShape*> >* configData = m_config->configData();
    configIdxs.resize(configData->size());
    float w;
    for (int i = 0 ; i < int(configData->size()) ; ++i){
        int sizebucket = configData->operator[](i).size();
        configIdxs[i].resize(sizebucket);
        for (int b = 0 ; b < sizebucket ; ++b){
            countor++;
            configIdxs[i][b] = countor;
            curentNode = g.addNode();
            // capacity to sink
            w = -configData->operator[](i)[b]->dataTerm();
            arc = g.addArc(curentNode, sink);
            cap[arc] = int(m_integerCoeff*(w));

        }
    }
    m_configSize = countor+1;

    // Add conf prim Shapes to graph
    int sizebucket;
    int countorPrim = -1;
    lemon::ListDigraph::Node nodeConf, nodeConfPrim;
    vector<vector<blMppShape*> > *configPrimData = m_configPrim->configData();
    for (int j=0 ; j < int(configPrimData->size()) ; ++j){
        sizebucket = configPrimData->operator[](j).size();

        for (int b = 0 ; b < sizebucket ; ++b){
            countorPrim++;
            // add the node with weigth
            g.addNode();
            blMppShape* curentShapePrim = configPrimData->operator[](j)[b];
            // capacity to sink
            nodeConfPrim = g.nodeFromId( countorPrim + countor+1+2 );
            w = -configPrimData->operator[](j)[b]->dataTerm();
            arc = g.addArc(source, nodeConfPrim);
            cap[arc] = int(m_integerCoeff*(w));

            // add weigth with overlaping Shapes of m_ShapesStates
            float interactionValue;
            std::vector<int> buckets = m_config->getNeighboorBucket(m_config->findClosestBucket(curentShapePrim));
            for (int i = 0 ; i < buckets.size() ; ++i){
                int bucketIdx = buckets[i];
                for (int s = 0 ; s < configData->operator [](bucketIdx).size() ; ++s){
                    blMppShape* shapeConfig = configData->operator[](bucketIdx)[s];
                    interactionValue = m_interaction->compute(curentShapePrim, shapeConfig);
                    if (interactionValue < 0){
                        nodeConf = g.nodeFromId( configIdxs[bucketIdx][s]+2 );
                        arc = g.addArc(nodeConfPrim, nodeConf);
                        cap[arc] = int(-float(m_integerCoeff)*interactionValue);
                    }
                }
            }
        }
    }

    // verbose
    if (n%m_iterationsVerboseRatio == 0){
        this->notify("Shape count = " + blCast::to_string(m_configSize));
        this->notifyProgress( int(100*(float(n)/float(m_maxIter) )) );
    }

    // optimisation
    lemon::Preflow<lemon::ListDigraph> preflow(g, cap, source, sink);
    preflow.runMinCut();

    // get output
    m_countorConf = -1;
    vector<vector<blMppShape* > > *newConfigData = new vector<vector<blMppShape*> >;
    newConfigData->resize(configData->size());
    for (unsigned int i = 0 ; i < configData->size() ; ++i){
        for (unsigned int b = 0 ; b < configData->operator[](i).size() ; ++b){
            m_countorConf++;
            if ( !preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
                newConfigData->operator [](i).push_back(configData->operator[](i)[b]);
            }
            else{
                delete configData->operator[](i)[b];
            }
        }
    }

    for (unsigned int j = 0 ; j < configPrimData->size() ; ++j){
        for (unsigned int b = 0 ; b < configPrimData->operator[](j).size() ; ++b){
            m_countorConf++;
            if ( preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
                newConfigData->operator [](j).push_back(configPrimData->operator[](j)[b]);
            }
            else{
                delete configPrimData->operator[](j)[b];
            }
        }
    }
    m_countorConf++;

    m_config->setData(newConfigData);
    m_configPrim->clear();
}

