/// \file blMppContainerConfigShape2D.cpp
/// \brief blMppContainerConfig2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppContainerConfigShape2D.h"


// /////////////////////////////////////////////////////////////////
//                   blMppContainerConfig2D
// /////////////////////////////////////////////////////////////////
blMppContainerConfig2D::blMppContainerConfig2D(){
    initAttributs();
}

blMppContainerConfig2D::blMppContainerConfig2D(blImageSize imageSize, int shapeMaxLength){
    initAttributs();
    initBuckets(imageSize, shapeMaxLength);
}

void blMppContainerConfig2D::initAttributs(){
    m_numBucketsX = 0;
    m_numBucketsY = 0;
    m_sizeBucketX = 0;
    m_sizeBucketY = 0;
    m_shapeMaxLength = 0;
    m_size = 0;
}

blMppContainerConfig2D::~blMppContainerConfig2D(){
}

// getters / setters
int blMppContainerConfig2D::getsize(){
    return m_size;
}

// operations
std::vector<int> blMppContainerConfig2D::getNeighboorBucket(int bucket){

    int i = bucket/m_numBucketsY;
    int j = bucket - m_numBucketsY*i;
    std::vector<int> buckets;

    if (i > 0 && i < m_numBucketsX-1 && j > 0 && j < m_numBucketsY-1 ){
        buckets.resize(9);
        buckets[0] = m_numBucketsY*(i-1)+(j-1);
        buckets[1] = m_numBucketsY*(i-1)+(j);
        buckets[2] = m_numBucketsY*(i-1)+(j+1);
        buckets[3] = m_numBucketsY*(i)+(j-1);
        buckets[4] = m_numBucketsY*(i)+(j);
        buckets[5] = m_numBucketsY*(i)+(j+1);
        buckets[6] = m_numBucketsY*(i+1)+(j-1);
        buckets[7] = m_numBucketsY*(i+1)+(j);
        buckets[8] = m_numBucketsY*(i+1)+(j+1);
    }
    else if (i == 0 && j > 0 && j < m_numBucketsY-1){
        buckets.resize(6);
        buckets[0] = m_numBucketsY*(i)+(j-1);
        buckets[1] = m_numBucketsY*(i)+(j);
        buckets[2] = m_numBucketsY*(i)+(j+1);
        buckets[3] = m_numBucketsY*(i+1)+(j-1);
        buckets[4] = m_numBucketsY*(i+1)+(j);
        buckets[5] = m_numBucketsY*(i+1)+(j+1);
    }
    else if (i == m_numBucketsX-1 && j > 0 && j < m_numBucketsY-1 ){
        buckets.resize(6);
        buckets[0] = m_numBucketsY*(i-1)+(j-1);
        buckets[1] = m_numBucketsY*(i-1)+(j);
        buckets[2] = m_numBucketsY*(i-1)+(j+1);
        buckets[3] = m_numBucketsY*(i)+(j-1);
        buckets[4] = m_numBucketsY*(i)+(j);
        buckets[5] = m_numBucketsY*(i)+(j+1);
    }
    else if (i > 0 && i < m_numBucketsX-1 &&  j == 0){
        buckets.resize(6);
        buckets[0] = m_numBucketsY*(i-1)+(j);
        buckets[1] = m_numBucketsY*(i)+(j);
        buckets[2] = m_numBucketsY*(i+1)+(j);
        buckets[3] = m_numBucketsY*(i-1)+(j+1);
        buckets[4] = m_numBucketsY*(i)+(j+1);
        buckets[5] = m_numBucketsY*(i+1)+(j+1);
    }
    else if (i > 0 && i < m_numBucketsX-1 && j == m_numBucketsY-1 ){
        buckets.resize(6);
        buckets[0] = m_numBucketsY*(i-1)+(j-1);
        buckets[1] = m_numBucketsY*(i)+(j-1);
        buckets[2] = m_numBucketsY*(i+1)+(j-1);
        buckets[3] = m_numBucketsY*(i-1)+(j);
        buckets[4] = m_numBucketsY*(i)+(j);
        buckets[5] = m_numBucketsY*(i+1)+(j);
    }
    else if ( i == 0 && j == 0){
        buckets.resize(4);
        buckets[0] = m_numBucketsY*(i)+(j);
        buckets[1] = m_numBucketsY*(i)+(j+1);
        buckets[2] = m_numBucketsY*(i+1)+(j);
        buckets[3] = m_numBucketsY*(i+1)+(j+1);
    }
    else if (i == 0 && j == m_numBucketsY-1 ){
        buckets.resize(4);
        buckets[0] = m_numBucketsY*(i)+(j-1);
        buckets[1] = m_numBucketsY*(i)+(j);
        buckets[2] = m_numBucketsY*(i+1)+(j-1);
        buckets[3] = m_numBucketsY*(i+1)+(j);
    }
    else if (i == m_numBucketsX-1 && j == 0 ){
        buckets.resize(4);
        buckets[0] = m_numBucketsY*(i-1)+(j);
        buckets[1] = m_numBucketsY*(i-1)+(j+1);
        buckets[2] = m_numBucketsY*(i)+(j);
        buckets[3] = m_numBucketsY*(i)+(j+1);
    }
    else if (i == m_numBucketsX-1 && j == m_numBucketsY-1 ){
        buckets.resize(4);
        buckets[0] = m_numBucketsY*(i-1)+(j-1);
        buckets[1] = m_numBucketsY*(i-1)+(j);
        buckets[2] = m_numBucketsY*(i)+(j-1);
        buckets[3] = m_numBucketsY*(i)+(j);
    }
    return buckets;
}

int blMppContainerConfig2D::findClosestBucket(blMppShape* shape){

    int i = int(shape->getCenter().x()/m_sizeBucketX);
    int j = int(shape->getCenter().y()/m_sizeBucketY);

    return m_numBucketsY*i+j;
}


// internal
void blMppContainerConfig2D::initBuckets(blImageSize imageSize, int shapeMaxLength){

    m_shapeMaxLength = shapeMaxLength;

    m_numBucketsX = int(float(imageSize.xSize())/floor(float(m_shapeMaxLength)+0.5));
    m_numBucketsY = int(float(imageSize.ySize())/floor(float(m_shapeMaxLength)+0.5));

    if (float(imageSize.xSize()) / float(m_numBucketsX) > floor(float(m_shapeMaxLength)+0.5)){
        m_numBucketsX = m_numBucketsX +1;
    }
    if (float(imageSize.ySize()) / float(m_numBucketsY) > floor(float(m_shapeMaxLength)+0.5)){
        m_numBucketsY = m_numBucketsY +1;
    }

    m_sizeBucketX = floor(float(m_shapeMaxLength)+0.5);
    m_sizeBucketY = floor(float(m_shapeMaxLength)+0.5);

    /*
    std::cout << "m_shapeMaxLength = " << m_shapeMaxLength << std::endl;
    std::cout << "m_numBucketsX = " << m_numBucketsX << std::endl;
    std::cout << "m_numBucketsY = " << m_numBucketsY << std::endl;
    std::cout << "m_sizeBucketX = " << m_sizeBucketX << std::endl;
    std::cout << "m_sizeBucketY = " << m_sizeBucketY << std::endl;
    */
}


// /////////////////////////////////////////////////////////////////
//                   blMppContainerConfigShape2D
// /////////////////////////////////////////////////////////////////
blMppContainerConfigShape2D::blMppContainerConfigShape2D() : blMppContainerConfig2D(){
    m_config = new std::vector<std::vector<blMppShape*> >;
}

blMppContainerConfigShape2D::blMppContainerConfigShape2D(blImageSize imageSize, int shapeMaxLength)
    : blMppContainerConfig2D(imageSize, shapeMaxLength){
    m_config = new std::vector<std::vector<blMppShape*> >;
    m_config->resize(m_numBucketsX*m_numBucketsY);
}

blMppContainerConfigShape2D::~blMppContainerConfigShape2D(){
    for (int i = 0 ; i < m_config->size() ; ++i){
        for (int j = 0 ; j < m_config->operator [](i).size() ; ++j){
            delete m_config->operator [](i)[j];
        }
    }
    delete m_config;
}

// getters / setters
void blMppContainerConfigShape2D::setData(std::vector<std::vector<blMppShape*> >* data){
    delete m_config;
    m_config = data;
}

void blMppContainerConfigShape2D::addShape(blMppShape *new_Shape){

    int pos = findClosestBucket(new_Shape);
    //std::cout << "pos bucket = " << pos << "on " << m_config->size() <<  std::endl;
    m_config->operator [](pos).push_back(new_Shape);
    m_size++;
}

std::vector<std::vector<blMppShape *> > *blMppContainerConfigShape2D::configData(){
    return m_config;
}

blMppShape* blMppContainerConfigShape2D::operator[](int ind){
    int nb = 0;
    for (int i = 0 ; i < m_config->size() ; ++i){
        for (int j = 0 ; j < (*m_config)[i].size() ; ++j){
            nb = nb+1;
            if(nb-1==ind){
                return (*m_config)[i][j];
            }
        }
    }
}

// operations
bool blMppContainerConfigShape2D::isInteraction(blMppShape* new_Shape, blMppInteraction *interaction){

    std::vector<int> neighboorsBuckets = getNeighboorBucket(findClosestBucket(new_Shape));

    for (int b = 0 ; b < neighboorsBuckets.size() ; ++b){
        int bucketIdx = neighboorsBuckets[b];
        //std::cout << "bucket id = " << bucketIdx << std::endl;
        for (int s = 0 ; s < m_config->operator [](bucketIdx).size() ; ++s){
            blMppShape* shapeConfig = m_config->operator[](bucketIdx)[s];
            float interactionVal = interaction->compute(new_Shape, shapeConfig);
            if (interactionVal > 0.000001 || interactionVal < 0.000001){
                return true;
            }
        }
    }
    return false;
}

void blMppContainerConfigShape2D::clear(){
    delete m_config;
    m_config = new std::vector<std::vector<blMppShape*> >();
    m_config->resize(m_numBucketsX*m_numBucketsY);
    m_size = 0;
}

// /////////////////////////////////////////////////////////////////
//                   blMppContainerConfigShapeGraph2D
// /////////////////////////////////////////////////////////////////
blMppContainerConfigShapeGraph2D::blMppContainerConfigShapeGraph2D() : blMppContainerConfig2D(){
    m_config = new std::vector<std::vector<blMppShapeGraph*> >;
}

blMppContainerConfigShapeGraph2D::blMppContainerConfigShapeGraph2D(blImageSize imageSize, int shapeMaxLength)
    : blMppContainerConfig2D(imageSize, shapeMaxLength){
    m_config = new std::vector<std::vector<blMppShapeGraph*> >;
    m_config->resize(m_numBucketsX*m_numBucketsY);
}

blMppContainerConfigShapeGraph2D::~blMppContainerConfigShapeGraph2D(){
    for (int i = 0 ; i < m_config->size() ; ++i){
        for (int j = 0 ; j < m_config->operator [](i).size() ; ++j){
            delete m_config->operator [](i)[j];
        }
    }
    delete m_config;
}

// getters / setters
void blMppContainerConfigShapeGraph2D::setData(std::vector<std::vector<blMppShapeGraph*> >* data){
    delete m_config;
    m_config = data;
}

void blMppContainerConfigShapeGraph2D::addShape(blMppShapeGraph *new_Shape){

    int pos = findClosestBucket(new_Shape->shape());
    m_config->operator [](pos).push_back(new_Shape);
    m_size++;
}

std::vector<std::vector<blMppShapeGraph *> > *blMppContainerConfigShapeGraph2D::configData(){
    return m_config;
}

// operations
bool blMppContainerConfigShapeGraph2D::isInteraction(blMppShape* new_Shape, blMppInteraction *interaction){

    std::vector<int> neighboorsBuckets = getNeighboorBucket(findClosestBucket(new_Shape));

    for (int b = 0 ; b < neighboorsBuckets.size() ; ++b){
        int bucketIdx = neighboorsBuckets[b];
        //std::cout << "bucket id = " << bucketIdx << std::endl;
        for (int s = 0 ; s < m_config->operator [](bucketIdx).size() ; ++s){
            blMppShapeGraph* shapeConfig = m_config->operator[](bucketIdx)[s];
            if (interaction->compute(new_Shape, shapeConfig->shape()) > 0 || interaction->compute(new_Shape, shapeConfig->shape()) < 0){
                return true;
            }
        }
    }
    return false;
}

void blMppContainerConfigShapeGraph2D::clear(){
    delete m_config;
    m_config = new std::vector<std::vector<blMppShapeGraph*> >();
    m_config->resize(m_numBucketsX*m_numBucketsY);
    m_size = 0;
}

