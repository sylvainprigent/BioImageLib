#include "blPfModelLinearMotion.h"
#include <blMath>
#include <blGeometry>

using namespace std;

// /////////////////////////////////////////////// //
//                blPfModelLinearMotion
// /////////////////////////////////////////////// //
//---------------------------------------------------
blPfModelLinearMotion::blPfModelLinearMotion(blPfDataManager* dataManager)
    :blPfModelInterface(dataManager){

    cout << "blPfModelLinearMotion constructor" << endl;
    m_particleModelName = "blPfModelLinearMotion";
}

void blPfModelLinearMotion::init(){
    m_stateSize = 10;
    //m_minMinor = 2;
    //m_maxMinor = 3;
    //m_minMajor = 3;
    //m_maxMajor = 5;
    m_radius = m_maxMajor;

    std::cout << "appearance = " << m_minMinor << ", " << m_maxMinor <<  ", " << m_minMajor << ", " << m_maxMajor << std::endl;
 }

void blPfModelLinearMotion::setAppearanceSizes(int minMinor, int maxMinor, int minMajor, int maxMajor){
    m_minMinor = minMinor;
    m_maxMinor = maxMinor;
    m_minMajor = minMajor;
    m_maxMajor = maxMajor;
}

// Virtual functions to implement for a specific model
//---------------------------------------------------
int blPfModelLinearMotion::getOrderMarkovModel(){
    return 2;
}

void blPfModelLinearMotion::setCurentFrameIdx(int frameId){

    //cout << "blPfModelLinearMotion::SetCurentFrameIdx------------------------------------------------------- begin" << endl;
    m_frameId = frameId;
    cout << "SetCurentFrameIdx -> curent frame: " << m_frameId << endl;
    m_dataManager->loadFrame(m_frameId);
    //m_mask = m_dataManager->loadMask(m_frameId)->itkFloat2DImagePointer();

    // load vesselness
    //cout << "load vesselness" << endl;
    string frameName = m_dataManager->getFrameNameAt(m_frameId);
    frameName.replace(frameName.end()-4,frameName.end(),".png");
    m_vesselnessImage = blImageIO::loadToFloat2DImage(m_vesselnessImagesPath+frameName);

    //cout << "blPfModelLinearMotion::SetCurentFrameIdx------------------------------------------------------- end" << endl;


}

void blPfModelLinearMotion::setCurentObjectNeighborhood(blPfState* selectedPreviousState){
    blPfModelInterface::setCurentObjectNeighborhood(selectedPreviousState);

    //std::cout << "blPfModelLinearMotion::setCurentObjectNeighborhood --------- done "<< std::endl;

    if (m_PredictionProportionDetection > 0.0){
        //cout << "search neigbooring detections " << endl;
        // find the detections in the neighboorhood of the curent position
        int x = m_selectedPreviousState->getStateAt(1);
        int y = m_selectedPreviousState->getStateAt(2);
        int z = m_selectedPreviousState->getStateAt(3);
        vector<vector<float> > curentFrameObjects = m_dataManager->getObjectsAt(m_frameId);
        m_neighborObjects.clear();
        for (unsigned int obj = 0 ; obj < curentFrameObjects.size() ; ++obj){
            if (fabs(curentFrameObjects[obj][0] - x) <= m_maxMoveX && fabs(curentFrameObjects[obj][1] - y) <= m_maxMoveY){
                m_neighborObjects.push_back(curentFrameObjects[obj]);
            }
        }
        //cout << "found " << m_neighborObjects.size() << "neigbor detection!" << endl;
    }

    //std::cout << "blPfModelLinearMotion::setCurentObjectNeighborhood --------- done "<< std::endl;


}

//---------------------------------------------------
vector<blPfParticle*> blPfModelLinearMotion::getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx){

    //std::cout << "blPfModelLinearMotion::getInitialStates --------- begin "<< std::endl;
    m_startingFrameIdx = startingFrameIdx;
    // Get the initial point
    m_initX = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[0];
    m_initY = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[1];
    //m_initZ = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[2];
    m_initMinor = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[3];
    m_initMajor = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[4];
    m_initTheta = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[5];

    vector<blPfParticle*> particles; particles.resize(particleNumber);
    vector<float> statedata; statedata.resize(m_stateSize);
    statedata[0] = m_startingFrameIdx;     // frameIdx
    statedata[1] = m_initX; // x
    statedata[2] = m_initY; // y
    statedata[3] = 0.0; // z
    statedata[4] = 0.0;     // vx
    statedata[5] = 0.0;     // vy
    statedata[6] = 0.0;     // vz
    statedata[7] = 0.0;     // theta
    statedata[8] = 0.0;     // minor axis
    statedata[9] = 0.0;     // major axis

    for (int i=0 ; i < particleNumber ; ++i){
        particles[i] = new blPfParticle(new blPfState(statedata));
        //particles[i]->keepOnlyLastnStates(2);
    }

    //std::cout << "blPfModelLinearMotion::getInitialStates --------- done " << std::endl;
    return particles;
}

//---------------------------------------------------
void blPfModelLinearMotion::particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState){

    //std::cout << "blPfModelLinearMotion::particlePropagate --------- begin "<< std::endl;

    blPfState* new_state = new blPfState(m_stateSize);;
    blPfState* state = particle->getStateAt(m_frameId-1);
    float x_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseX);
    float y_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseY);
    //float z_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseZ);
    float thetaDetection;
    bool useDetection = false;

    // Move the position:
    float test = float(rand()) / float(RAND_MAX+1.0);
    // 1- by adding noise
    //cout << "cumul coeffs = 1: " << cumulcompt1 << ", 2: " << cumulcompt2 << endl;
    if (test < m_PredictionProportionStatic){
        //new_state = new blPfState(m_stateSize);
        new_state->setStateAt(0, state->getStateAt(0) + 1);
        new_state->setStateAt(1, state->getStateAt(1) + x_dev);
        new_state->setStateAt(2, state->getStateAt(2) + y_dev);
        //new_state->setStateAt(3, state->getStateAt(3) + z_dev);
    }
    // 2- by moving in the velocity direction
    else if(test>= m_PredictionProportionStatic && test < m_PredictionProportionStatic + m_PredictionProportionDynamic){
        //new_state = new blPfState(m_stateSize);

        if (m_frameId > 1){
            // Move the position:
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + state->getStateAt(4) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + state->getStateAt(5) + y_dev);
            //new_state->setStateAt(3, state->getStateAt(3) + state->getStateAt(6) + z_dev);
        }
        else{
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + y_dev);
            //new_state->setStateAt(3, state->getStateAt(3) + z_dev);
        }
    }
    // 3- by moving to some detection
    else if (test >= m_PredictionProportionStatic + m_PredictionProportionDynamic && test < m_PredictionProportionStatic + m_PredictionProportionDynamic + m_PredictionProportionDetection){
        int neighboorsNumber = m_neighborObjects.size();
        if (neighboorsNumber > 0){
            // randomly pick a detection
            int pos = rand() % neighboorsNumber + 0;
            // sample around this detection point
            //new_state = new blPfState(m_stateSize);
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, m_neighborObjects[pos][0]);
            new_state->setStateAt(2, m_neighborObjects[pos][1]);
            //new_state->setStateAt(3, m_neighborObjects[pos][2]);
            thetaDetection = m_neighborObjects[pos][5];
            useDetection = true;
        }
        else{
            // sample around the previous position
            //new_state = new blPfState(m_stateSize);
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + y_dev);
            //new_state->setStateAt(3, state->getStateAt(3) + z_dev);
        }
    }

    // update all the states
    // get velocity
    float vx = new_state->getStateAt(1) -  selectedPreviousState->getStateAt(1);
    float vy = new_state->getStateAt(2) -  selectedPreviousState->getStateAt(2);
    // get angle
    float theta;
    if (useDetection){
        theta = thetaDetection;
    }
    else{
        theta = atan2(-vy, vx);
    }

    // update the particle
    new_state->setStateAt( 4, vx);
    new_state->setStateAt( 5, vy);
    //new_state->setStateAt( 6, vz);
    new_state->setStateAt( 7, theta);

    particle->addState(new_state);
    //return particle;

    //std::cout << "blPfModelLinearMotion::particlePropagate --------- done "<< std::endl;

}

//---------------------------------------------------
float blPfModelLinearMotion::calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates){

    //std::cout << "blPfModelLinearMotion::calculateParticuleWeight --------- begin "<< std::endl;

    // 0- Check if the particle moved in the expected area
    float previousX = particle->getStateAt(m_frameId-1)->getStateAt(1);
    float previousY = particle->getStateAt(m_frameId-1)->getStateAt(2);
    //float previousZ = particle->getStateAt(m_frameId-1)->getStateAt(3);
    float curentX = particle->getStateAt(m_frameId)->getStateAt(1);
    float curentY = particle->getStateAt(m_frameId)->getStateAt(2);
    //float curentZ = particle->getStateAt(m_frameId)->getStateAt(3);

    blPfState* previousSelectedState = previousSelectedStates->getLastState();
    if ( fabs(previousSelectedState->getStateAt(1)-curentX) > m_maxMoveX || fabs(previousSelectedState->getStateAt(2)-curentY) > m_maxMoveY){
        return 0.0;
    }
    int radius = m_radius+1;
    if ( int(curentX) - radius < 0 || int(curentX) + radius >= m_dataManager->getFrameNl()
         || int(curentY) - radius < 0 || int(curentY) + radius >= m_dataManager->getFrameNc()
         ){
        return 0.0;
    }
    if ( int(previousX) - radius < 0 || int(previousX) + radius >= m_dataManager->getFrameNl()
         || int(previousY) - radius < 0 || int(previousY) + radius >= m_dataManager->getFrameNc()
         ){
        return 0.0;
    }

    Float2DImage::IndexType pixelIndex;
    pixelIndex[0] = previousX;
    pixelIndex[1] = previousY;
    /*
    if (m_mask->GetPixel(pixelIndex) > 200){
        //cout << "refuse particle in mask" << endl;
        return 0.0;
    }
    */

    // 1- Cacluate observation likelihood
    float likelihood_weight = calculateObservationLikelihood(particle);
    if (likelihood_weight < 1e-14) return 0.0;

    // 2-  Caclulate veselness
    float vesselness_weight = calculateVesselness(particle, previousSelectedStates);

    // 3 = Calculate motion model
    float motion_weight = calculateMotionModel(particle, previousSelectedStates);

    // 3- Calulate likelihood diff
    float likelihood_diff = calculateObservationDiff(particle, previousSelectedStates);


    float euclidean_dist = calculateEuclideanDist(particle, previousSelectedStates);
    // return global weight

    //std::cout << "blPfModelLinearMotion::calculateParticuleWeight --------- done "<< std::endl;

    //std::cout << "likelihood_weight = " << likelihood_weight << "vesselness_weight = " << vesselness_weight << "motion_weight = " << motion_weight << std::endl;

    //return motion_weight;
    return likelihood_weight*vesselness_weight*motion_weight*likelihood_diff*euclidean_dist;
}

//---------------------------------------------------
blPfState* blPfModelLinearMotion::selectAndUpdateParticle(vector<blPfParticle*> particles, vector<float> weights){
    // Select the max weight
    int posMax = blMathVector::maxPos(weights);
    return particles[posMax]->getStateAt(m_frameId);
}

// parameters setters
//---------------------------------------------------
void blPfModelLinearMotion::setSigmaPredictionNoise(float sigmaX, float sigmaY){
    m_SigmaPredictionNoiseX = sigmaX;
    m_SigmaPredictionNoiseY = sigmaY;
}

//---------------------------------------------------
void blPfModelLinearMotion::setModelParameters(float alpha, float beta, float gamma, float delta, float psi){
    m_alpha = alpha;
    m_beta = beta;
    m_gamma = gamma;
    m_delta = delta;
    m_psi = psi;
}

//---------------------------------------------------
void blPfModelLinearMotion::setPredictionParameters(float proportionStatic, float proportionDynamic, float proportionDetection){
    m_PredictionProportionStatic = proportionStatic;
    m_PredictionProportionDynamic = proportionDynamic;
    m_PredictionProportionDetection = proportionDetection;

    if (m_PredictionProportionStatic + m_PredictionProportionDynamic + m_PredictionProportionDetection != 1.0){
        throw blException(("blPfModelAxon: The prediction proportions MUST sum to 1"));
    }
}

//---------------------------------------------------
void blPfModelLinearMotion::setMaxMove(int maxMoveX, int maxMoveY){
    m_maxMoveX = maxMoveX;
    m_maxMoveY = maxMoveY;
}

void blPfModelLinearMotion::setVesselnessImagesPath(std::string path){
    m_vesselnessImagesPath = path;
}

// -------------------------- Internal function
//---------------------------------------------------
float blPfModelLinearMotion::calculateEuclideanDist(blPfParticle* particle, blPfTrack *previousSelectedStates){

    float x1 = previousSelectedStates->getLastState()->getStateAt(1);
    float y1 = previousSelectedStates->getLastState()->getStateAt(2);

    float x2 = particle->getStateAt(m_frameId)->getStateAt(1);
    float y2 = particle->getStateAt(m_frameId)->getStateAt(2);

    float dist = sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) );

    return exp(-m_psi*dist);
}


float blPfModelLinearMotion::calculateObservationDiff(blPfParticle* particle, blPfTrack *previousSelectedStates){

    float area = particle->getStateAt(m_frameId)->getStateAt(8)*particle->getStateAt(m_frameId)->getStateAt(9);
    float areaBefore = previousSelectedStates->getLastState()->getStateAt(8)*previousSelectedStates->getLastState()->getStateAt(9);


    if (area > areaBefore){
        //std::cout << "areas = " <<  area << ", " << areaBefore << ", ratio = " << areaBefore/area << ", energy = " << exp(-m_delta*(1.0-areaBefore/area)) << std::endl;
        return exp(-m_delta*(1.0-areaBefore/area));
    }
    //std::cout << "areas = " <<  area << ", " << areaBefore << ", ratio = " << area/areaBefore << ", energy = " << exp(-m_delta*(1.0-area/areaBefore)) << std::endl;
    return exp(-m_delta*(1.0-area/areaBefore));
}

float blPfModelLinearMotion::calculateObservationLikelihood(blPfParticle* particle){

    //vector<float> energy;
    float energy = 0;
    float inter;
    for (int i = m_minMinor ; i < m_maxMinor ; ++i){
        for (int j = m_minMajor; j < m_maxMajor ; ++j){
            inter = calculateObservationLikelihoodBhattacharyyaEllipse(particle,i,j);
            if (inter > energy){
                energy = inter;
                particle->setStateAt(particle->size()-1, 8, i);
                particle->setStateAt(particle->size()-1, 9, j);
            }
        }
    }

    return energy;
    //return blMathVector::max(energy);
}

float blPfModelLinearMotion::calculateObservationLikelihoodBhattacharyyaEllipse(blPfParticle *particle, float minor, float major){
    int centerX = particle->getStateAt(m_frameId)->getStateAt(1);
    int centerY = particle->getStateAt(m_frameId)->getStateAt(2);
    int centerZ = particle->getStateAt(m_frameId)->getStateAt(3);
    float theta = particle->getStateAt(m_frameId)->getStateAt(7);

    // Calculate ellipse inside values
    vector<float> object; object.resize(6);
    object[0] = centerX;
    object[1] = centerY;
    object[2] = centerZ;
    object[3] = minor;
    object[4] = major;
    object[5] = theta;

    // Calculate ellipse border partitionned
    // 1- Get the border pixels coordinates and value
    vector<vector<int> > outsidePixels;
    getEllipseBorderPixelsCoordinates(object, outsidePixels);
    vector<float> outsideValues; outsideValues.resize(outsidePixels.size());
    Float2DImage::Pointer image = m_dataManager->getCurentFrame()->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;
    for (unsigned int i = 0 ; i < outsidePixels.size() ; ++i){
        pixelIndex[0] = outsidePixels[i][0];
        pixelIndex[1] = outsidePixels[i][1];
        outsideValues[i] = image->GetPixel(pixelIndex);
    }


    // 3- Calculate the Bhattacharyya distance for each region
    vector<float> valuesIn;
    getInsideEllipsePixelsValues(object, valuesIn);

    float dB = blDistance::bhattacharyya(valuesIn, outsideValues);
    float mD = blMathVector::mean(valuesIn) - blMathVector::mean(outsideValues);
    if (mD < 0){dB=0;}

    // 4- Makes the decision
    if (blMath::isNan(dB)){
        return 0;
    }
    return 1-exp(-m_alpha*dB);

}

void blPfModelLinearMotion::getInsideEllipsePixelsValues(const vector<float> object, vector<float> &insideValues){
    int margin = 2*object[4] + 2;
    float X, Y;
    float value;
    float cosi = cos(object[5]);
    float sinu = sin(object[5]);
    float a2 = object[4]*object[4]+0.1, b2 = object[3]*object[3]+0.1;
    Float2DImage::IndexType pixelIndex;

    Float2DImage::Pointer image = m_dataManager->getCurentFrame()->itkFloat2DImagePointer();
    for (int x = object[0] - margin ; x <= object[0] + margin ; ++x){
        for (int y = object[1] - margin ; y <= object[1] + margin ; ++y){
            X = (x - object[0])*cosi - (y - object[1])*sinu;
            Y = (x-object[0])*sinu + (y - object[1])*cosi;
            value = X*X/(a2) + Y*Y/(b2);
            if (value <= 1.0){
                pixelIndex[0] = x; pixelIndex[1] = y;
                insideValues.push_back(image->GetPixel(pixelIndex));
            }
        }
    }
}

//---------------------------------------------------
void blPfModelLinearMotion::getEllipseBorderPixelsCoordinates(vector<float> object, vector<vector<int> > &outsidePixels){
    int margin = 2*object[4] + 2*1 + 2;
    float X, Y;
    float value, valueOut;
    float aOut = object[4] + 1.0 +0.1; float bOut = object[3] + 1.0+0.1;
    float aOut2 = aOut*aOut; float bOut2 = bOut*bOut;
    float cosi = cos(object[5]);
    float sinu = sin(object[5]);
    float a2 = object[4]*object[4]+0.1, b2 = object[3]*object[3]+0.1;
    vector<int> pixel; pixel.resize(2);

    for (int x = object[0] - margin ; x <= object[0] + margin ; ++x){
        for (int y = object[1] - margin ; y <= object[1] + margin ; ++y){
            X = (x - object[0])*cosi - (y - object[1])*sinu;
            Y = (x-object[0])*sinu + (y - object[1])*cosi;
            value = X*X/(a2) + Y*Y/(b2);
            if (value > 1.0){
                valueOut = X*X/(aOut2) + Y*Y/(bOut2);
                if (valueOut <= 1.0){
                    pixel[0] = x; pixel[1] = y;
                    outsidePixels.push_back(pixel);
                }
            }
        }
    }
}

float blPfModelLinearMotion::calculateVesselness(blPfParticle* particle, blPfTrack *previousSelectedStates){
    // 1- calculate the points coordinales
    vector<int> lineX, lineY;

    vector<int> endPoint;
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(1)); // x
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(2)); // y
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(3)); // z
    blMathGeometry::Calculate2DLineCoordinates(previousSelectedStates->getLastState()->getStateAt(1),
                                               previousSelectedStates->getLastState()->getStateAt(2),
                                               endPoint[0],
                                               endPoint[1],
                                               lineX, lineY);

    Float2DImage::IndexType pixelIndex;

    // 2- calculate the mean vesselness
    float vesselness = 0.0;
    for (unsigned int i = 0 ; i < lineX.size() ; ++i){
        pixelIndex[0] = lineX[i];
        pixelIndex[1] = lineY[i];
        vesselness += m_vesselnessImage->GetPixel(pixelIndex);
    }
    vesselness/=lineX.size();

    // 3- Calculate the vesselness model
    float term = exp(-m_beta*(1.0-vesselness/255.0));
    //cout << "vesselness term = " << term << ", " << vesselness << ", " << lineX.size() << endl;
    if (blMath::isNan(term))
        return 0.0;
    else
        return term;

}

//---------------------------------------------------
float blPfModelLinearMotion::calculateMotionModel(blPfParticle *particle, blPfTrack *previousSelectedStates){


    int trackSize = previousSelectedStates->getTrackSize();
    if (trackSize >= 2){

        previousSelectedStates->getStateAt(trackSize-1);
        // positions previous frame
        float x1 = previousSelectedStates->getStateAt(trackSize-2)->getStateAt(1);
        float y1 = previousSelectedStates->getStateAt(trackSize-2)->getStateAt(2);

        float x2 = previousSelectedStates->getStateAt(trackSize-1)->getStateAt(1);
        float y2 = previousSelectedStates->getStateAt(trackSize-1)->getStateAt(2);

        float x3 = particle->getStateAt(m_frameId)->getStateAt(1);
        float y3 = particle->getStateAt(m_frameId)->getStateAt(2);;


        float a = pow(x3-x2,2) + pow(y3-y2,2);
        float b = pow(x2-x1,2) + pow(y2-y1,2);
        float c = pow(x3-x1,2) + pow(y3-y1,2);
        float angle = acos( (a+b-c) / sqrt(4*a*b) );

        //cout << "motion term = " << exp(-m_gamma*(M_PI-angle)) << ", " << angle << endl << endl;
        return exp(-m_gamma*(M_PI-angle));
    }
    return 1;
}
