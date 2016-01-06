#include "blPfModelAxonBaseRetract.h"
#include <blFiltering/blFrangiFilter.h>
#include <blMath>
#include <blGeometry>

using namespace std;

// /////////////////////////////////////////////// //
//                blPfModelAxonBaseRetract
// /////////////////////////////////////////////// //
//---------------------------------------------------
blPfModelAxonBaseRetract::blPfModelAxonBaseRetract(blPfDataManager* dataManager)
    :blPfModelInterface(dataManager){

    cout << "blPfModelAxonBaseRetract constructor" << endl;
    m_particleModelName = "blPfModelAxonBaseRetract";


}

void blPfModelAxonBaseRetract::init(){
    m_stateSize = 9;
    m_minMinor = 2;
    m_maxMinor = 3;
    m_minMajor = 3;
    m_maxMajor = 5;
    m_radius = m_maxMajor;
}

//---------------------------------------------------
/*
bool blPfModelAxonBaseRetract::checkParameters(){
    if (m_processParameters){
        m_SigmaPredictionNoiseX = m_processParameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseX", 5.0);
        m_SigmaPredictionNoiseY = m_processParameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseY", 5.0);
        m_SigmaPredictionNoiseZ = m_processParameters->getValueOfKey<float>("blPfModelAxon_SigmaPredictionNoiseZ", 0.5);
        m_alpha = m_processParameters->getValueOfKey<float>("blPfModelAxon_alpha", 0.7);
        m_beta = m_processParameters->getValueOfKey<float>("blPfModelAxon_beta", 2.5);
        m_gamma = m_processParameters->getValueOfKey<float>("blPfModelAxon_gamma", 0.25);
        m_PredictionProportionStatic = m_processParameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionStatic", 0.5);
        m_PredictionProportionDynamic = m_processParameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDynamic", 0.25);
        m_PredictionProportionDetection = m_processParameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDetection", 0.0);
        m_proportionDynamicReverse = m_processParameters->getValueOfKey<float>("blPfModelAxon_PredictionProportionDynamicReverse", 0.0);
        m_likelihoodWindowSizeX = m_processParameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeX", 5);
        m_likelihoodWindowSizeY = m_processParameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeY", 5);
        m_likelihoodWindowSizeZ = m_processParameters->getValueOfKey<int>("blPfModelAxon_LikelihoodWindowSizeZ", 2);
        m_maxMoveX = m_processParameters->getValueOfKey<int>("blPfModelAxon_MaxMoveX", 15);
        m_maxMoveY = m_processParameters->getValueOfKey<int>("blPfModelAxon_MaxMoveY", 15);
        m_maxMoveZ = m_processParameters->getValueOfKey<int>("blPfModelAxon_MaxMoveZ", 2);

        m_vesselnessImagesPath = m_processParameters->getValueOfKey<string>("blPfModelAxon_VesselnessPath", "");

        if (m_PredictionProportionStatic+m_PredictionProportionDynamic+ m_proportionDynamicReverse +m_PredictionProportionDetection!=1){
            m_paramMessage = "blPfModelAxonRetract -> Predictions proportuiondsmust sum to 1";
            return false;
        }
        //cout << "parameters blPfModelAxonBaseRetract:" << endl;
        //cout << "m_PredictionProportionStatic = " << m_PredictionProportionStatic << endl;
        //cout << "m_PredictionProportionDynamic = " << m_PredictionProportionDynamic << endl;
        //cout << "m_PredictionProportionDetection = " << m_PredictionProportionDetection << endl;
        return true;
    }
    else{
        m_paramMessage = "blPfModelAxon -> Cannot load the parameters";
        return false;
    }
}
*/

// Virtual functions to implement for a specific model
//---------------------------------------------------
void blPfModelAxonBaseRetract::setCurentFrameIdx(int frameId){

    //cout << "blPfModelAxon::SetCurentFrameIdx------------------------------------------------------- begin" << endl;
    m_frameId = frameId;
    cout << "SetCurentFrameIdx -> curent frame: " << m_frameId << endl;
    m_dataManager->loadFrame(m_frameId);
    m_mask = m_dataManager->loadMask(m_frameId)->itkFloat3DImagePointer();

    // Calculate vesselness
    if (frameId == m_startingFrameIdx+1){
        // load vesselness previous
        //cout << "load previous vesselness" << endl;
        string frameName = m_dataManager->getFrameNameAt(m_frameId-1);
        string rootPath = "F:\\experiments\\film_04_05_z1\\RGSeq\\vesselness\\";
        cout << "vesselness previous image = " << rootPath+frameName << endl;
        m_vesselnessImagePrevious = blImageIO::loadToFloat3DImage(m_vesselnessImagesPath+frameName);
    }
    else{
        cout << "Copy vesselness for previous" << endl;
        m_vesselnessImagePrevious = m_vesselnessImage;
    }
    // load vesselness
    //cout << "load vesselness" << endl;
    string frameName = m_dataManager->getFrameNameAt(m_frameId);
    string rootPath = "F:\\experiments\\film_04_05_z1\\RGSeq\\vesselness\\";
    cout << "vesselness image = " << rootPath+frameName << endl;
    m_vesselnessImage = blImageIO::loadToFloat3DImage(m_vesselnessImagesPath+frameName);




    //cout << "blPfModelAxon::frangi filter begin" << endl;
    //BasicImageProcessing::FrangiFilter3D *frangi = new BasicImageProcessing::FrangiFilter3D();
    // //Float3DImage::Pointer blImageIO::LoadToFloat3DImage(m_framesPath[frame2]);
    //Float3DImage::Pointer imageFrame = m_dataManager->Get3DFrameAt(m_frameId);
    //frangi->SetInput(imageFrame);
    //frangi->SetScaleRange(1, 2);
    //frangi->SetScaleRation(1);
    //frangi->SetWeights(0.5, 0.5, 5);
    //frangi->SetBlackRides(false);
    //frangi->Update();
    //m_vesselnessImage = frangi->GetOutput();
    //cout << "blPfModelAxon::frangi filter end" << endl;

    //cout << "save vesselness" << endl;
    //string frameName = m_dataManager->GetFrameNameAt(m_frameId);
    //string rootPath = "F:\\experiments\\film_04_05_z1\\RGSeq\\vesselness\\";
    //cout << "save vesselness image = " << rootPath+frameName << endl;
    //blImageIO::SaveFloat3DImage(rootPath+frameName, m_vesselnessImage);
}

void blPfModelAxonBaseRetract::setCurentObjectNeighborhood(blPfState* selectedPreviousState){
    blPfModelInterface::setCurentObjectNeighborhood(selectedPreviousState);

    if (m_PredictionProportionDetection > 0.0){
        //cout << "search neigbooring detections " << endl;
        // find the detections in the neighboorhood of the curent position
        int x = m_selectedPreviousState->getStateAt(1);
        int y = m_selectedPreviousState->getStateAt(2);
        int z = m_selectedPreviousState->getStateAt(3);
        vector<vector<float> > curentFrameObjects = m_dataManager->getObjectsAt(m_frameId);
        m_neighborObjects.clear();
        for (unsigned int obj = 0 ; obj < curentFrameObjects.size() ; ++obj){
            if (fabs(curentFrameObjects[obj][2] - z) <= m_maxMoveZ){
                if (fabs(curentFrameObjects[obj][0] - x) <= m_maxMoveX && fabs(curentFrameObjects[obj][1] - y) <= m_maxMoveY){
                    m_neighborObjects.push_back(curentFrameObjects[obj]);
                }
            }
        }
        //cout << "found " << m_neighborObjects.size() << "neigbor detection!" << endl;
    }

}

//---------------------------------------------------
vector<blPfParticle*> blPfModelAxonBaseRetract::getInitialStates(int particleNumber, int startingFrameIdx, int objectIdx){

    m_startingFrameIdx = startingFrameIdx;
    // Get the initial point
    m_initX = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[0];
    m_initY = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[1];
    m_initZ = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[2];
    m_initMinor = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[3];
    m_initMajor = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[4];
    m_initTheta = m_dataManager->getObjectsAt(startingFrameIdx, objectIdx)[5];

    vector<blPfParticle*> particles; particles.resize(particleNumber);
    vector<float> statedata; statedata.resize(m_stateSize);
    statedata[0] = m_startingFrameIdx;     // frameIdx
    statedata[1] = m_initX; // x
    statedata[2] = m_initY; // y
    statedata[3] = m_initZ; // z
    statedata[4] = 0.0;     // vx
    statedata[5] = 0.0;     // vy
    statedata[6] = 0.0;     // vz
    statedata[7] = 0.0;     // theta
    statedata[8] = 0; // retraction boolean; 1: for retraction, 0: else

    for (int i=0 ; i < particleNumber ; ++i){
        particles[i] = new blPfParticle(new blPfState(statedata));
        //particles[i]->keepOnlyLastnStates(2);
    }
    return particles;
}

//---------------------------------------------------
void blPfModelAxonBaseRetract::particlePropagate(blPfParticle* particle, blPfState *selectedPreviousState){


    blPfState* new_state = new blPfState(m_stateSize);;
    blPfState* state = particle->getStateAt(m_frameId-1);
    float x_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseX);
    float y_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseY);
    float z_dev = blMath::randSign()*(float(rand()) / float(RAND_MAX+1.0) )*(m_SigmaPredictionNoiseZ);
    float thetaDetection;
    bool use_retraction = false;
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
        new_state->setStateAt(3, state->getStateAt(3) + z_dev);
    }
    // 2- by moving in the velocity direction
    else if(test>= m_PredictionProportionStatic && test < m_PredictionProportionStatic + m_PredictionProportionDynamic){
        //new_state = new blPfState(m_stateSize);

        if (m_frameId > 1){
            // Move the position:
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + state->getStateAt(4) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + state->getStateAt(5) + y_dev);
            new_state->setStateAt(3, state->getStateAt(3) + state->getStateAt(6) + z_dev);
        }
        else{
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + y_dev);
            new_state->setStateAt(3, state->getStateAt(3) + z_dev);
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
            new_state->setStateAt(3, m_neighborObjects[pos][2]);
            thetaDetection = m_neighborObjects[pos][5];
            useDetection = true;
        }
        else{
            // sample around the previous position
            //new_state = new blPfState(m_stateSize);
            new_state->setStateAt(0, state->getStateAt(0) + 1);
            new_state->setStateAt(1, state->getStateAt(1) + x_dev);
            new_state->setStateAt(2, state->getStateAt(2) + y_dev);
            new_state->setStateAt(3, state->getStateAt(3) + z_dev);
        }
    }
    // 4- by moving retraction direction
    else if(test >= 1- m_proportionDynamicReverse){
        use_retraction = true;
        // Calculate the coordinates in the line
        vector<int> startPoint, endPoint;
        startPoint.resize(3); endPoint.resize(3);
        startPoint[0] = state->getStateAt(1) - state->getStateAt(4);
        startPoint[1] = state->getStateAt(2) - state->getStateAt(5);
        startPoint[2] = state->getStateAt(3) - state->getStateAt(6);
        endPoint[0] = state->getStateAt(1);
        endPoint[1] = state->getStateAt(2);
        endPoint[2] = state->getStateAt(3);
        vector<int> lineX, lineY, lineZ;
        blMathGeometry::Calculate3dLineCoordinates(lineX, lineY, lineZ, startPoint, endPoint);

        int pixNumber = lineX.size();
        int pos = rand() % pixNumber + 0;

        //new_state = new blPfState(m_stateSize);
        new_state->setStateAt(0, state->getStateAt(0) + 1);
        new_state->setStateAt(1, lineX[pos] + blMath::rand10_1());
        new_state->setStateAt(2, lineY[pos] + blMath::rand10_1());
        new_state->setStateAt(3, lineZ[pos] + blMath::rand10_1());
    }


    // update all the states
    // get velocity
    float vx = new_state->getStateAt(1) -  state->getStateAt(1);
    float vy = new_state->getStateAt(2) -  state->getStateAt(2);
    float vz = new_state->getStateAt(3) -  state->getStateAt(3);
    // get angle
    float theta;
    if (useDetection){
        theta = thetaDetection;
    }
    else{
        theta = atan2(-vy, vx);
    }
    if (use_retraction){
        new_state->setStateAt( 8, 1);
    }
    else{
        new_state->setStateAt( 8, 0);
    }
    // update the particle
    new_state->setStateAt( 4, vx);
    new_state->setStateAt( 5, vy);
    new_state->setStateAt( 6, vz);
    new_state->setStateAt( 7, theta);

    particle->addState(new_state);
    //return particle;
}

//---------------------------------------------------
float blPfModelAxonBaseRetract::calculateParticuleWeight(blPfParticle* particle, float previousWeight, blPfTrack *previousSelectedStates){

    // 0- Check if the particle moved in the expected area
    float previousX = particle->getStateAt(m_frameId-1)->getStateAt(1);
    float previousY = particle->getStateAt(m_frameId-1)->getStateAt(2);
    float previousZ = particle->getStateAt(m_frameId-1)->getStateAt(3);
    float curentX = particle->getStateAt(m_frameId)->getStateAt(1);
    float curentY = particle->getStateAt(m_frameId)->getStateAt(2);
    float curentZ = particle->getStateAt(m_frameId)->getStateAt(3);


    blPfState* previousSelectedState = previousSelectedStates->getLastState();
    if ( fabs(previousSelectedState->getStateAt(1)-curentX) > m_maxMoveX || fabs(previousSelectedState->getStateAt(2)-curentY) > m_maxMoveY || fabs(previousZ-curentZ) > m_maxMoveZ ){
        return 0.0;
    }
    int radius = m_radius+1;
    if ( int(curentX) - radius < 0 || int(curentX) + radius >= m_dataManager->getFrameNl()
         || int(curentY) - radius < 0 || int(curentY) + radius >= m_dataManager->getFrameNc()
         || int(curentZ) < 0 || int(curentZ)  >= m_dataManager->getFrameNz()
         ){
        return 0.0;
    }
    if ( int(previousX) - radius < 0 || int(previousX) + radius >= m_dataManager->getFrameNl()
         || int(previousY) - radius < 0 || int(previousY) + radius >= m_dataManager->getFrameNc()
         || int(previousZ) < 0 || int(previousZ)  >= m_dataManager->getFrameNz()
         ){
        return 0.0;
    }

    Float3DImage::IndexType pixelIndex;
    pixelIndex[0] = previousX;
    pixelIndex[1] = previousY;
    pixelIndex[2] = previousZ;
    if (m_mask->GetPixel(pixelIndex) > 200){
        //cout << "refuse particle in mask" << endl;
        return 0.0;
    }

    // 1- Cacluate observation likelihood
    float likelihood_weight = calculateObservationLikelihood(particle);
    if (likelihood_weight < 1e-14) return 0.0;

    // 2-  Caclulate veselness
    float vesselness_weight = calculateVesselness(particle);

    // 3 = Calculate motion model
    float motion_weight = calculateMotionModel(particle);

    // return global weight
    return likelihood_weight*vesselness_weight*motion_weight;
}

//---------------------------------------------------
blPfState* blPfModelAxonBaseRetract::selectAndUpdateParticle(vector<blPfParticle*> particles, vector<float> weights){
    // Select the max weight
    int posMax = blMathVector::maxPos(weights);
    return particles[posMax]->getStateAt(m_frameId);
}

// parameters setters
//---------------------------------------------------
void blPfModelAxonBaseRetract::setSigmaPredictionNoise(float sigmaX, float sigmaY, float sigmaZ ){
    m_SigmaPredictionNoiseX = sigmaX;
    m_SigmaPredictionNoiseY = sigmaY;
    m_SigmaPredictionNoiseZ = sigmaZ;
}

//---------------------------------------------------
void blPfModelAxonBaseRetract::setModelParameters(float alpha, float beta, float gamma){
    m_alpha = alpha;
    m_beta = beta;
    m_gamma = gamma;
}

//---------------------------------------------------
void blPfModelAxonBaseRetract::setPredictionParameters(float proportionStatic, float proportionDynamic, float proportionDynamicReverse, float proportionDetection){
    m_PredictionProportionStatic = proportionStatic;
    m_PredictionProportionDynamic = proportionDynamic;
    m_PredictionProportionDetection = proportionDetection;
    m_proportionDynamicReverse = proportionDynamicReverse;

    if (m_PredictionProportionStatic + m_PredictionProportionDynamic + m_proportionDynamicReverse + m_PredictionProportionDetection != 1.0){
        throw blException(("blPfModelAxon: The prediction proportions MUST sum to 1"));
    }
}

//---------------------------------------------------
void blPfModelAxonBaseRetract::setMaxMove(int maxMoveX, int maxMoveY, int maxMoveZ){
    m_maxMoveX = maxMoveX;
    m_maxMoveY = maxMoveY;
    m_maxMoveZ = maxMoveZ;
}

void blPfModelAxonBaseRetract::setPredictionWindowSize(int x, int y, int z){
    m_likelihoodWindowSizeX = x;
    m_likelihoodWindowSizeY = y;
    m_likelihoodWindowSizeZ = z;
}

void blPfModelAxonBaseRetract::setVesselnessImagesPath(std::string path){
    m_vesselnessImagesPath = path;
}

// -------------------------- Internal function
//---------------------------------------------------

float blPfModelAxonBaseRetract::calculateObservationLikelihood(blPfParticle* particle){

    vector<float> energy;
    for (int i = m_minMinor ; i < m_maxMinor ; ++i){
        for (int j = m_minMajor; j < m_maxMajor ; ++j){
            energy.push_back(calculateObservationLikelihoodBhattacharyyaEllipse(particle,i,j));
        }
    }

    return blMathVector::max(energy);
}

float blPfModelAxonBaseRetract::calculateObservationLikelihoodBhattacharyyaEllipse(blPfParticle *particle, float minor, float major){
    int centerX = particle->getStateAt(m_frameId)->getStateAt(1);
    int centerY = particle->getStateAt(m_frameId)->getStateAt(2);
    int centerZ = particle->getStateAt(m_frameId)->getStateAt(3);
    //float minor = particle->getStateAt(m_frameId)->getStateAt(8);
    //float major = particle->getStateAt(m_frameId)->getStateAt(9);
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
    Float3DImage::Pointer image = m_dataManager->getCurentFrame()->itkFloat3DImagePointer();
    Float3DImage::IndexType pixelIndex;
    pixelIndex[2] = centerZ;
    for (unsigned int i = 0 ; i < outsidePixels.size() ; ++i){
        pixelIndex[0] = outsidePixels[i][0];
        pixelIndex[1] = outsidePixels[i][1];
        outsideValues[i] = image->GetPixel(pixelIndex);
    }

    // 2- Partition the border into 4 regions
    vector<float> vhead1,vhead2, vside1, vside2;
    partitionEllipseIn3D(object, outsidePixels, outsideValues, vhead1,vhead2, vside1, vside2 );

    // 3- Calculate the Bhattacharyya distance for each region
    vector<float> valuesIn;
    getInsideEllipsePixelsValues(object, valuesIn);

    float dB_head1 = blDistance::bhattacharyya(valuesIn, vhead1);
    float dB_head2 = blDistance::bhattacharyya(valuesIn, vhead2);
    float dB_side1 = blDistance::bhattacharyya(valuesIn, vside1);
    float dB_side2 = blDistance::bhattacharyya(valuesIn, vside2);

    float mD_head1 = blMathVector::mean(valuesIn) - blMathVector::mean(vhead1);
    float mD_head2 = blMathVector::mean(valuesIn) - blMathVector::mean(vhead2);
    float mD_side1 = blMathVector::mean(valuesIn) - blMathVector::mean(vside1);
    float mD_side2 = blMathVector::mean(valuesIn) - blMathVector::mean(vside2);
    if (mD_head1 < 0){dB_head1=0;}
    if (mD_head2 < 0){dB_head2=0;}
    if (mD_side1 < 0){dB_side1=0;}
    if (mD_side2 < 0){dB_side2=0;}

    // 4- Makes the decision
    float dB_head = max(dB_head1, dB_head2);
    // take the min
    float dB = dB_side1;
    if (dB_side2<dB)
        dB = dB_side2;
    if (dB_head<dB)
        dB=dB_head;

    if (blMath::isNan(dB)){
        return 0;
    }
    return 1-exp(-m_alpha*dB);

}

void blPfModelAxonBaseRetract::getInsideEllipsePixelsValues(const vector<float> object, vector<float> &insideValues){
    int margin = 2*object[4] + 2;
    float X, Y;
    float value;
    float cosi = cos(object[5]);
    float sinu = sin(object[5]);
    float a2 = object[4]*object[4]+0.1, b2 = object[3]*object[3]+0.1;
    Float3DImage::IndexType pixelIndex;
    pixelIndex[2] = object[2];

    Float3DImage::Pointer image = m_dataManager->getCurentFrame()->itkFloat3DImagePointer();
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
void blPfModelAxonBaseRetract::getEllipseBorderPixelsCoordinates(vector<float> object, vector<vector<int> > &outsidePixels){
    int margin = 2*object[4] + 2*1 + 2;
    float X, Y;
    float value, valueOut;
    float aOut = object[4] + 1.0 +0.1; float bOut = object[3] + 1.0+0.1;
    float aOut2 = aOut*aOut; float bOut2 = bOut*bOut;
    float cosi = cos(object[5]);
    float sinu = sin(object[5]);
    float a2 = object[4]*object[4]+0.1, b2 = object[3]*object[3]+0.1;
    vector<int> pixel; pixel.resize(3);
    pixel[2] = object[2];

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

float blPfModelAxonBaseRetract::calculateVesselness(blPfParticle* particle){
    // 1- calculate the points coordinales
    vector<int> lineX, lineY, lineZ;
    vector<int> startPoint;
    startPoint.push_back(particle->getStateAt(m_frameId-1)->getStateAt(1)); // x
    startPoint.push_back(particle->getStateAt(m_frameId-1)->getStateAt(2)); // y
    startPoint.push_back(particle->getStateAt(m_frameId-1)->getStateAt(3)); // z
    vector<int> endPoint;
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(1)); // x
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(2)); // y
    endPoint.push_back(particle->getStateAt(m_frameId)->getStateAt(3)); // z
    blMathGeometry::Calculate3dLineCoordinates(lineX, lineY, lineZ, startPoint, endPoint);

    Float3DImage::IndexType pixelIndex;


    if (particle->getStateAt(m_frameId)->getStateAt(8) < 1){
        // 2- calculate the mean vesselness
        float vesselness = 0.0;
        for (unsigned int i = 0 ; i < lineX.size() ; ++i){
            pixelIndex[0] = lineX[i];
            pixelIndex[1] = lineY[i];
            pixelIndex[2] = lineZ[i];
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
    else {
        // 2- calculate the mean vesselness difference between frames
        float vesselness = 0.0;
        float vesselnessPrevious = 0.0;
        for (unsigned int i = 0 ; i < lineX.size() ; ++i){
            pixelIndex[0] = lineX[i];
            pixelIndex[1] = lineY[i];
            pixelIndex[2] = lineZ[i];
            vesselness += m_vesselnessImage->GetPixel(pixelIndex);
            vesselnessPrevious += m_vesselnessImagePrevious->GetPixel(pixelIndex);
        }
        vesselness /= lineX.size();
        vesselnessPrevious /= lineX.size();

        if (vesselnessPrevious < vesselness){
            return 0.0;
        }

        vesselness = vesselnessPrevious - vesselness;
        // 3- Calculate the vesselness model
        float term = exp(-m_beta*(1.0-vesselness/255.0));
        //cout << "vesselness term = " << term << ", " << vesselness << ", " << lineX.size() << endl;
        if (blMath::isNan(term))
            return 0.0;
        else
            return term;
    }
}

//---------------------------------------------------
float blPfModelAxonBaseRetract::calculateMotionModel(blPfParticle *particle){

    // velocity previous frame
    float vx_before = particle->getStateAt(m_frameId-1)->getStateAt(4); // vx
    float vy_before = particle->getStateAt(m_frameId-1)->getStateAt(5); // vy
    float vz_before = particle->getStateAt(m_frameId-1)->getStateAt(6); // vz
    // velocity curent frame
    float vx_curent = particle->getStateAt(m_frameId)->getStateAt(4); // vx
    float vy_curent = particle->getStateAt(m_frameId)->getStateAt(5); // vy
    float vz_curent = particle->getStateAt(m_frameId)->getStateAt(6); // vz

    vector<float> crossProd = blMathVector::cross(vx_before, vy_before, vz_before, vx_curent, vy_curent, vz_curent);
    float normCross = sqrt( crossProd[0]*crossProd[0] + crossProd[1]*crossProd[1] + crossProd[2]*crossProd[2] );
    float dotProd = vx_before*vx_curent + vy_before*vy_curent + vz_before*vz_curent;

    float angle = fabs(atan2(normCross, dotProd));

    //cout << "motion term = " << exp(-m_gamma*angle/M_PI) << ", " << angle << endl << endl;
    return exp(-m_gamma*angle/M_PI);
}

void blPfModelAxonBaseRetract::partitionEllipseIn3D( vector<float> object, vector<vector<int> > borderPixels, vector<float> outsideValues, vector<float> &vhead1, vector<float> &vhead2, vector<float> &vside1, vector<float> &vside2){

    float cosi = cos(object[5]);
    float sinu = sin(object[5]);
    float a = object[4];
    float b = object[3];
    float xc = object[0];
    float yc = object[1];

    vector<float> p_large1; p_large1.resize(2);
    p_large1[0] = -cosi*a + xc;
    p_large1[1] = sinu*a + yc;
    vector<float> p_large2; p_large2.resize(2);
    p_large2[0] = cosi*a + xc;
    p_large2[1] = -sinu*a + yc;
    vector<float> p_small1; p_small1.resize(2);
    p_small1[0] =  sinu*b + xc;
    p_small1[1] =  cosi*b + yc;
    vector<float> p_small2; p_small2.resize(2);
    p_small2[0] =  -sinu*b + xc;
    p_small2[1] =  -cosi*b + yc;

    vector<float> d; d.resize(4);
    int pos;
    vhead1.clear(); vhead2.clear(); vside1.clear(); vside2.clear();
    for (unsigned int i=0 ; i < borderPixels.size() ; ++i){
        d[0] = blDistance::euclidian_square(p_large1[0], p_large1[1], borderPixels[i][0], borderPixels[i][1]);
        d[1] = blDistance::euclidian_square(p_large2[0], p_large2[1], borderPixels[i][0], borderPixels[i][1]);
        d[2] = blDistance::euclidian_square(p_small1[0], p_small1[1], borderPixels[i][0], borderPixels[i][1]);
        d[3] = blDistance::euclidian_square(p_small2[0], p_small2[1], borderPixels[i][0], borderPixels[i][1]);

        pos =  blMathVector::minPos(d);

        if (pos == 0){
            vhead1.push_back( outsideValues[i] );
        }
        else if (pos==1){
            vhead2.push_back( outsideValues[i] );
        }
        else if (pos==2){
            vside1.push_back( outsideValues[i] );
        }
        else{
            vside2.push_back( outsideValues[i] );
        }
    }

}
