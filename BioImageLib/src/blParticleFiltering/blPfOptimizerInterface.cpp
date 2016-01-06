#include "blPfOptimizerInterface.h"

#include "itkRescaleIntensityImageFilter.h"
#include "blGeometry/blMathGeometry.h"
#include "blCastStacks/blProjectionImage.h"

using namespace std;

// //////////////////////////////////////////////////// //
//                blPfOptimizerInterface                //
// //////////////////////////////////////////////////// //
const std::string blPfOptimizerInterface::ResampleMultinomial = "ResampleMultinomial";
const std::string blPfOptimizerInterface::ResampleResidual = "ResampleResidual";
const std::string blPfOptimizerInterface::ResampleStratified = "ResampleStratified";
const std::string blPfOptimizerInterface::ResampleSystematic = "ResampleSystematic";

blPfOptimizerInterface::blPfOptimizerInterface(blPfModelInterface *particleModel, int startingFrameIdx, int objectIdx) : blProcess(){
    m_particleModel = particleModel;
    m_startingFrameIdx = startingFrameIdx;
    m_objectIdx = objectIdx;
}

blPfOptimizerInterface::blPfOptimizerInterface(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, string resamplingMethod, int startingFrameIdx, int objectIdx)
    : blProcess(){

    m_particleModel = particleModel;
    m_Ns = particleNumber;
    m_Nth = resamplingThreshold;
    m_resamplingMethod = resamplingMethod;
    m_startingFrameIdx = startingFrameIdx;
    m_objectIdx = objectIdx;

    initialization();
}

blPfOptimizerInterface::~blPfOptimizerInterface(){

    //cout << "~blPfOptimizerInterface number of particles to delete = " << m_particles.size() << endl;
    for (unsigned int i = 0 ; i < m_particles.size() ; ++i){
        //cout << "delete particle " << i << endl;

        //for (int j = 0 ; j < m_particles[i]->size(); ++j){
        //    cout << "\t state " << i << ", " << j << "= " << m_particles[i]->getStateAt(j)->getStateAt(0) << ", " << m_particles[i]->getStateAt(j)->getStateAt(1) << ", " << m_particles[i]->getStateAt(j)->getStateAt(2) << endl;
        //}

        if (m_particles[i] != NULL){
            delete m_particles[i];
        }
        //cout << "delete particle " << i << "done " << endl;
        //cout << endl;
    }
}

void blPfOptimizerInterface::initialization(){

    // set resampling function pointer
    if (m_resamplingMethod == "ResampleMultinomial"){
        ResamplingPtr = &blPfOptimizerInterface::resampleMultinomial;
    }
    else if (m_resamplingMethod == "ResampleResidual"){
        ResamplingPtr = &blPfOptimizerInterface::resampleResidual;
    }
    else if (m_resamplingMethod == "ResampleStratified"){
        ResamplingPtr = &blPfOptimizerInterface::resampleStratified;
    }
    else if (m_resamplingMethod == "ResampleSystematic"){
        ResamplingPtr = &blPfOptimizerInterface::resampleSystematic;
    }

    // Get the initial state
    m_track = new blPfTrack();
    m_SizeParticleFrameInMemory = m_particleModel->getOrderMarkovModel();
    m_particles = m_particleModel->getInitialStates(m_Ns, m_startingFrameIdx, m_objectIdx);
    cout << "m_particles size = " << m_particles.size() << endl;

    //cout << "particles size = " << m_particles.size();
    blPfState *stateInter = m_particles[0]->getStateAt(m_startingFrameIdx);
    cout << "stateInter = " << stateInter->getStateAt(0) << ", " << stateInter->getStateAt(1) << ", " << stateInter->getStateAt(2) << endl;
    m_track->addState(stateInter->copy());
    m_curentweights.resize(m_Ns);
    m_useSaveInter = false;
    m_iter = 0;
}

void blPfOptimizerInterface::setResamplingMethod(string resamplingMethod){
    m_resamplingMethod = resamplingMethod;
}

void blPfOptimizerInterface::setParticleNumber(int particleNumber){
    m_Ns = particleNumber;
}

void blPfOptimizerInterface::setResamplingThreshold(int resamplingThreshold){
    m_Nth = resamplingThreshold;
}

blPfState* blPfOptimizerInterface::oneStepParticleFilter(int curentFrameIdx){
    m_curentFrameIdx = curentFrameIdx;
    return oneStepParticleFilter();
}

blPfState* blPfOptimizerInterface::oneStepParticleFilter(){

    std::cout << "blPfOptimizerInterface::oneStepParticleFilter() begin" << std::endl;
    m_iter++;
    // free some memory
    //int m_SizeParticleFrameInMemory = 2;
    std::cout << "m_SizeParticleFrameInMemory = "<< m_SizeParticleFrameInMemory << std::endl;
    if (m_SizeParticleFrameInMemory > -1){
        if (m_iter > m_SizeParticleFrameInMemory){
            cout << "free memory for frame " << m_curentFrameIdx-m_SizeParticleFrameInMemory << endl;
            for (int i=0 ; i<m_Ns ; ++i){
                //m_particles[i]->deleteStateAt(m_curentFrameIdx-2-m_startingFrameIdx);
                m_particles[i]->deleteStateAt(m_curentFrameIdx-m_SizeParticleFrameInMemory);
            }
        }
    }
    m_particleModel->setCurentObjectNeighborhood(m_track->getStateAt(m_curentFrameIdx-1-m_startingFrameIdx));
    cout << "OneStepParticleFilter, curent frame idx = " << m_curentFrameIdx << endl;
    // Prediction
    cout << "prediction" << endl;
    float totalWeight = 0.0;
    for (int i=0 ; i<m_Ns ; ++i){
        m_particleModel->particlePropagate(m_particles[i], m_track->getStateAt(m_curentFrameIdx-1-m_startingFrameIdx));
        //if (i == 0){
        //    blPfState* state = m_particles[i]->getStateAt(m_curentFrameIdx);
        //    cout << "selected previous state = " << m_track->getStateAt(m_curentFrameIdx-1-m_startingFrameIdx)->getStateAt(0) << endl;
        //    cout << "propagated state = " << state->getStateAt(0) << ", " << state->getStateAt(1) << ", " << state->getStateAt(2) << endl;
        //}
        m_curentweights[i] = m_particleModel->calculateParticuleWeight(m_particles[i], m_curentweights[i], m_track);
        totalWeight += m_curentweights[i];
    }

    // Select a particle
    cout << "Select a particle" << endl;
    if (m_particleModel->isSelectionBeforeResampling()){
        m_selectedParticleState = m_particleModel->selectAndUpdateParticle(m_particles, m_curentweights)->copy();
        //cout << "selected state = " << stateInter->getStateAt(0) << ", " <<stateInter->getStateAt(1)<< ", " <<stateInter->getStateAt(2) << endl;
        //cout << "selected state copy = " << m_selectedParticleState->getStateAt(0) << ", " <<m_selectedParticleState->getStateAt(1)<< ", " <<m_selectedParticleState->getStateAt(2) << endl;

    }

    // Normalize states
    cout << "Normalize states" << endl;
    for (int i=0 ; i<m_Ns ; ++i){
        m_curentweights[i] /= totalWeight;
    }

    // Calculate Neff
    cout << "Calculate Neff" << endl;
    float Neff = 0.0;
    for (int i=0 ; i<m_Ns ; ++i){
        Neff += pow(m_curentweights[i], 2);
    }
    Neff = 1.0/Neff;
    // Resampling
    cout << "Resampling" << endl;
    if (Neff < m_Nth){
        (this->*ResamplingPtr)();
    }

    // Select a particle
    cout << "Select a particle" << endl;
    if (!m_particleModel->isSelectionBeforeResampling()){
        m_selectedParticleState = m_particleModel->selectAndUpdateParticle(m_particles, m_curentweights)->copy();
    }

    m_track->addState(m_selectedParticleState);

    if (m_useSaveInter){
        saveParticlesStatesRepresentation();
    }

    return m_selectedParticleState;
}

void blPfOptimizerInterface::resampleMultinomial(){

    // Construct the CDF
    vector<float> c; c.resize(m_Ns);
    c[0] = m_curentweights[0];
    for (int i=1 ; i<m_Ns ; ++i){
        c[i] = c[i-1] + m_curentweights[i];
    }

    // Resample
    int i=0; int j;
    vector<blPfState* > interStates; interStates.resize(m_Ns);
    float sampl;
    while (i<m_Ns){
        sampl = (float)rand()/((float)RAND_MAX+1.0);  // (0,1]
        j=0;
        while (c[j]<sampl){
            j++;
        }
        interStates[i] = m_particles[j]->getStateAt(m_curentFrameIdx)->copy();
        i++;
    }
    // copy the new states
    for (int i=0 ; i < m_Ns ; ++i){
        m_particles[i]->setStateAt(m_curentFrameIdx, interStates[i]);
    }

}

void blPfOptimizerInterface::resampleResidual(){

    std::cout << "res 1" << std::endl;
    vector<int> Ns; Ns.resize(m_Ns); // "Repetition counts" (plus the random part, later on):
    int R = 0; //The "remainder" or "residual" count
    for (int i=0 ; i < m_Ns ; ++i){
        Ns[i] = int(floor(m_curentweights[i]*m_Ns));
        R += Ns[i];
    }

    std::cout << "res 2" << std::endl;
    // The number of particles which will be drawn stocastically:
    int M_rdn = m_Ns-R;

    // The modified weights:
    std::cout << "res 3" << std::endl;
    vector<float> Ws; Ws.resize(m_Ns);
    float inter;
    for (int i=0 ; i < m_Ns ; ++i){
        inter = m_Ns*m_curentweights[i];
        Ws[i] = (inter - floor(inter))/float(M_rdn);
    }

    std::cout << "res 4" << std::endl;
    vector<blPfState* > interStates; interStates.resize(m_Ns);
    // Draw the deterministic part:
    int i=0;
    for (int j=0 ; j < m_Ns ; ++j){
        for (int k=1 ; k<=Ns[j] ; ++k){
            interStates[i] = m_particles[j]->getStateAt(m_curentFrameIdx)->copy();
            i++;
        }
    }

    std::cout << "res 5" << std::endl;
    // Daw the stocastic (Multinomial) part:
    vector<float> c; c.resize(m_Ns);
    c[0] = Ws[0];
    for (int k=1 ; k<m_Ns ; ++k){
        c[k] = c[k-1] + Ws[k];
    }
    c[m_Ns-1] = 1; // in case;

    std::cout << "res 6" << std::endl;
    int j; float sampl;
    while (i<m_Ns){
        sampl = (float)rand()/((float)RAND_MAX+1.0);  // (0,1]
        j=0;
        while (c[j]<sampl){
            j++;
        }
        interStates[i] = m_particles[j]->getStateAt(m_curentFrameIdx)->copy();
        i=i+1;
    }

    // copy the new states
    std::cout << "copy the states"<< std::endl;
    for (int i=0 ; i < m_Ns ; ++i){
        m_particles[i]->deleteStateAt(m_curentFrameIdx);
        m_particles[i]->setStateAt(m_curentFrameIdx, interStates[i]);
    }
    std::cout << "copy the states done"<< std::endl;
}

void blPfOptimizerInterface::resampleStratified(){

    // Construct the CDF
    vector<float> c; c.resize(m_Ns);
    c[0] = m_curentweights[0];
    for (int i=1 ; i<m_Ns ; ++i){
        c[i] = c[i-1] + m_curentweights[i];
    }

    vector<float> T; T.resize(m_Ns);
    for (int i=1 ; i<=m_Ns ; ++i){
        T[i-1] = ((float)rand()/((float)RAND_MAX+1.0))/m_Ns + (i-1)/m_Ns;
    }
    // T(N+1) = 1;

    int i=0;
    int j=0;
    vector<blPfState* > interStates; interStates.resize(m_Ns);
    while (i<m_Ns){
        if (T[i]<c[j]){
            interStates[i] = m_particles[j]->getStateAt(m_curentFrameIdx)->copy();
            i++;
        }
        else{
            j++;
        }
    }
    // copy the new states
    for (int i=0 ; i < m_Ns ; ++i){
        m_particles[i]->setStateAt(m_curentFrameIdx, interStates[i]);
    }
}

void blPfOptimizerInterface::resampleSystematic(){

    // Construct the CDF
    vector<float> c; c.resize(m_Ns);
    c[0] = m_curentweights[0];
    for (int i=1 ; i<m_Ns ; ++i){
        c[i] = c[i-1] + m_curentweights[i];
    }

    vector<float> T = blMathGeometry::LinSpace(0, 1-1/m_Ns, m_Ns);
    for (int i=0 ; i<m_Ns ; ++i){
        T[i] += ((float)rand()/((float)RAND_MAX))/m_Ns;
    }
    //T(N+1) = 1;

    int i=0;
    int j=0;
    vector<blPfState* > interStates; interStates.resize(m_Ns);
    while (i<m_Ns){
        if (T[i]<c[j]){
            interStates[i] = m_particles[j]->getStateAt(m_curentFrameIdx)->copy();
            i++;
        }
        else{
            j++;
        }
    }
    // copy the new states
    for (int i=0 ; i < m_Ns ; ++i){
        m_particles[i]->setStateAt(m_curentFrameIdx, interStates[i]);
    }
}

/*
bool blPfOptimizerInterface::checkParameters(){
    m_Ns = m_processParameters->getValueOfKey<int>("blPfparticleNumber", 200);
    m_Nth = m_processParameters->getValueOfKey<float>("blPfdegeneracyTh", 10000.0);
    m_resamplingMethod = m_processParameters->getValueOfKey<string>("blPfResampling", "ResampleResidual");
    return true;
}
*/

blPfTrack* blPfOptimizerInterface::getTrack(){
    return m_track;
}

void blPfOptimizerInterface::useSaveParticlesStatesRepresentation(string outputFolderPath){
    m_interOutputFolderPath = outputFolderPath;
    m_useSaveInter = true;
}

void blPfOptimizerInterface::saveParticlesStatesRepresentation(){

    // Get the frame
    Float2DImage::Pointer image;

    if ( m_particleModel->getDataManager()->is3DImage() ){
        blZProjection projection;
        projection.setInput( m_particleModel->getDataManager()->getCurentFrame() );
        projection.setProjectionType(blZProjection::Maximum);
        projection.run();
        image = projection.output()->itkFloat2DImagePointer();
        //image = blIntegrateImage::Integrate3DToMip(m_particleModel->GetDataManager()->Get3DCurentFrame());
    }
    else {
        image = m_particleModel->getDataManager()->getCurentFrame()->itkFloat2DImagePointer();
    }

    // Copy into rgb
    FloatColor2DImage::Pointer resImage = blImageCreate::createNewFloatColor2DImage(m_particleModel->getDataManager()->getFrameNl(),m_particleModel->getDataManager()->getFrameNc());
    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(image, image->GetLargestPossibleRegion());

    itk::RGBPixel<float> pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    // plot the particles
    Float2DImage::IndexType pixelIndex;

    pixel[0] = 255 ; pixel[1] = 0 ; pixel[2] = 0 ;
    for (unsigned int i=0 ; i < m_particles.size() ; ++i){
        pixelIndex[0] = m_particles[i]->getStateAt(m_curentFrameIdx)->getStateAt(1);
        pixelIndex[1] = m_particles[i]->getStateAt(m_curentFrameIdx)->getStateAt(2);
        resImage->SetPixel(pixelIndex, pixel);
    }

    pixel[0] = 0 ; pixel[1] = 0 ; pixel[2] = 255;
    pixelIndex[0] = m_selectedParticleState->getStateAt(1);
    pixelIndex[1] = m_selectedParticleState->getStateAt(2);
    resImage->SetPixel(pixelIndex, pixel);
    // save the frame
    blImageIO::saveFloatColor2DImage(m_interOutputFolderPath + m_particleModel->getDataManager()->getFrameNameAt(m_curentFrameIdx) + ".tif", resImage);
}


void blPfOptimizerInterface::removeParticleFirstState(){
    for (unsigned int i = 0 ; i < m_particles.size() ; ++i){
        m_particles[i]->removeInitialState();
    }
}
