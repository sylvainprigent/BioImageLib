/// \file blMppAlgorithmMBCARCluster.cpp
/// \brief blMppAlgorithmMBCARCluster class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppAlgorithmMBCARCluster.h"
#include <lemon/preflow.h>
#include <lemon/smart_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/capacity_scaling.h>
#include <lemon/list_graph.h>
#include "blMppInteractionAttraction.h"
#include "itkRescaleIntensityImageFilter.h"
#include "blMppCluster.h"

using namespace std;

blMppAlgorithmMBCARCluster::blMppAlgorithmMBCARCluster()
    : blMppAlgorithm(){
    initAttributs();
}

blMppAlgorithmMBCARCluster::blMppAlgorithmMBCARCluster(blMppGeneratorShape* shapeGenerator,
                                         blMppDataTerm* dataTerm,
                                         blMppInteraction* interaction)
    : blMppAlgorithm(shapeGenerator, dataTerm, interaction){
    initAttributs();
}

blMppAlgorithmMBCARCluster::~blMppAlgorithmMBCARCluster(){
    if (m_freeConfig)
        delete m_config;
    delete m_configPrim;
}

// parameters setters
std::string blMppAlgorithmMBCARCluster::name(){
    return "blMppAlgorithmMBCARCluster";
}

// setters parameters
void blMppAlgorithmMBCARCluster::setMaxIterations(int value){
    m_maxIter = value;
    if (m_maxIter < 1){
        throw( blException("blMppAlgorithmMBCARCluster: maxIterations must be at least one"));
    }
}

void blMppAlgorithmMBCARCluster::setPerturbationProba(float value){
    m_PerturbationProba = value;
    if (m_PerturbationProba > 1 || m_PerturbationProba < 0){
        throw( blException("blMppAlgorithmMBCARCluster: perturbationProba must be in [0,1]"));
    }
}

void blMppAlgorithmMBCARCluster::setNewShapesPerIterations(int value){
    m_nPerIter = value;
    if (m_nPerIter < 1 ){
        throw( blException("blMppAlgorithmMBCARCluster: newShapesPerIterations must be at least one"));
    }
}

void blMppAlgorithmMBCARCluster::setIntegerCoef(int value){
    m_integerCoeff = value;
    if (m_integerCoeff < 1 ){
        throw( blException("blMppAlgorithmMBCARCluster: integerCoef must be at least one"));
    }
}

void blMppAlgorithmMBCARCluster::setIterationsVerboseRatio(int value){
    m_iterationsVerboseRatio = value;
    if (m_iterationsVerboseRatio < 0 ){
        throw( blException("blMppAlgorithmMBCARCluster: iterationsVerboseRatio must be positive"));
    }
}

void blMppAlgorithmMBCARCluster::freeConfig(bool value){
    m_freeConfig = value;
}

blMppContainerConfigShapeGraph2D* blMppAlgorithmMBCARCluster::configuration(){
    return m_config;
}

void blMppAlgorithmMBCARCluster::setConfig(blMppContainerConfigShapeGraph2D *config){
    m_config = config;
}

void blMppAlgorithmMBCARCluster::setConfigPrim(blMppContainerConfigShapeGraph2D *config){
    m_configPrim = config;
}

void blMppAlgorithmMBCARCluster::initAttributs(){

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

// virtuals
std::vector<blMppShape*> blMppAlgorithmMBCARCluster::GetShapes(){

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

void blMppAlgorithmMBCARCluster::compute(){

    string filePath = "C:\\Users\\tarun\\Documents\\test\\shape_count.txt";
    //ofstream file(filePath.c_str(), ios::out | ios::trunc);

    // create configurations
    int shapeMaxLength = m_shapeGenerator->shapesMaxLength();
    m_config = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);
    m_configPrim = new blMppContainerConfigShapeGraph2D(m_dataTerm->image()->imageSize(), shapeMaxLength);

    // initial configuration
    ID = 0;
    //generateConfig();

    // main loop
    int n = 0;
    int new_count = 0;
    while (n < m_maxIter){
        generateConfig();
        //saveConfig("C:\\Users\\tarun\\Documents\\test\\interBefore_" + blCast::to_string(n) + ".tif");
        new_count = cut(n);
        //new_additions = new_count - initial_count;
        //initial_count = new_count;
        //if(file)
        //{
            //file<<new_additions<<endl;
        //    file<<new_count<<endl;
        //}
        //else{
        //    cerr << "Impossible to open the file!" << endl;
        //}

        //std::cout<<"iter = "<<n<<std::endl;
        //saveConfig("C:\\Users\\tarun\\Documents\\test\\inter_" + blCast::to_string(n) + ".tif");
        n++;
    }

    //file.close();

    //saveConfig("C:\\Users\\tarun\\Documents\\test\\inter_" + blCast::to_string(n) + ".tif");

    this->notify("Finished");
    this->notifyProgress( 100 );
}


// internal function
blMppCluster* blMppAlgorithmMBCARCluster::find_cluster(double id)
{
    int N = Config_P.size();
    for (int i = 0; i<N; ++i)
    {
        if(id == Config_P[i]->getId())
            return Config_P[i];
    }
    N = Config_Q.size();
    for (int j = 0; j<N; ++j)
    {
        if(id == Config_Q[j]->getId())
            return Config_Q[j];
    }

    return NULL;
}

void blMppAlgorithmMBCARCluster::generateConfig(){

    int k = 0,n = 0,n_max = m_nPerIter, NP = 0, NQ = 0;

    float dataTerm;

    while (k < m_nPerIter && n<n_max){
start:
        //1- Generate rand shape
        blMppShapeGraph *new_shape = new blMppShapeGraph(m_shapeGenerator->generateRandom());
        blMppCluster *new_cluster = new blMppCluster();
        new_cluster->set_merge(0);

        NP = NQ = 0;

        //std::cout<<"---------------- Iteration Number --------------------------"<<k<<std::endl;

        //Step 2
        new_shape->setId(ID);
        new_cluster->setId(ID);
        new_cluster->addshape(new_shape);
        new_shape->update_cluster(new_cluster->getId());
        ++ID;

        //Step 3A
        //If configuration is empty put object by default in P (m_config)
        if(m_config->getsize()==0 && m_configPrim->getsize()==0)
        {
            //dataTerm = 0 ;
            //std::cout<<"first object"<<std::endl;
            dataTerm = m_dataTerm->compute(new_shape->shape());
            new_shape->shape()->setDataTerm(dataTerm);
            m_config->addShape(new_shape);      //Need something similar for clusters
            new_shape->setLabel(0);
            //std::cout<<"first object set dataterm"<<std::endl;
            new_cluster->setDataTerm();
            //std::cout<<"first object set label"<<std::endl;
            new_cluster->setLabel(0);
            //std::cout<<"first object add to p"<<std::endl;
            Config_P.push_back(new_cluster);
            n = 0;
            k++;
            std::cout<<"first object added to P"<<std::endl;
            //std::cout<<"Dataterm of shape = "<<dataTerm<<std::endl;
            //std::cout<<"Dataterm of cluster = "<<new_cluster->dataTerm()<<std::endl;
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
            }
        }

        for (int i = 0 ; i < buckets1.size() ; ++i){
            int bucketIdx = buckets1[i];
            for (int s = 0 ; s < configData->operator [](bucketIdx).size() ; ++s){
                blMppShapeGraph* shapeConfig = configData->operator[](bucketIdx)[s];
                blMppCluster* temp = find_cluster(shapeConfig->get_CID());
                float e1 = temp->find_interaction(new_shape);
                //std::cout << "e1 = " << e1 << std::endl;

                if(new_cluster->getConnectionStatus(temp->getId()) == 2)
                {
                    new_cluster->AddConnection(temp);
                    //e1 = temp->find_interaction(new_shape);
                    new_cluster->AddEnergy(e1);
                    new_cluster->AddDefaultConnectionStatus();

                    //EDIT

                    temp->AddConnection(new_cluster);
                    temp->AddEnergy(e1);
                    temp->AddDefaultConnectionStatus();
                    NP++;
                }
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
            }
        }


        for (int i = 0 ; i < buckets2.size() ; ++i){
            int bucketIdx = buckets2[i];
            for (int s = 0 ; s < configDataPrim->operator [](bucketIdx).size() ; ++s){
                blMppShapeGraph* shapeConfig = configDataPrim->operator[](bucketIdx)[s];
                blMppCluster* temp = find_cluster(shapeConfig->get_CID());
                float e1 = temp->find_interaction(new_shape);
                //std::cout << "e1 = " << e1 << std::endl;

                if(new_cluster->getConnectionStatus(temp->getId()) == 2)
                {
                    new_cluster->AddConnection(temp);
                    //e1 = temp->find_interaction(new_shape);
                    new_cluster->AddEnergy(e1);
                    new_cluster->AddDefaultConnectionStatus();

                    //EDIT

                    temp->AddConnection(new_cluster);
                    temp->AddEnergy(e1);
                    temp->AddDefaultConnectionStatus();
                    NQ ++;
                }
            }
        }

        //std::cout << "NQ = " << NQ << std::endl;

        //Step 4A : Extract lists of connecting objects in P and Q
        std::vector<blMppCluster*> T = new_cluster->getconnection();

        std::vector<float> t = new_cluster->getenergy();
        //std::cout << "t = " << std::endl;
        //for (int m = 0 ; m < t.size() ; ++m){
        //    std::cout << t[m] << std::endl;
        //}
        //std::cout << "t end " << std::endl;
        std::vector<blMppCluster*> Q ;
        std::vector<float> q ;
        std::vector<blMppCluster*> P;
        std::vector<float> p;

        if(NP != 1 && !T.empty())
        {
            std::vector<blMppCluster*> PInter(T.begin(),T.begin()+NP);
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
            std::vector<blMppCluster*> QInter (T.begin()+NP,T.end());
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
                delete new_cluster;
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
                        delete new_cluster;
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
                        new_cluster->setDataTerm();
                        new_cluster->setLabel(1);
                        Config_Q.push_back(new_cluster);
                        k++;
                        n=0;
                        //std::cout<<"Dataterm of shape = "<<dataTerm<<std::endl;
                        //std::cout<<"Dataterm of cluster = "<<new_cluster->dataTerm()<<std::endl;
                        //std::cout<<"Object in Q"<<std::endl;
                    }
                    else
                        // attraction with P and repulsion with Q
                    {
                        //cannot add object to configuration
                        --ID;
                        delete new_shape;
                        delete new_cluster;
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
                        delete new_cluster;
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
                        new_cluster->setDataTerm();
                        new_cluster->setLabel(0);
                        Config_P.push_back(new_cluster);
                        k++;
                        n=0;
                        //std::cout<<"Dataterm of shape = "<<dataTerm<<std::endl;
                        //std::cout<<"Dataterm of cluster = "<<new_cluster->dataTerm()<<std::endl;
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
                        delete new_cluster;
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
                            delete new_cluster;
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
                            new_cluster->setDataTerm();
                            new_cluster->setLabel(1);
                            Config_Q.push_back(new_cluster);
                            k++;
                            n=0;
                            //std::cout<<"Dataterm of shape = "<<dataTerm<<std::endl;
                            //std::cout<<"Dataterm of cluster = "<<new_cluster->dataTerm()<<std::endl;
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
                    new_cluster->setDataTerm();
                    new_cluster->setLabel(0);
                    Config_P.push_back(new_cluster);
                    k++;
                    n=0;
                    //std::cout<<"Dataterm of shape = "<<dataTerm<<std::endl;
                    //std::cout<<"Dataterm of cluster = "<<new_cluster->dataTerm()<<std::endl;
                    //std::cout<<"Object in P"<<std::endl;
                }
            }
        }
    }
}

void blMppAlgorithmMBCARCluster::saveConfig(std::string file){


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

    //itk::Vector<float,3> pixel;
    itk::RGBPixel<float> pixel;
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

//MODIFIED TO WORK WITH CLUSTERS
int blMppAlgorithmMBCARCluster::cut(int n){

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
    //vector<vector<blMppShapeGraph*> >* configData = m_config->configData();
    //configIdxs.resize(configData->size());
    float w;
    int N = Config_P.size();
    for (int i = 0 ; i < N ; ++i){

        currentNode = g.addNode();
        // capacity to sink
        blMppCluster * temp1 = Config_P[i];
        temp1->set_merge(0);
        temp1->destroygraph();
        w = temp1->dataTerm();
        temp1->setgraphid(countor);
        countor++;

        //std::cout<<"Dataterm of cluster while building graph = "<<w<<std::endl;

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
    //m_configSize = countor;

    // Add conf prim Shapes to graph
    //std::cout << "Add conf prim Shapes to graph" << std::endl;
    //int sizebucket;
    int countorPrim = -1;
    lemon::ListDigraph::Node nodeConfPrim;
    int M = Config_Q.size();
    //vector<vector<blMppShapeGraph*> > *configPrimData = m_configPrim->configData();
    for (int j=0 ; j < M ; ++j){

        countorPrim++;
        // add the node with weigth
        nodeConfPrim = g.addNode();
        // capacity to sink
        blMppCluster * temp2 = Config_Q[j];
        temp2->set_merge(0);
        temp2->destroygraph();
        w = temp2->dataTerm();
        temp2->setgraphid(countor);
        //last node of previous loop and first node of this loop -> same countor ??
        countor ++;
        //std::cout<<"Dataterm of cluster while building graph = "<<w<<std::endl;

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

    //LARGE EDIT : Add functionality to add arcs depending on interactions between objects
    //std::cout << " add arcs depending on interactions" << std::endl;
    //vector<vector<blMppShapeGraph*> >* configData1 = m_config->configData();
    //configIdxs.resize(configData1->size());

    //std::cout << " add arcs depending on interactions config" << std::endl;
    lemon::ListDigraph::Node node1, node2;
    int N1 = Config_P.size();
    for (int i = 0 ; i < N1 ; ++i){

        blMppCluster* tempshape = Config_P[i];
        tempshape->set_merge(0);
        int M = (tempshape->getconnection()).size();
        //std::cout << "M " << M << std::endl;
        for (int j = 0; j<M; ++j)
        {
            //std::cout<<"Value of iterator j : "<<j<<std::endl;
            //std::cout << "Enter 1 " << std::endl;
            blMppCluster* tempshape2 = (tempshape->getconnection())[j];
            tempshape2->set_merge(0);
            if (tempshape2){
                //std::cout << "Enter 2 " << std::endl;
                float e1 = (tempshape->getenergy())[j];
                int e = int(m_integerCoeff*(e1));
                //std::cout << "Enter 3 " << std::endl;

                //std::cout<<"Interaction Energy : "<<e<<std::endl;

                if (e>0 && (tempshape->getLabel() == tempshape2->getLabel()))
                {
                    //std::cout << "Enter 4 " << std::endl;
                    if(tempshape->getConnectionStatus(tempshape2->getId()) == 0)
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
                    if(tempshape->getConnectionStatus(tempshape2->getId()) == 0)
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

    //std::cout << " add arcs depending on interactions config prim" << std::endl;
    vector<vector<blMppShapeGraph*> >* configData2 = m_configPrim->configData();
    //configIdxs.resize(configData2->size());

    int N2 = Config_Q.size();
    for (int i = 0 ; i < N2 ; ++i){
        blMppCluster* tempshape = Config_Q[i];
        tempshape->set_merge(0);
        int M = (tempshape->getconnection()).size();
        for (int j = 0; j<M; ++j)
        {
            blMppCluster* tempshape2 = (tempshape->getconnection())[j];
            tempshape2->set_merge(0);
            if (tempshape2){
                float e = (tempshape->getenergy())[j];
                //std::cout<<"Interaction Energy : "<<e<<std::endl;

                if (e>0 && (tempshape->getLabel() == tempshape2->getLabel()))
                {
                    if(tempshape->getConnectionStatus(tempshape2->getId()) == 0)
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

    // optimisation
    //std::cout << "optimisation" << std::endl;
    lemon::Preflow<lemon::ListDigraph> preflow(g, cap, source, sink);
    preflow.runMinCut();

    // get output
    //std::cout << "get output" << std::endl;
    m_countorConf = -1;
    //vector<vector<blMppShapeGraph* > > *newConfigData = new vector<vector<blMppShapeGraph*> >;
    //newConfigData->resize(configData->size());
    for (int i = N1 - 1 ; i >= 0 ; --i){
        m_countorConf++;
        blMppCluster * shape1 = Config_P[i];
        shape1->set_merge(0);
        //if ( !preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
        if ( !preflow.minCut (g.nodeFromId(shape1->getgraphid() + 2)) ){
            shapes_added++;
        }
        else{
            //std::cout<<"Object 1 Deleted"<<std::endl;
            //delete_cluster(shape1);
            Config_P.erase(Config_P.begin()+i);
            //N1--;
            delete_cluster(shape1);

        }

    }

    //vector<vector<blMppShapeGraph* > > *newConfigData1 = new vector<vector<blMppShapeGraph*> >;
    //vector<vector<blMppShapeGraph*> >* configPrimData2 = m_configPrim->configData();
    //newConfigData1->resize(configPrimData2->size());

    for (int j = N2 - 1 ; j >=0 ; --j){
        m_countorConf++;
        blMppCluster * shape2 = Config_Q[j];
        shape2->set_merge(0);
        //if ( preflow.minCut (g.nodeFromId(m_countorConf+2)) ){
        if ( preflow.minCut (g.nodeFromId(shape2->getgraphid() + 2)) ){
            shapes_added++;
        }
        else{
            //std::cout<<"Object 2 Deleted"<<std::endl;
            //delete_cluster(shape2);
            Config_Q.erase(Config_Q.begin()+j);
            //N2--;
            delete_cluster(shape2);
        }

    }
    m_countorConf++;

    //m_config->setData(newConfigData);
    //m_configPrim->setData(newConfigData1);

    //plotPQ("End iteration " + blCast::to_string(n));

    //TODO : ADD FUNCTIONALITY TO MERGE CLUSTERS
    //Merging clusters with repulsion

    int No_Merge = 0;
    int A = Config_P.size();
    for (int i=0; i<A; ++i)
    {
        blMppCluster* temp = Config_P[i];
        std::vector<float> energy = temp->getenergy();
        int N = energy.size();
        std::vector<blMppCluster*> connect = temp->getconnection();

        //for (int j = 0; j<N; ++j)
        for (int j = N-1; j>=0; --j)
        {
            blMppCluster* cluster = connect[j];
            if(energy[j]<0 && temp->get_size() == 1 && cluster->get_size() == 1 && temp->getLabel() != cluster->getLabel())
            {
                //float CommonArea = cluster->get_content()[0]->silhouette()->purcentageOverlaping(temp->get_content()[0]->silhouette());
                //if(CommonArea > 0)
                //{
                temp->set_merge(1);
                cluster->set_merge(1);
                double id = cluster->getId();
                int M = Config_Q.size();
                for (int k = 0; k<M; ++k)
                {
                    if(Config_Q[k]->getId() == id)
                    {
                        Config_Q.erase(Config_Q.begin()+k);
                        break;
                    }
                }
                bool flag = merge(temp,cluster);
                temp->set_merge(0);
                //std::cout<<"Merge Finished"<<std::endl;
                if (flag)
                {
                    //(temp->get_content())[1]->setLabel(temp->getLabel());
                    //temp->setDataTerm();        //EDIT
                    No_Merge++;
                    break;
                }
                //}
            }
        }
    }

    m_config->clear();
    m_configPrim->clear();

    //Move all remaining clusters in Q to P
    move();
    //std::cout<<"Size of P = "<<Config_P.size()<<std::endl;
    //std::cout<<"Size of Q = "<<Config_Q.size()<<std::endl;

    int count_P = 0;
    int count_Q = 0;

    for(int i=0; i<Config_P.size(); ++i)
    {
        blMppCluster* cluster = Config_P[i];
        int size = cluster->get_size();

        //std::cout<<"Connection Status Lists for P "<<std::endl;
        //printStatus(cluster->getConnectionStatus());

        if(size>2)
            std::cout<<"Cluster with larger size : "<<size<<std::endl;
        /*if (size == 1)
        {
            m_config->addShape((cluster->get_content())[0]);
        }
        else if (size == 2)
        {
            m_config->addShape((cluster->get_content())[0]);
            m_config->addShape((cluster->get_content())[1]);
        }*/

        for(int j=0; j<size; ++j)
        {
            m_config->addShape((cluster->get_content())[j]);
            count_P++;
        }
    }

    for(int i=0; i<Config_Q.size(); ++i)
    {
        blMppCluster* cluster = Config_Q[i];
        int size = cluster->get_size();

        //std::cout<<"Connection Status Lists for Q "<<std::endl;
        //printStatus(cluster->getConnectionStatus());

        if(size>2)
            std::cout<<"Cluster with larger size : "<<size<<std::endl;

        /*if (size == 1)
        {
            m_configPrim->addShape((cluster->get_content())[0]);
        }
        else if (size == 2)
        {
            m_configPrim->addShape((cluster->get_content())[0]);
            m_configPrim->addShape((cluster->get_content())[1]);
        }*/

        for(int j=0; j<size; ++j)
        {
            m_configPrim->addShape((cluster->get_content())[j]);
            count_Q++;
        }
    }

    // verbose
    if (n%m_iterationsVerboseRatio == 0){
        int size = m_config->getsize() + m_configPrim->getsize();
        this->notify("Shape count = " + blCast::to_string(size));
        this->notifyProgress( int(100*(float(n)/float(m_maxIter) )) );
    }

    //std::cout<<"Config P size = "<<Config_P.size()<<std::endl;
    //std::cout<<"Config shapes P size = "<<count_P<<std::endl;
    //std::cout<<"Config Q size = "<<Config_Q.size()<<std::endl;
    //std::cout<<"Config shapes Q size = "<<count_Q<<std::endl;

    //return shapes_added;
    return No_Merge;
}

void blMppAlgorithmMBCARCluster::printlist(std::vector<blMppCluster *> list)
{
    int N = list.size();
    std::cout<<"Content of list with size "<<N<<std::endl;
    for(int i=0; i<N; ++i)
    {
        std::cout<<list[i]->getId()<<std::endl;
    }
}

void blMppAlgorithmMBCARCluster::printStatus(std::vector<int> list)
{
    int N = list.size();
    std::cout<<"status: "<<N<<std::endl;
    for(int i=0; i<N; ++i)
    {
        std::cout<<list[i]<<std::endl;
    }
}

bool blMppAlgorithmMBCARCluster::merge(blMppCluster * cluster1, blMppCluster * cluster2)
{

    //Cluster 2 is already erased from config Q

    int n1 = cluster1->get_size();
    int n2 = cluster2->get_size();

    //std::cout<<"Connection Status for cluster 2 before merging :"<<std::endl;
    //printStatus(cluster2->getConnectionStatus());

    //merge only if both clusters have size 1
    if (n1 == 1 && n2 == 1)
    {
        blMppShapeGraph * temp_shape = (cluster2->get_content())[0];     //Cluster 2 still has a pointer to tempshape, goes away when cluster is deleted ?
        cluster1->addshape(temp_shape);
        temp_shape->update_cluster(cluster1->getId());

        //std::cout<<"ID of cluster 1 "<<cluster1->getId()<<std::endl;
        //std::cout<<"ID of cluster 2 "<<cluster2->getId()<<std::endl;

        //std::cout<<"Cluster lists before mutual deletion"<<std::endl;
        //printlist(cluster1->getconnection());
        //printlist(cluster2->getconnection());
        //std::cout<<"Cluster lists after mutual deletion"<<std::endl;

        //Deleting mutual connection between the 2 clusters

        (cluster1->get_content())[1]->setLabel(cluster1->getLabel());
        cluster1->setDataTerm();
        double z = cluster1->DeleteConnection(cluster2->getId());
        cluster1->DeleteEnergy(z);
        z = cluster2->DeleteConnection(cluster1->getId());
        cluster2->DeleteEnergy(z);

        //printlist(cluster1->getconnection());
        //printlist(cluster2->getconnection());

        //EDIT: Modifying and updating connection list of the merged cluster
        //blMppShapeGraph * initial_shape = cluster1->get_content()[0];
        std::vector <blMppCluster*> list1 = cluster1->getconnection();
        std::vector <blMppCluster*> list2 = cluster2->getconnection();
        int N1 = list1.size();
        int N2 = list2.size();

        //std::cout<<"N1 = "<<N1<<std::endl;
        //printlist(list1);
        //std::cout<<"N2 = "<<N2<<std::endl;
        //printlist(list2);

        std::vector<bool> checked;
        for (int k=0; k<N2; ++k)
            checked.push_back(0);

        for (int i=0; i<N1; ++i)
        {
            blMppCluster * temp1 = list1[i];
            for(int j=0; j<N2; ++j)
            {
                blMppCluster * temp2 = list2[j];
                if(temp1->getId() == temp2->getId())
                {
                    //cluster1->getenergy()[i] += cluster2->getenergy()[j];
                    cluster1->modify_energy(i,cluster2->getenergy()[j]);
                    checked[j] = 1;
                    //std::cout<<"Inner point reached checked = 1"<<std::endl;
                    //std::cout<<"j = "<<j<<std::endl;
                    break;
                }
            }
        }

        for (int i=0; i<N2; ++i)
        {
            blMppCluster * temp2 = list2[i];
            if(!checked[i])
            {
                //blMppCluster * tempcluster = find_cluster(temp2->get_CID()) ;
                cluster1->AddConnection(temp2);
                cluster1->AddDefaultConnectionStatus();
                cluster1->AddEnergy(cluster2->getenergy()[i]);
                cluster1->setConnectionStatus2(cluster2->getConnectionStatus(temp2->getId()),temp2->getId());
                //std::cout<<"Initial connection status : "<<cluster2->getConnectionStatus(temp2->getId())<<std::endl;
                //std::cout<<"Initial connection status other way around : "<<temp2->getConnectionStatus(cluster2->getId())<<std::endl;
                //std::cout<<"Inner point reached checked = 0"<<std::endl;
                //std::cout<<"i = "<<i<<std::endl;
            }
        }

        //connections from cluster 2 are now routed through cluster 1 (outward connections)

        for (int i=0; i<N2; ++i)
        {
            //std::cout<<"Point Reached i = "<<i<<std::endl;
            if(checked[i])
            {
                //There is already a connection to cluster 1
                blMppCluster * temp = list2[i];
                double k = temp->DeleteConnection(cluster2->getId());
                //>> CHECK THIS !!
                //float e = cluster2->getenergy()[k];
                float e = temp->getenergy()[k];
                double r = temp->find_location(cluster1->getId());
                //cluster1->getenergy()[r] += e;
                //cluster1->modify_energy(r,e);
                temp->modify_energy(r,e);
                temp->DeleteEnergy(k);
            }
            else
            {
                //There is connection only to cluster 2
                blMppCluster * temp1 = list2[i];
                double t = temp1->find_location(cluster2->getId());
                //temp1->getconnection()[t] = cluster1;
                temp1->modify_connection(t,cluster1);
            }
        }

        //Now clusters that were connected to cluster2 are now connected only to cluster 1

        //std::cout<<"Cluster lists after merge "<<std::endl;
        //printlist(cluster1->getconnection());

        //std::cout<<"Cluster lists after merge connections status "<<std::endl;
        //printStatus(cluster1->getConnectionStatus());

        //double temp_id = cluster2->getId();
        //printlist(cluster2->getconnection());
        delete cluster2;

        /*int S = Config_P.size();
        for (int i = 0; i<S; ++i)
        {
            blMppCluster* xyz = Config_P[i];
            double K = xyz->find_location(temp_id);
            if (K!=10000)
                std::cout<<"Cluster 2 connection still exists somewhere"<<std::endl;
        }

        S = Config_Q.size();
        for (int i = 0; i<S; ++i)
        {
            blMppCluster* xyz = Config_Q[i];
            double K = xyz->find_location(temp_id);
            if (K!=10000)
                std::cout<<"Cluster 2 connection still exists somewhere"<<std::endl;
        }*/

        return 1;
    }
    return 0;
}

void blMppAlgorithmMBCARCluster::delete_cluster(blMppCluster* cluster)
{
    /*if(cluster->get_size() == 1)
        delete cluster->get_content()[0];
    else if (cluster->get_size() == 2)
    {
        delete cluster->get_content()[0];
        delete cluster->get_content()[1];
    }*/

    int n = cluster->get_size();
    for (int i=n-1; i>=0; --i)
    {
        delete cluster->get_content()[i];
    }

    delete cluster;
}

void blMppAlgorithmMBCARCluster::plotPQ(std::string title){

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

void blMppAlgorithmMBCARCluster::move()
{
    //Moving everything from config Q to Config P after merging clusters

    int N = Config_Q.size();
    for (int i = N-1; i >= 0; --i)
    {
        blMppCluster* temp_cluster = Config_Q[i];
        blMppShapeGraph* temp_shape = temp_cluster->get_content()[0];
        Config_Q.pop_back();
        Config_P.push_back(temp_cluster);
        temp_cluster->setLabel(0);
        temp_shape->setLabel(0);
    }
}
