#include "blPfOptimizerMultipleObject.h"
#include "blPfOptimizerInterface.h"
#include "blCastStacks/blProjectionImage.h"
#include <blGeometry>

using namespace std;

// ////////////////////////////////////////// //
//            blPfOptimizerMultipleObject
// ////////////////////////////////////////// //
blPfOptimizerMultipleObject::blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int startingFrameIdx){
    m_particleModel = particleModel;
    m_startingFrameIdx = startingFrameIdx;
}


blPfOptimizerMultipleObject::blPfOptimizerMultipleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, string resamplingMethod, int startingFrameIdx){
    m_particleModel = particleModel;
    m_particleNumber = particleNumber;
    m_resamplingThreshold = resamplingThreshold;
    m_resamplingMethod = resamplingMethod;
    m_startingFrameIdx = startingFrameIdx;
}

void blPfOptimizerMultipleObject::setResamplingMethod(string resamplingMethod){
    m_resamplingMethod = resamplingMethod;
}

void blPfOptimizerMultipleObject::setParticleNumber(int particleNumber){
    m_particleNumber = particleNumber;
}

void blPfOptimizerMultipleObject::setResamplingThreshold(int resamplingThreshold){
    m_resamplingThreshold = resamplingThreshold;
}


void blPfOptimizerMultipleObject::run(){

    // Initialize containers
    std::cout << "initialize containers" << std::endl;
    int frameNumber = m_particleModel->getFrameNumber();
    int objectsNumber = m_particleModel->getDataManager()->getObjectNumberAtFrame(m_startingFrameIdx);
    std::cout << "objectsNumber = " << objectsNumber << std::endl;
    m_tracks.resize(objectsNumber); // to store the results
    vector<blPfOptimizerInterface*> blPfinterface; blPfinterface.resize(objectsNumber);
    for (int obj = 0 ; obj < objectsNumber ; ++obj){
        m_tracks[obj] = new blPfTrack();
        //m_tracks[obj]->addState(m_particleModel->GetDataManager()->GetObjectsAt(0,obj));
        std::cout << "create interface" << std::endl;
        blPfinterface[obj] = new blPfOptimizerInterface(m_particleModel, m_particleNumber, m_resamplingThreshold, m_resamplingMethod, m_startingFrameIdx, obj);
        std::cout << "add first state" << std::endl;
        m_tracks[obj]->addState(blPfinterface[obj]->getTrack()->getStateAt(0));

        // to save only one inter result bu  hand
        //vector<float> objectState = m_particleModel->GetDataManager()->GetObjectsAt(m_startingFrameIdx, obj);
        //if (objectState[0] == 654 && objectState[1] == 171){
        //    blPfinterface[obj]->useSaveParticlesStatesRepresentation("F:\\experiments\\film_04_05_z1\\RGSeq\\cpp_particle_filtering\\test_ground_truth\\run\\particles_inter\\");
        //}
    }
    if (objectsNumber == 1){
        //blPfinterface[0]->useSaveParticlesStatesRepresentation("F:\\experiments\\film_04_05_z1\\RGSeq\\cpp_particle_filtering\\test_ground_truth\\run\\particles_inter\\");

    }

    // main loop
    cout << "blPfOptimizerMultipleObject -> main loop" << endl;
    for (int t=1 ; t<frameNumber ; ++t){
        this->notify("process frame " + blCast::to_string(t));
        m_particleModel->setCurentFrameIdx(t); // to load the data
        // proced one step
        for (int obj = 0 ; obj < objectsNumber ; ++obj){
            m_particleModel->setCurentObjectNeighborhood(m_tracks[obj]->getLastState());
            blPfState* selectedState = blPfinterface[obj]->oneStepParticleFilter(t);
            m_tracks[obj]->addState(selectedState);
        }
    }
    // plot tracks
    cout << "blPfOptimizerMultipleObject -> end" << endl;
}

/*
bool blPfOptimizerMultipleObject::checkParameters(){
    m_particleNumber = m_processParameters->getValueOfKey<int>("blPfparticleNumber", 200);
    m_resamplingThreshold = m_processParameters->getValueOfKey<float>("blPfdegeneracyTh", 10000.0);
    m_resamplingMethod = m_processParameters->getValueOfKey<string>("blPfResampling", "ResampleResidual");
    return true;
}
*/

vector<blPfTrack*> blPfOptimizerMultipleObject::getTracks(){
    return m_tracks;
}

void blPfOptimizerMultipleObject::setTracks(vector<blPfTrack*> tracks){
    m_tracks = tracks;
}

void blPfOptimizerMultipleObject::saveTrackInTxt(string fileName){

    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        for (unsigned int obj = 0 ; obj < m_tracks.size() ; ++obj){ // for each track

            file << "track " << obj << endl;

            for (int t = 0 ; t < m_tracks[obj]->getTrackSize(); ++t){
                // Get the track states
                blPfState *initialState = m_tracks[obj]->getStateAt(t);
                vector<float> stateStart = initialState->getState();
                // add the state
                for (unsigned int s = 0 ; s < stateStart.size() ; ++s){
                    file << stateStart.at(s) << "\t";
                }
                file << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}

void blPfOptimizerMultipleObject::saveTrackRepresentation(string imagePath){

    /*
    cout << "tracks size: " << m_tracks.size() << endl;
    for (int obj = 0 ; obj < m_tracks.size() ; ++obj){
        cout << m_tracks[obj]->getTrackSize() << endl;
        for (int i = 0 ; i < m_tracks[obj]->getTrackSize() ; ++i){
            cout << "\t" << m_tracks[obj]->getStateAt(i)->getStateAt(1) << ", " << m_tracks[obj]->getStateAt(i)->getStateAt(2) << endl;
        }
    }
    */

    // get the image 2D or 3D
    bool is3Dimage = m_particleModel->getDataManager()->is3DImage();
    int framesNum = m_particleModel->getDataManager()->getTotalFrameNumber();
    cout << "blPfOptimizerMultipleObject::SaveTrackRepresentation, use 3D image" <<  is3Dimage << endl;
    cout << "framesNum " <<  framesNum << endl;

    if (is3Dimage){
        cout << "blPfSingleObject::SaveTrackRepresentation -> 3D case" << endl;
        // Create a 3d image to save the representation
        int nl = m_particleModel->getDataManager()->getFrameNl();
        int nc = m_particleModel->getDataManager()->getFrameNc();
        cout << "nl = " << nl << "nc = " << nc << endl;
        FloatColor3DImage::Pointer outputImage = blImageCreate::createNewFloatColor3DImage(nl, nc, framesNum);

        // copy the frames
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;
        for (int i = 0 ; i < framesNum ; ++i){

            index3d[2] = i;
            blZProjection projection;
            projection.setInput( m_particleModel->getDataManager()->getFrameAt(i));
            projection.setProjectionType(blZProjection::Maximum);
            projection.run();
            Float2DImage::Pointer curentFramePointer = projection.output()->itkFloat2DImagePointer();

            //Float2DImage::Pointer curentFramePointer = blIntegrateImage::Integrate3DToMip( m_particleModel->GetDataManager()->Get3DFrameAt(i) );
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = curentFramePointer->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        // plot
        for (unsigned int obj = 0 ; obj < m_tracks.size() ; ++obj){ // for each track

            // get random color
            vector<int> rgb = blColor::GetRandRGB();
            pixel3d[0] = rgb[0]; pixel3d[1] = rgb[1]; pixel3d[2] = rgb[2];

            cout << "plot track " << obj << endl;

            int trackBegin = m_tracks[obj]->getStateAt(0)->getStateAt(0);
            int trackEnd = m_tracks[obj]->getStateAt(m_tracks[obj]->getTrackSize()-1)->getStateAt(0);

            for (int i = trackBegin ; i <= trackEnd ; ++i){
                index3d[2] = i;
                // plot the track till i

                for (int t = trackBegin ; t <= i ; ++t){

                    if (t == trackBegin){
                        int x1 = m_tracks[obj]->getStateAt(t-trackBegin)->getStateAt(1);
                        int y1 = m_tracks[obj]->getStateAt(t-trackBegin)->getStateAt(2);
                        index3d[0] = x1;
                        index3d[1] = y1;
                        outputImage->SetPixel(index3d, pixel3d);
                    }
                    else{

                        // Cacluate the line points
                        vector<int> px, py;
                        int x1 = m_tracks[obj]->getStateAt(t-1-trackBegin)->getStateAt(1);
                        int y1 = m_tracks[obj]->getStateAt(t-1-trackBegin)->getStateAt(2);
                        int x2 = m_tracks[obj]->getStateAt(t-trackBegin)->getStateAt(1);
                        int y2 = m_tracks[obj]->getStateAt(t-trackBegin)->getStateAt(2);
                        blMathGeometry::Calculate2DLineCoordinates(x1,y1, x2, y2, px, py);

                        // plot the points
                        for (unsigned int j = 0 ; j < px.size() ; ++j){
                            index3d[0] = px[j];
                            index3d[1] = py[j];
                            if (!(px[j] < 0 || px[j] >=nl || py[j] < 0 || py[j] >=nc))
                                outputImage->SetPixel(index3d, pixel3d);
                        }
                    }
                }
            }
        }


        // save the 3d image
        blImageIO::saveFloatColor3DImage(imagePath, outputImage, true);
    }
    else{
        cout << "blPfSingleObject::SaveTrackRepresentation -> 2D case" << endl;
        // Create a 3d image to save the representation
        int nl = m_particleModel->getDataManager()->getFrameNl();
        int nc = m_particleModel->getDataManager()->getFrameNc();
        FloatColor3DImage::Pointer outputImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_tracks[0]->getTrackSize());

        // copy the frames
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;
        for (int i = 0 ; i < m_tracks[0]->getTrackSize() ; ++i){
            // copy the frame
            index3d[2] = i;
            Float2DImage::Pointer curentFramePointer = m_particleModel->getDataManager()->getFrameAt(i)->itkFloat2DImagePointer();
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = curentFramePointer->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        for (unsigned int obj = 0 ; obj < m_tracks.size() ; ++obj){
            vector<int> rgb = blColor::GetRandRGB();
            //cout << "plot track " << obj << endl;
            for (int i = 0 ; i < m_tracks[obj]->getTrackSize() ; ++i){
                index3d[2] = i;
                // plot the track till i
                pixel3d[0] = rgb[0]; pixel3d[1] = rgb[1]; pixel3d[2] = rgb[2];
                for (int t = 1 ; t <= i ; ++t){
                    // Cacluate the line points
                    vector<int> px, py;
                    int x1 = m_tracks[obj]->getStateAt(t-1)->getStateAt(1);
                    int y1 = m_tracks[obj]->getStateAt(t-1)->getStateAt(2);
                    int x2 = m_tracks[obj]->getStateAt(t)->getStateAt(1);
                    int y2 = m_tracks[obj]->getStateAt(t)->getStateAt(2);
                    blMathGeometry::Calculate2DLineCoordinates(x1,y1, x2, y2, px, py);

                    // plot the points
                    for (unsigned int j = 0 ; j < px.size() ; ++j){
                        index3d[0] = px[j];
                        index3d[1] = py[j];
                        if (!(px[j] < 0 || px[j] >=nl || py[j] < 0 || py[j] >=nc))
                            outputImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
        }
        // save the 3d image
        blImageIO::saveFloatColor3DImage(imagePath, outputImage, true);
    }
}
