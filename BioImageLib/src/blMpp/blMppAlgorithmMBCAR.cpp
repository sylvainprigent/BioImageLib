/// \file blMppAlgorithmMBCAR.cpp
/// \brief blMppAlgorithmMBCAR class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppAlgorithmMBCAR.h"
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/capacity_scaling.h>
#include <lemon/list_graph.h>
#include "blMppInteractionAttraction.h"
#include "itkRescaleIntensityImageFilter.h"
#include "blMppShapeGraph.h"

using namespace std;

blMppAlgorithmMBCAR::blMppAlgorithmMBCAR()
    : blMppAlgorithm(){
    initAttributs();
}

blMppAlgorithmMBCAR::blMppAlgorithmMBCAR(blMppGeneratorShape* shapeGenerator,
                                         blMppDataTerm* dataTerm,
                                         blMppInteraction* interaction)
    : blMppAlgorithm(shapeGenerator, dataTerm, interaction){
    initAttributs();
}

blMppAlgorithmMBCAR::~blMppAlgorithmMBCAR(){
    if (m_freeConfig)
        delete m_config;
    delete m_configPrim;
}

// parameters setters
std::string blMppAlgorithmMBCAR::name(){
    return "blMppAlgorithmMBCAR";
}

// setters parameters
void blMppAlgorithmMBCAR::setMaxIterations(int value){
    m_maxIter = value;
    if (m_maxIter < 1){
        throw( blException("blMppAlgorithmMBCAR: maxIterations must be at least one"));
    }
}

void blMppAlgorithmMBCAR::setPerturbationProba(float value){
    m_PerturbationProba = value;
    if (m_PerturbationProba > 1 || m_PerturbationProba < 0){
        throw( blException("blMppAlgorithmMBCAR: perturbationProba must be in [0,1]"));
    }
}

void blMppAlgorithmMBCAR::setNewShapesPerIterations(int value){
    m_nPerIter = value;
    if (m_nPerIter < 1 ){
        throw( blException("blMppAlgorithmMBCAR: newShapesPerIterations must be at least one"));
    }
}

void blMppAlgorithmMBCAR::setIntegerCoef(int value){
    m_integerCoeff = value;
    if (m_integerCoeff < 1 ){
        throw( blException("blMppAlgorithmMBCAR: integerCoef must be at least one"));
    }
}

void blMppAlgorithmMBCAR::setIterationsVerboseRatio(int value){
    m_iterationsVerboseRatio = value;
    if (m_iterationsVerboseRatio < 0 ){
        throw( blException("blMppAlgorithmMBCAR: iterationsVerboseRatio must be positive"));
    }
}

void blMppAlgorithmMBCAR::freeConfig(bool value){
    m_freeConfig = value;
}

blMppContainerConfigShapeGraph2D *blMppAlgorithmMBCAR::configuration(){
    return m_config;
}

void blMppAlgorithmMBCAR::setConfig(blMppContainerConfigShapeGraph2D *config){
    m_config = config;
}

void blMppAlgorithmMBCAR::setConfigPrim(blMppContainerConfigShapeGraph2D *config){
    m_configPrim = config;
}

// virtuals
std::vector<blMppShape*> blMppAlgorithmMBCAR::GetShapes(){

    std::vector<blMppShape*> shapes;
    vector<vector<blMppShapeGraph*> >*data = m_config->configData();
    for (int i = 0 ; i < data->size() ; ++i){
        for (int j = 0 ; j < data->operator [](i).size() ; ++j){
            blMppShape* copied = data->operator [](i)[j]->shape()->copy();
            shapes.push_back(copied);
        }
    }

    vector<vector<blMppShapeGraph*> >*data1 = m_configPrim->configData();
    for (int i = 0 ; i < data1->size() ; ++i){
        for (int j = 0 ; j < data1->operator [](i).size() ; ++j){
            blMppShape* copied = data1->operator [](i)[j]->shape()->copy();
            shapes.push_back(copied);
        }
    }
    return shapes;
}

void blMppAlgorithmMBCAR::initAttributs(){
    m_maxIter = 1000;
    m_PerturbationProba = 0;
    m_nPerIter = 100;
    m_integerCoeff = 1000;
    m_iterationsVerboseRatio = 100;
    m_ProbaPerutbation1 = 0.3;
    m_ProbaPerutbation2 = 0.6;

    m_config = NULL;
    m_configPrim = NULL;
    m_configSize = 0;
    m_countorConf = 0;
    m_freeConfig = false;
}


void blMppAlgorithmMBCAR::initConfig(){
    // create configurations
    int shapeMaxLength = 20; //m_shapeGenerator->shapesMaxLength();
    std::cout << "image size = " << m_dataTerm->image()->imageSize().xSize()<< std::endl; ;
    m_config = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);
    m_configPrim = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);
}

void blMppAlgorithmMBCAR::compute(){

    // create configurations
    int shapeMaxLength = m_shapeGenerator->shapesMaxLength();
    m_config = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);
    m_configPrim = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);

    // initial configuration
    ID = 0;

    // main loop
    int n = 0;
    int new_additions = 0;
    int initial_count = 0;
    int new_count = 0;
    while (n < m_maxIter){
        generateConfig();
        new_count = cut(n);
        new_additions = new_count - initial_count;
        initial_count = new_count;
        n++;
    }

    this->notify("Finished");
    this->notifyProgress( 100 );
}

// internal function
void blMppAlgorithmMBCAR::testConfig(std::vector<blMppShape*> &shapes){

    int k = 0, NP = 0, NQ = 0;
    double ID = 0;
    int K = shapes.size();

    float dataTerm;

    //start:
    while (k < K){
start:
        NP = NQ = 0;
        //std::cout << "k = " << k << std::endl;
        //1- Generate rand shape
        blMppShapeGraph *new_shape = new blMppShapeGraph(shapes[k]);
        //blMppShape *new_shape = shapes[k];
        //std::cout << "s2 new_shape = " << new_shape->getCenter().x() << ", "<< new_shape->getCenter().y() << std::endl;

        std::cout<<"---------------- Iteration Number --------------------------"<<k<<std::endl;

        //Step 2
        new_shape->setId(ID);
        ++ID;

        //Step 3A
        //If configuration is empty put object by default in P (m_config)
        if(m_config->getsize()==0 && m_configPrim->getsize()==0)
        {
            dataTerm = 0 ;//m_dataTerm->compute(new_shape);
            new_shape->shape()->setDataTerm(dataTerm);
            m_config->addShape(new_shape);
            new_shape->setLabel(0);
            k++;
            std::cout<<"first object added to P"<<std::endl;
            //continue;
            goto start;
        }

        //std::cout<<"Second Object"<<std::endl;

        //Step 3B : Comparing with shapes in P_prime
        vector<vector<blMppShapeGraph*> > *configData = m_config->configData();
        //std::cout << "s2 new_shape = " << new_shape->getCenter().x() << ", "<< new_shape->getCenter().y() << std::endl;

        int closestBucket = m_config->findClosestBucket(new_shape->shape());

        //std::cout<<"closestBucket = "<< closestBucket << std::endl;
        std::vector<int> buckets1 = m_config->getNeighboorBucket(closestBucket);
        //std::cout<<"buckets1 = "<< buckets1.size() << std::endl;
        //std::cout<<buckets1.size()<<std::endl;
        for (int i = 0 ; i < buckets1.size() ; ++i){
            //std::cout<<"i = "<< i << std::endl;
            int bucketIdx = buckets1[i];
            for (int s = 0 ; s < configData->operator [](bucketIdx).size() ; ++s){
                //std::cout << "s = " << s <<std::endl;
                blMppShapeGraph* shapeConfig = configData->operator[](bucketIdx)[s];
                float e = m_interaction->compute(new_shape->shape(), shapeConfig->shape());
                std::cout<<"Energy for (P class) object k = "<<k<<" is "<<e<<std::endl;
                //std::cout<<"Point Reached"<<std::endl;
                new_shape->addConnection(shapeConfig);
                new_shape->addEnergy(e);
                new_shape->addDefaultConnectionStatus();
                NP++;
                //std::cout<<"step 3B"<<std::endl;
            }
            //std::cout<<"Step 3B"<<std::endl;
        }
        std::cout << "NP = " << NP << std::endl;

        //Step 3C : Comparing with shapes in Q_prime
        //std::cout << "Step 3C : Comparing with shapes in Q_prime" << std::endl;
        vector<vector<blMppShapeGraph*> > *configDataPrim = m_configPrim->configData();
        std::vector<int> buckets2 = m_configPrim->getNeighboorBucket(m_configPrim->findClosestBucket(new_shape->shape()));
        //std::cout << "buckets2 = " << buckets2.size() << std::endl;
        for (int i = 0 ; i < buckets2.size() ; ++i){
            int bucketIdx = buckets2[i];
            for (int s = 0 ; s < configDataPrim->operator [](bucketIdx).size() ; ++s){
                blMppShapeGraph* shapeConfig = configDataPrim->operator[](bucketIdx)[s];
                float e = m_interaction->compute(new_shape->shape(), shapeConfig->shape());
                std::cout<<"Energy for (Q class) object k = "<<k<<" is "<<e<<std::endl;
                new_shape->addConnection(shapeConfig);
                new_shape->addEnergy(e);
                new_shape->addDefaultConnectionStatus();
                //std::cout<<"step 3C"<<std::endl;
                NQ ++;
            }
            //std::cout<<"Step 3C"<<std::endl;
        }
        std::cout << "NQ = " << NQ << std::endl;

        //Step 4A : Extract lists of connecting objects in P and Q
        std::vector<blMppShapeGraph*> T = new_shape->getconnection();

        std::vector<float> t = new_shape->getenergy();
        std::cout << "t = " << std::endl;
        for (int m = 0 ; m < t.size() ; ++m){
            std::cout << t[m] << std::endl;
        }
        std::cout << "t end " << std::endl;
        std::vector<blMppShapeGraph*> Q ;
        std::vector<float> q ;
        std::vector<blMppShapeGraph*> P;
        std::vector<float> p;

        if(NP != 1 && !T.empty())
        {
            std::vector<blMppShapeGraph*> PInter(T.begin(),T.begin()+NP);
            P = PInter;
            std::vector<float> pInter (t.begin(),t.begin()+NP);
            p = pInter;
        }
        else if (!T.empty())
        {
            P.push_back(T[0]);
            p.push_back(t[0]);
        }

        std::cout << "NQ = " << NQ << std::endl;
        if(NQ != 0 && !T.empty())
        {
            std::vector<blMppShapeGraph*> QInter (T.begin()+NP,T.end());
            Q = QInter;
            std::vector<float> qInter (t.begin()+NP,t.end());
            q = qInter;
        }

        //Step 4B : Implementation of Proposed algorithm for labelling
        std::cout << "Step 4B : Implementation of Proposed algorithm for labelling"<< std::endl;

        bool qDone = false;
        bool pDone = false;
        //int i = 0, j = 0;
        std::vector<float>::iterator i = p.begin();
        std::vector<float>::iterator j = q.begin();
        std::vector<float> Tab_p;
        std::vector<float> Tab_q;

        std::cout << "begin while loop" << std::endl;
        if (p.empty()){
            pDone = true;
        }
        else{
            while ( (*i >= 0) && (i != p.end()) )           //EDIT: Added equality
            {
                if(*i > 0)
                {
                    Tab_p.push_back(*i);
                    std::cout << "interaction = " << *i<< std::endl;
                    ++i;
                }
                else
                    i++;
                if ( i == p.end()){
                    pDone = true;
                }
            }
        }
        std::cout << "Tab_p size = " << Tab_p.size() << std::endl;

        if (!pDone)
            // in this case there is an object in P which is repulsive with new_shape
        {
            std::cout << "in this case there is an object in P which is repulsive with new_shape" << std::endl;
            std::cout << "p not done" << std::endl;
            if (NQ == 0){
                qDone = true;
            }
            else{
                while( (*j>=0) && (j!=q.end()) )        //EDIT: Added equality
                {
                    if(*j > 0)
                    {
                        Tab_q.push_back(*j);
                        ++j;
                    }
                    else
                        ++j;
                    if (j == q.end()){
                        qDone = true;
                    }
                }
            }
            if (!qDone)
                // in this case there is an object in Q which is repulsive with curent object
            {
                //cannot add object to configuration
                --ID;
                delete new_shape;
                k++;
                std::cout<<"Object Deleted at !qDone one"<<std::endl;
                //continue;
                //goto start;          //check later
            }
            else
                // in this case there is no repulsion between new_shape and the objects of Q
            {
                std::cout << "in this case there is no repulsion between new_shape and the objects of Q" << std::endl;
                if (Tab_p.size() > 0){
                    if(*std::max_element(Tab_p.begin(),Tab_p.end())>0)
                        // in this case there is an object in P which is attractive with new_shape
                        // then -> attraction an repusion with P we refuse the new_shape
                    {
                        std::cout << "attraction an repusion with P we refuse the new_shape" << std::endl;
                        //cannot add object to configuration
                        --ID;
                        delete new_shape;
                        k++;
                        std::cout<<"Object Deleted"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                }
                else
                {
                    while( (i!=p.end()) && (*i<0) )
                    {
                        Tab_p.push_back(*i);
                        ++i;
                        if ( i == p.end() ){
                            pDone = true;
                        }
                    }
                    if (pDone)
                        // no attraction with P
                    {
                        std::cout << "no attraction with P Put in Q" << std::endl;
                        //Put O in Q
                        dataTerm = 0;//m_dataTerm->compute(new_shape);
                        new_shape->shape()->setDataTerm(dataTerm);
                        new_shape->setLabel(1);
                        m_configPrim->addShape(new_shape);
                        k++;
                        std::cout<<"Object in Q"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                    else
                        // attraction with P and repulsion with Q
                    {
                        //cannot add object to configuration
                        --ID;
                        delete new_shape;
                        k++;
                        std::cout<<"Object Deleted"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                }
            }
        }
        else
            // No repulsion with P
        {
            if (Tab_p.size() > 0){
                if (*std::max_element(Tab_p.begin(),Tab_p.end())>0)
                    // Attraction with P
                {
                    if (NQ == 0){
                        qDone = true;
                    }
                    else{
                        while ( (j != q.end()) && (*j<0) )
                        {
                            Tab_q.push_back(*j);
                            ++j;
                            if (j == q.end()){
                                qDone = true;
                            }
                        }
                    }
                    if (!qDone)
                        // Attraction with Q
                    {
                        //cannot add object to configuration because attraction with P and Q
                        --ID;
                        delete new_shape;
                        k++;
                        std::cout<<"Object Deleted"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                    else
                        // no attraction with Q
                    {
                        //Put O in P
                        dataTerm = 0 ;//m_dataTerm->compute(new_shape);
                        new_shape->shape()->setDataTerm(dataTerm);
                        new_shape->setLabel(0);
                        m_config->addShape(new_shape);
                        k++;
                        std::cout<<"Object in P"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                }
            }
            else
                // No attraction with P
            {
                if (q.empty()){
                    qDone = true;
                }
                else{
                    while( (j != q.end()) && (*j<0) )
                    {
                        Tab_q.push_back(*j);
                        ++j;
                        if (j == q.end()){
                            qDone = true;
                        }
                    }
                }
                if(!qDone)
                    // attraction with Q
                {
                    if(*std::min_element(Tab_q.begin(),Tab_q.end())<0)
                        // Repulsion with Q
                    {
                        //cannot add object to configuration because attraction and
                        // repusltion with Q
                        --ID;
                        delete new_shape;
                        k++;
                        std::cout<<"Object Deleted"<<std::endl;
                        //continue;
                        //goto start;      //check later
                    }
                    else
                    {
                        if (q.empty()){
                            qDone = true;
                        }
                        else{
                            while( (j != q.end()) && (*j<0) )
                            {
                                Tab_q.push_back(*j);
                                ++j;
                                if (j == q.end()){
                                    qDone = true;
                                }
                            }
                        }
                        if(!qDone)
                            // Repulsion with Q
                        {
                            // cannot add object to configuration because
                            // attraction and repulsion with Q
                            --ID;
                            delete new_shape;
                            k++;
                            std::cout<<"Object Deleted"<<std::endl;
                            //continue;
                            //goto start;      //check later
                        }
                        else
                            // Repusltion with Q
                        {
                            //Put O in Q
                            dataTerm = 0;//m_dataTerm->compute(new_shape);
                            new_shape->shape()->setDataTerm(dataTerm);
                            new_shape->setLabel(1);
                            m_configPrim->addShape(new_shape);
                            k++;
                            std::cout<<"Object in Q"<<std::endl;
                            //continue;
                            //goto start;      //check later
                        }
                    }
                }
                else
                    // No attractions with Q
                {
                    //Put O in P
                    dataTerm = 0;//m_dataTerm->compute(new_shape);
                    new_shape->shape()->setDataTerm(dataTerm);
                    new_shape->setLabel(0);
                    m_config->addShape(new_shape);
                    k++;
                    std::cout<<"Object in P"<<std::endl;
                    //continue;
                    //goto start;      //check later
                }
            }
        }
    }
}

void blMppAlgorithmMBCAR::generateConfig(){

    int k = 0,n = 0,n_max = m_nPerIter, NP = 0, NQ = 0;

    float dataTerm;

    while (k < m_nPerIter && n<n_max){
start:
        //1- Generate rand shape
        blMppShapeGraph* new_shape = new blMppShapeGraph(m_shapeGenerator->generateRandom());
        //blMppShape *new_shape = m_shapeGenerator->generateRandom();

        NP = NQ = 0;

        //std::cout<<"---------------- Iteration Number --------------------------"<<k<<std::endl;

        //Step 2
        new_shape->setId(ID);
        ++ID;

        //Step 3A
        //If configuration is empty put object by default in P (m_config)
        if(m_config->getsize()==0 && m_configPrim->getsize()==0)
        {
            //dataTerm = 0 ;
            dataTerm = m_dataTerm->compute(new_shape->shape());
            new_shape->shape()->setDataTerm(dataTerm);
            m_config->addShape(new_shape);
            new_shape->setLabel(0);
            n = 0;
            k++;
            //std::cout<<"first object added to P"<<std::endl;
            goto start;
        }

        //Step 3B : Comparing with shapes in P_prime
        vector<vector<blMppShapeGraph*> > *configData = m_config->configData();
        int closestBucket = m_config->findClosestBucket(new_shape->shape());
        std::vector<int> buckets1 = m_config->getNeighboorBucket(closestBucket);

        for (int i = 0 ; i < buckets1.size() ; ++i){
            int bucketIdx = buckets1[i];
            for (int s = 0 ; s < configData->operator [](bucketIdx).size() ; ++s){
                blMppShapeGraph* shapeConfig = configData->operator[](bucketIdx)[s];
                float e = m_interaction->compute(new_shape->shape(), shapeConfig->shape());
                //std::cout<<"Energy for (P class) object k = "<<k<<" is "<<e<<std::endl;
                new_shape->addConnection(shapeConfig);
                new_shape->addEnergy(e);
                new_shape->addDefaultConnectionStatus();
                shapeConfig->addConnection(new_shape);
                shapeConfig->addEnergy(e);
                shapeConfig->addDefaultConnectionStatus();
                NP++;
            }
        }

        //std::cout << "NP = " << NP << std::endl;

        //Step 3C : Comparing with shapes in Q_prime
        vector<vector<blMppShapeGraph*> > *configDataPrim = m_configPrim->configData();
        std::vector<int> buckets2 = m_configPrim->getNeighboorBucket(m_configPrim->findClosestBucket(new_shape->shape()));
        for (int i = 0 ; i < buckets2.size() ; ++i){
            int bucketIdx = buckets2[i];
            for (int s = 0 ; s < configDataPrim->operator [](bucketIdx).size() ; ++s){
                blMppShapeGraph* shapeConfig = configDataPrim->operator[](bucketIdx)[s];
                float e = m_interaction->compute(new_shape->shape(), shapeConfig->shape());
                //std::cout<<"Energy for (Q class) object k = "<<k<<" is "<<e<<std::endl;
                new_shape->addConnection(shapeConfig);
                new_shape->addEnergy(e);
                new_shape->addDefaultConnectionStatus();
                shapeConfig->addConnection(new_shape);
                shapeConfig->addEnergy(e);
                shapeConfig->addDefaultConnectionStatus();
                NQ ++;
            }
        }

        //std::cout << "NQ = " << NQ << std::endl;

        //Step 4A : Extract lists of connecting objects in P and Q
        std::vector<blMppShapeGraph*> T = new_shape->getconnection();

        std::vector<float> t = new_shape->getenergy();
        //std::cout << "t = " << std::endl;
        //for (int m = 0 ; m < t.size() ; ++m){
        //    std::cout << t[m] << std::endl;
        //}
        //std::cout << "t end " << std::endl;
        std::vector<blMppShapeGraph*> Q ;
        std::vector<float> q ;
        std::vector<blMppShapeGraph*> P;
        std::vector<float> p;

        if(NP != 1 && !T.empty())
        {
            std::vector<blMppShapeGraph*> PInter(T.begin(),T.begin()+NP);
            P = PInter;
            std::vector<float> pInter (t.begin(),t.begin()+NP);
            p = pInter;
        }
        else if (!T.empty())
        {
            P.push_back(T[0]);
            p.push_back(t[0]);
        }

        //std::cout << "NQ = " << NQ << std::endl;
        if(NQ != 0 && !T.empty())
        {
            std::vector<blMppShapeGraph*> QInter (T.begin()+NP,T.end());
            Q = QInter;
            std::vector<float> qInter (t.begin()+NP,t.end());
            q = qInter;
        }

        //Step 4B : Implementation of Proposed algorithm for labelling
        //std::cout << "Step 4B : Implementation of Proposed algorithm for labelling"<< std::endl;

        int count = 0;
        bool qDone = false;
        bool pDone = false;
        std::vector<float>::iterator i = p.begin();
        std::vector<float>::iterator j = q.begin();
        std::vector<float> Tab_p;
        std::vector<float> Tab_q;

        //std::cout << "begin while loop" << std::endl;
        if (p.empty()){
            pDone = true;
        }
        else{
            while ( (*i >= 0) && (i != p.end()) )           //EDIT: Added equality
            {
                if(*i > 0)
                {
                    Tab_p.push_back(*i);
                    //std::cout << "interaction = " << *i<< std::endl;
                    ++i;
                }
                else
                    i++;
                if ( i == p.end()){
                    pDone = true;
                }
            }
        }
        //std::cout << "Tab_p size = " << Tab_p.size() << std::endl;

        if (!pDone)
            // in this case there is an object in P which is repulsive with new_shape
        {
            //std::cout << "in this case there is an object in P which is repulsive with new_shape" << std::endl;
            //std::cout << "p not done" << std::endl;
            if (NQ == 0){
                qDone = true;
            }
            else{
                while( (*j>=0) && (j!=q.end()) )        //EDIT: Added equality
                {
                    if(*j > 0)
                    {
                        Tab_q.push_back(*j);
                        ++j;
                    }
                    else
                        ++j;
                    if (j == q.end()){
                        qDone = true;
                    }
                }
            }
            if (!qDone)
                // in this case there is an object in Q which is repulsive with curent object
            {
                //cannot add object to configuration
                --ID;
                delete new_shape;
                n++;
                //std::cout<<"Object Deleted at !qDone one"<<std::endl;
            }
            else
                // in this case there is no repulsion between new_shape and the objects of Q
            {
                //std::cout << "in this case there is no repulsion between new_shape and the objects of Q" << std::endl;
                if (Tab_p.size() > 0){
                    if(*std::max_element(Tab_p.begin(),Tab_p.end())>0)
                        // in this case there is an object in P which is attractive with new_shape
                        // then -> attraction an repusion with P we refuse the new_shape
                    {
                        //std::cout << "attraction an repusion with P we refuse the new_shape" << std::endl;
                        //cannot add object to configuration
                        --ID;
                        delete new_shape;
                        n++;
                        //std::cout<<"Object Deleted"<<std::endl;
                    }
                }
                else
                {
                    while( (i!=p.end()) && (*i<=0) )     //EDIT : Added Equality
                    {
                        if(*i<0)
                        {
                            Tab_p.push_back(*i);
                            ++i;
                        }
                        else
                            ++i;
                        if ( i == p.end() ){
                            pDone = true;
                        }
                    }
                    if (pDone)
                        // no attraction with P
                    {
                        //std::cout << "no attraction with P Put in Q" << std::endl;
                        //Put O in Q
                        //dataTerm = 0;
                        dataTerm = m_dataTerm->compute(new_shape->shape());
                        new_shape->shape()->setDataTerm(dataTerm);
                        new_shape->setLabel(1);
                        m_configPrim->addShape(new_shape);
                        k++;
                        n=0;
                        //std::cout<<"Object in Q"<<std::endl;
                    }
                    else
                        // attraction with P and repulsion with Q
                    {
                        //cannot add object to configuration
                        --ID;
                        delete new_shape;
                        n++;
                        //std::cout<<"Object Deleted"<<std::endl;
                    }
                }
            }
        }
        else
            // No repulsion with P
        {
            if (Tab_p.size() > 0){
                if (*std::max_element(Tab_p.begin(),Tab_p.end())>0)
                    // Attraction with P
                {
                    if (NQ == 0){
                        qDone = true;
                    }
                    else{
                        while ( (j != q.end()) && (*j<=0) )     //EDIT: Added equality
                        {
                            if(*j<0)
                            {
                                Tab_q.push_back(*j);
                                ++j;
                            }
                            else
                                ++j;
                            if (j == q.end()){
                                qDone = true;
                            }
                        }
                    }
                    if (!qDone)
                        // Attraction with Q
                    {
                        //cannot add object to configuration because attraction with P and Q
                        --ID;
                        delete new_shape;
                        n++;
                        //std::cout<<"Object Deleted"<<std::endl;
                    }
                    else
                        // no attraction with Q
                    {
                        //Put O in P
                        //dataTerm = 0 ;
                        dataTerm = m_dataTerm->compute(new_shape->shape());
                        new_shape->shape()->setDataTerm(dataTerm);
                        new_shape->setLabel(0);
                        m_config->addShape(new_shape);
                        k++;
                        n=0;
                        //std::cout<<"Object in P"<<std::endl;
                    }
                }
            }
            else
                // No attraction with P
            {
                if (q.empty()){
                    qDone = true;
                }
                else{
                    while( (j != q.end()) && (*j <= 0) )       //EDIT: Added Equality
                    {
                        //std::cout<<"Value of j in loop"<<count<<std::endl;
                        //std::cout<<"Value of energy "<<*j<<std::endl;
                        count++;
                        if(*j < 0)
                        {
                            Tab_q.push_back(*j);
                            ++j;
                        }
                        else
                            ++j;

                        if (j == q.end()){
                            qDone = true;
                        }
                    }
                }
                if(!qDone)
                    // attraction with Q
                {
                    if(!Tab_q.empty() && *std::min_element(Tab_q.begin(),Tab_q.end())<0)
                        // Repulsion with Q
                    {
                        //cannot add object to configuration because attraction and
                        // repusltion with Q
                        --ID;
                        delete new_shape;
                        n++;
                        //std::cout<<"Object Deleted"<<std::endl;
                    }
                    else
                    {
                        if (q.empty()){
                            qDone = true;
                        }
                        else{
                            while( (j != q.end()) && (*j>=0) )      //EDIT: Added equality, sign change
                            {
                                if(*j>0)
                                {
                                    Tab_q.push_back(*j);
                                    ++j;
                                }
                                else
                                    ++j;

                                if (j == q.end()){
                                    qDone = true;
                                }
                            }
                        }
                        if(!qDone)
                            // Repulsion with Q
                        {
                            // cannot add object to configuration because
                            // attraction and repulsion with Q
                            --ID;
                            delete new_shape;
                            n++;
                            //std::cout<<"Object Deleted"<<std::endl;
                        }
                        else
                            //No Repusltion with Q
                        {
                            //Put O in Q
                            //dataTerm = 0;
                            dataTerm = m_dataTerm->compute(new_shape->shape());
                            new_shape->shape()->setDataTerm(dataTerm);
                            new_shape->setLabel(1);
                            m_configPrim->addShape(new_shape);
                            k++;
                            n=0;
                            //std::cout<<"Object in Q"<<std::endl;
                        }
                    }
                }
                else
                    // No attractions with Q
                {
                    //Put O in P
                    //dataTerm = 0;
                    dataTerm = m_dataTerm->compute(new_shape->shape());
                    new_shape->shape()->setDataTerm(dataTerm);
                    new_shape->setLabel(0);
                    m_config->addShape(new_shape);
                    k++;
                    n=0;
                    //std::cout<<"Object in P"<<std::endl;
                }
            }
        }
    }
}

void blMppAlgorithmMBCAR::saveConfig(std::string file){


    Float2DImage::Pointer imageRes = m_dataTerm->image()->itkFloat2DImagePointer();

    // normalize intensity
    typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(imageRes);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    imageRes = rescaleFilter->GetOutput();

    // create image
    FloatColor2DImage::Pointer resImage = FloatColor2DImage::New();

    FloatColor2DImage::RegionType region;
    FloatColor2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    itk::Image<itk::RGBPixel<float>, 2>::SizeType size;
    size[0] = m_dataTerm->image()->imageSize().xSize();
    size[1] = m_dataTerm->image()->imageSize().ySize();
    region.SetSize(size);

    resImage->SetRegions(region);
    resImage->Allocate();

    // copy the input image to the rgb output
    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageRes, imageRes->GetLargestPossibleRegion());

    PixelColorFloatType pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    // draw pixels on image
    // draw P
    vector<vector<blMppShapeGraph*> >* P = m_config->configData();
    FloatColor2DImage::IndexType pixelIndex;
    pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
    for (int i = 0 ; i < P->size() ; ++i){
        for (int b = 0 ; b < P->operator [](i).size() ; ++b){
            std::vector<blPixel> *pix = P->operator [](i)[b]->shape()->representation("border");
            for (unsigned int k = 0 ; k < pix->size() ; k++){
                pixelIndex[0] = pix->operator [](k).x();
                pixelIndex[1] = pix->operator [](k).y();
                resImage->SetPixel(pixelIndex, pixel);
            }
        }
    }
    // draw Q
    vector<vector<blMppShapeGraph*> >* Q = m_configPrim->configData();
    pixel[0] = 0; pixel[1] = 0; pixel[2] = 255;
    for (int i = 0 ; i < Q->size() ; ++i){
        for (int b = 0 ; b < Q->operator [](i).size() ; ++b){
            std::vector<blPixel> *pix = Q->operator [](i)[b]->shape()->representation("border");
            for (unsigned int k = 0 ; k < pix->size() ; k++){
                pixelIndex[0] = pix->operator [](k).x();
                pixelIndex[1] = pix->operator [](k).y();
                resImage->SetPixel(pixelIndex, pixel);
            }
        }
    }

    blImage* finalImage = new blImage(resImage);
    finalImage->saveFloatColor2DImage(file, true);
    //"C:\\Users\\prigent\\Documents\\Documents\\experiments\\test_blMppnew\\inter.tif"

}

//TODO : Edit code in here
int blMppAlgorithmMBCAR::cut(int n){

    int shapes_added = 0;


    //plotPQ("begin iteration " + blCast::to_string(n));

    // create graph
    lemon::ListDigraph g;
    lemon::ListDigraph::ArcMap<int> cap(g);

    // add source and sink
    //vector<vector<int> > configIdxs;
    lemon::ListDigraph::Node source, sink;
    lemon::ListDigraph::Arc arc;
    source = g.addNode();
    sink = g.addNode();
    //int countor = -1;
    unsigned int countor = 0;

    // Add conf Shapes to graph
    //std::cout << "Add conf Shapes to graph" << std::endl;
    lemon::ListDigraph::Node currentNode;
    vector<vector<blMppShapeGraph*> >* configData = m_config->configData();
    //configIdxs.resize(configData->size());
    float w;
    for (int i = 0 ; i < int(configData->size()) ; ++i){
        int sizebucket = configData->operator[](i).size();
        //configIdxs[i].resize(sizebucket);
        for (int b = 0 ; b < sizebucket ; ++b){
            //countor++;
            //configIdxs[i][b] = countor;
            currentNode = g.addNode();
            // capacity to sink
            blMppShapeGraph * temp1 = configData->operator[](i)[b];
            temp1->destroygraph();
            w = temp1->shape()->dataTerm();
            temp1->setgraphid(countor);
            countor++;

            //std::cout<<"Dataterm : "<<w<<std::endl;

            if(w>0)
            {
                arc = g.addArc(source,currentNode);
                cap[arc] = int(m_integerCoeff*(w));
            }
            else
            {
                arc = g.addArc(currentNode,sink);
                cap[arc] = int(m_integerCoeff*(-w));
            }
        }
    }

    // Add conf prim Shapes to graph
    //std::cout << "Add conf prim Shapes to graph" << std::endl;
    int sizebucket;
    int countorPrim = -1;
    lemon::ListDigraph::Node nodeConfPrim;
    vector<vector<blMppShapeGraph*> > *configPrimData = m_configPrim->configData();
    for (int j=0 ; j < int(configPrimData->size()) ; ++j){
        sizebucket = configPrimData->operator[](j).size();

        for (int b = 0 ; b < sizebucket ; ++b){
            countorPrim++;
            // add the node with weigth
            nodeConfPrim = g.addNode();
            // capacity to sink
            blMppShapeGraph * temp2 = configPrimData->operator[](j)[b];
            temp2->destroygraph();
            w = temp2->shape()->dataTerm();
            temp2->setgraphid(countor);
            //last node of previous loop and first node of this loop -> same countor ??
            countor ++;

            //std::cout<<"Dataterm : "<<w<<std::endl;

            if (w>0)
            {
                arc = g.addArc(nodeConfPrim,sink);
                cap[arc] = int(m_integerCoeff*(w));
            }
            else
            {
                arc = g.addArc(source,nodeConfPrim);
                cap[arc] = int(m_integerCoeff*(-w));
            }
        }
    }
    m_configSize = countor;

    //LARGE EDIT : Add functionality to add arcs depending on interactions between objects
    //std::cout << " add arcs depending on interactions" << std::endl;
    vector<vector<blMppShapeGraph*> >* configData1 = m_config->configData();
    //configIdxs.resize(configData1->size());

    //std::cout << " add arcs depending on interactions config" << std::endl;
    lemon::ListDigraph::Node node1, node2;
    for (int i = 0 ; i < int(configData1->size()) ; ++i){
        //std::cout << "add connection shape " << i << std::endl;
        int sizebucket = configData1->operator[](i).size();
        //configIdxs[i].resize(sizebucket);
        for (int b = 0 ; b < sizebucket ; ++b){

            blMppShapeGraph* tempshape = (configData1->operator[](i)[b]);
            int M = (tempshape->getconnection()).size();
            //std::cout << "M " << M << std::endl;
            for (int j = 0; j<M; ++j)
            {
                //std::cout<<"Value of iterator j : "<<j<<std::endl;
                //std::cout << "Enter 1 " << std::endl;
                blMppShapeGraph* tempshape2 = (tempshape->getconnection())[j];
                if (tempshape2){
                    //std::cout << "Enter 2 " << std::endl;
                    float e1 = (tempshape->getenergy())[j];
                    int e = int(m_integerCoeff*(e1));
                    //std::cout << "Enter 3 " << std::endl;

                    //std::cout<<"Interaction Energy : "<<e<<std::endl;

                    if (e>=0 && (tempshape->getLabel() == tempshape2->getLabel()))
                    {
                        //std::cout << "Enter 4 " << std::endl;
                        if(!tempshape->getConnectionStatus(tempshape2->getId()))
                        {
                            //std::cout << "id = " << tempshape->getgraphid() + 2 << std::endl;
                            //std::cout << "id 2 = " << tempshape2->getgraphid() + 2 << std::endl;
                            node1 = g.nodeFromId( tempshape->getgraphid() + 2 );
                            node2 = g.nodeFromId( tempshape2->getgraphid() + 2 );
                            arc = g.addArc(node1,node2);
                            cap[arc] = e;
                            //think of combining all branches from node 2 to sink
                            arc = g.addArc(node2,sink);
                            cap[arc] = e;
                            tempshape->setConnectionStatus(1,j);
                            tempshape2->setConnectionStatus2(1,tempshape->getId());
                        }
                    }
                    else if (e<0 && (tempshape->getLabel() != tempshape2->getLabel()))
                    {
                        if(!tempshape->getConnectionStatus(tempshape2->getId()))
                        {
                            node1 = g.nodeFromId( tempshape->getgraphid() + 2 );
                            node2 = g.nodeFromId( tempshape2->getgraphid() + 2 );
                            //arc = g.addArc(node2,node1);
                            arc = g.addArc(source,node1);
                            cap[arc] = -e;
                            arc = g.addArc(node1,node2);
                            cap[arc] = -1*e;
                            arc = g.addArc(node2,sink);
                            cap[arc] = -e;
                            tempshape->setConnectionStatus(1,j);
                            tempshape2->setConnectionStatus2(1,tempshape->getId());
                        }
                    }
                }
            }
        }
    }

    //std::cout << " add arcs depending on interactions config prim" << std::endl;
    vector<vector<blMppShapeGraph*> >* configData2 = m_configPrim->configData();
    //configIdxs.resize(configData2->size());

    for (int i = 0 ; i < int(configData2->size()) ; ++i){
        int sizebucket = configData2->operator[](i).size();
        //configIdxs[i].resize(sizebucket);
        //float w;
        for (int b = 0 ; b < sizebucket ; ++b){
            blMppShapeGraph* tempshape = (configData2->operator[](i)[b]);
            int M = (tempshape->getconnection()).size();
            for (int j = 0; j<M; ++j)
            {
                blMppShapeGraph* tempshape2 = (tempshape->getconnection())[j];
                if (tempshape2){
                    float e = (tempshape->getenergy())[j];
                    //std::cout<<"Interaction Energy : "<<e<<std::endl;

                    if (e>=0 && (tempshape->getLabel() == tempshape2->getLabel()))
                    {
                        if(!tempshape->getConnectionStatus(tempshape2->getId()))
                        {
                            node1 = g.nodeFromId( tempshape->getgraphid() + 2 );
                            node2 = g.nodeFromId( tempshape2->getgraphid() + 2 );
                            arc = g.addArc(node1,node2);
                            cap[arc] = e;
                            //think of combining all branches from node 2 to sink
                            arc = g.addArc(source,node1);
                            cap[arc] = e;
                            tempshape->setConnectionStatus(1,j);
                            tempshape2->setConnectionStatus2(1,tempshape->getId());
                        }
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
    //std::cout << "optimisation" << std::endl;
    lemon::Preflow<lemon::ListDigraph> preflow(g, cap, source, sink);
    preflow.runMinCut();

    // get output
    //std::cout << "get output" << std::endl;
    m_countorConf = -1;
    vector<vector<blMppShapeGraph* > > *newConfigData = new vector<vector<blMppShapeGraph*> >;
    newConfigData->resize(configData->size());
    for (unsigned int i = 0 ; i < configData->size() ; ++i){
        for (unsigned int b = 0 ; b < configData->operator[](i).size() ; ++b){
            m_countorConf++;
            blMppShapeGraph * shape1 = configData->operator[](i)[b];
            if ( !preflow.minCut (g.nodeFromId(shape1->getgraphid() + 2)) ){
                newConfigData->operator [](i).push_back(shape1);
                shapes_added++;
            }
            else{
                delete shape1;
            }
        }
    }

    vector<vector<blMppShapeGraph* > > *newConfigData1 = new vector<vector<blMppShapeGraph*> >;
    vector<vector<blMppShapeGraph*> >* configPrimData2 = m_configPrim->configData();
    newConfigData1->resize(configPrimData2->size());

    for (unsigned int j = 0 ; j < configPrimData2->size() ; ++j){
        for (unsigned int b = 0 ; b < configPrimData2->operator[](j).size() ; ++b){
            m_countorConf++;
            blMppShapeGraph * shape2 = configPrimData2->operator[](j)[b];
            //if ( preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
            if ( preflow.minCut (g.nodeFromId(shape2->getgraphid() + 2)) ){
                newConfigData1->operator [](j).push_back(shape2);
                shapes_added++;
            }
            else{
                //std::cout<<"Object 2 Deleted"<<std::endl;
                delete shape2;
            }
        }
    }
    m_countorConf++;

    m_config->setData(newConfigData);
    m_configPrim->setData(newConfigData1);

    //plotPQ("End iteration " + blCast::to_string(n));

    return shapes_added;
}

void blMppAlgorithmMBCAR::plotPQ(std::string title){

    std::cout << title << std::endl;
    // plot P
    vector<vector<blMppShapeGraph* > >* configDataP = m_config->configData();
    for (unsigned int i = 0 ; i < configDataP->size() ; ++i){
        //std::cout << "i = " << i << std::endl;
        for (unsigned int b = 0 ; b < configDataP->operator[](i).size() ; ++b){
            //std::cout << "b = " << b << std::endl;
            blMppShapeGraph * shape1 = configDataP->operator[](i)[b];
            vector<blMppShapeGraph*> conn = shape1->getconnection();
            std::cout << "P : shape " << i << ", " << b << std::endl;
            for (int k = 0 ; k < conn.size() ; ++k){
                std::cout << "connection " << k << " to (" <<conn[k]->shape()->getCenter().x() << ", " << conn[k]->shape()->getCenter().y() << ")" << std::endl;
            }
        }
    }

    // plot Q
    vector<vector<blMppShapeGraph* > > *configDataQ = m_configPrim->configData();
    for (unsigned int i = 0 ; i < configDataQ->size() ; ++i){
        for (unsigned int b = 0 ; b < configDataQ->operator[](i).size() ; ++b){
            blMppShapeGraph * shape1 = configDataQ->operator[](i)[b];
            vector<blMppShapeGraph*> conn = shape1->getconnection();
            std::cout << "Q : shape" << i << ", " << b << std::endl;
            for (int k = 0 ; k < conn.size() ; ++k){
                std::cout << "connection " << k << " to (" <<conn[k]->shape()->getCenter().x() << ", " << conn[k]->shape()->getCenter().y() << ")" << std::endl;
            }
        }
    }
}

