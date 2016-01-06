#include "blPfModelDisk.h"
#include <blMath>
#include <blGeometry>

using namespace std;

blPfModelDisk::blPfModelDisk(blPfDataManager* dataManager):blPfModelInterface(dataManager){
    m_particleModelName = "blPfModelDisk";
    m_stateSize = 5;
}

void blPfModelDisk::init(){
    calculateInsideCircleCoordinates();
    calculateCircleBorderCoordinales();

    // Get the image size
    m_nl = m_dataManager->getFrameNl();
    m_nc = m_dataManager->getFrameNc();
    m_stateSize = 5;
}

void blPfModelDisk::setSigmaPredictionX(int sigma){
    m_VarPreditionNoiseX = sigma;
}

void blPfModelDisk::setSigmaPredictionY(int sigma){
    m_VarPreditionNoiseY = sigma;
}

void blPfModelDisk::setObjectRadius(int radius){
    m_objectRadius = radius;
}

void blPfModelDisk::setMaxMoveX(float maxMoveX){
    m_maxMoveX = maxMoveX;
}

void blPfModelDisk::setMaxMoveY(float maxMoveY){
    m_maxMoveY = maxMoveY;
}


// Virtual functions to implement for a specific model
void blPfModelDisk::setCurentFrameIdx(int frameId){
    m_frameId = frameId;
    m_dataManager->loadFrame(m_frameId);
}

vector<blPfParticle*> blPfModelDisk::getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx){

    m_startingFrameIdx = startingFrameIdx;
    // Get the initial point
    m_initX = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[0];
    m_initY = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[1];
    cout << "initial state = " << m_initX << ", " << m_initY << endl;

    vector<blPfParticle*> particles; particles.resize(particleNumber);
    vector<float> statedata; statedata.resize(m_stateSize);
    statedata[0] = startingFrameIdx;
    statedata[1] = m_initX;
    statedata[2] = m_initY;
    statedata[3] = 0.0;
    statedata[4] = 0.0;
    for (int i=0 ; i < particleNumber ; ++i){
        particles[i] = new blPfParticle(new blPfState(statedata));
        //particles[i]->keepOnlyLastnStates(2);
    }
    cout << "blPfModelDisk::GetInitialStates end" << endl;
    return particles;
}

void blPfModelDisk::particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState){

    blPfState* new_state;
    //cout << "mframeId = " << m_frameId << endl;

    blPfState* state = particle->getStateAt(m_frameId-1);
    // Move the position:
    float test = float(rand()) / float(RAND_MAX+1.0);
    float x_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_VarPreditionNoiseX);
    float y_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_VarPreditionNoiseY);
    // 1- by adding noise
    if (test < 0.5){
        new_state = new blPfState(m_stateSize);
        new_state->setStateAt(0, state->getStateAt(0) + 1);
        new_state->setStateAt(1, state->getStateAt(1) + x_dev);
        new_state->setStateAt(2, state->getStateAt(2) + y_dev);
    }
    // 2- by adding vellocity + noise
    else{
        new_state = new blPfState(m_stateSize);
        new_state->setStateAt(0, state->getStateAt(0) + 1);
        new_state->setStateAt(1, state->getStateAt(1) + selectedPreviousState->getStateAt(3) + x_dev);
        new_state->setStateAt(2, state->getStateAt(2) + selectedPreviousState->getStateAt(4) + y_dev);
    }

    // Update the particle states
    float vx, vy;
    vx = new_state->getStateAt(1) -  selectedPreviousState->getStateAt(1);
    vy = new_state->getStateAt(2) -  selectedPreviousState->getStateAt(2);
    new_state->setStateAt(3, vx);
    new_state->setStateAt(4, vy);


    particle->addState(new_state);
    //return particle;
}

float blPfModelDisk::calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates){


    //return 0.0;
    //cout << "blPfModelDisk::CalculateParticuleWeight -> begin " << endl;
    int xc = int(particle->getStateAt(m_frameId)->getStateAt(1));
    int yc = int(particle->getStateAt(m_frameId)->getStateAt(2));

    blPfState* previousSelectedState = previousSelectedStates->getLastState();
    if ( fabs(previousSelectedState->getStateAt(1)-xc) > m_maxMoveX || fabs(previousSelectedState->getStateAt(2)-yc) > m_maxMoveY){
        return 0.0;
    }

    if ( xc-(m_objectRadius+1) <0 || xc+m_objectRadius+1 >= m_dataManager->getFrameNl()
         || yc-(m_objectRadius+1) <0 || yc+m_objectRadius+1 >= m_dataManager->getFrameNc()
         ){
        return 0.0;
    }
    // Calculate pixels values inside the circle
    Float2DImage::IndexType pixelIndex;
    Float2DImage::Pointer curentImage = m_dataManager->getCurentFrame()->itkFloat2DImagePointer();

    vector<float> insideValues; insideValues.resize(m_insidePixels.size());
    for (unsigned int i = 0 ; i < m_insidePixels.size() ; ++i){
        pixelIndex[0] = m_insidePixels[i][0] + xc;
        pixelIndex[1] = m_insidePixels[i][1] + yc;
        insideValues[i] = curentImage->GetPixel(pixelIndex);
    }

    // Calculate pixels values in the border the circle
    vector<float> borderValues; borderValues.resize(m_borderPixels.size());
    for (unsigned int i = 0 ; i < m_borderPixels.size() ; ++i){
        pixelIndex[0] = m_borderPixels[i][0] + xc;
        pixelIndex[1] = m_borderPixels[i][1] + yc;
        borderValues[i] = curentImage->GetPixel(pixelIndex);
    }

    //cout << "blPfModelDisk::CalculateParticuleWeight -> end " << endl;

    float dist = blDistance::bhattacharyya(insideValues, borderValues);
    if (blMath::isNan(dist)){
        return 0;
    }
    return dist;
}

blPfState* blPfModelDisk::selectAndUpdateParticle(vector<blPfParticle*> particles, vector<float> weights){
    // select the mean position  or max weight?
    // Select the max weight

    int posMax = blMathVector::maxPos(weights);
    return particles[posMax]->getStateAt(m_frameId);
}

int blPfModelDisk::getOrderMarkovModel(){
    return 2;
}

// --------------------------------- internal methods --------------------------------
void blPfModelDisk::calculateInsideCircleCoordinates(){
    vector<int> center; center.resize(2); center[0] = 0; center[1] = 0;
    m_insidePixels.push_back(center);
    for (int radius = 1 ; radius <= m_objectRadius ; ++radius ){
        blMathGeometry::CalculateCircleCoordinates(0, 0, radius, m_insidePixels);
    }
    //cout << "inside pixel size = " << m_insidePixels.size() << endl;
}

void blPfModelDisk::calculateCircleBorderCoordinales(){
    blMathGeometry::CalculateCircleCoordinates(0, 0, m_objectRadius+1, m_borderPixels);
    //cout << "border pixel size = " << m_borderPixels.size() << endl;
}
