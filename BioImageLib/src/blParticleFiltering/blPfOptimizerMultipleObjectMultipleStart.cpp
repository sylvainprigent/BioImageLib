#include "blPfOptimizerMultipleObjectMultipleStart.h"
#include "blPfOptimizerInterface.h"

#include "itkRescaleIntensityImageFilter.h"
#include "blCastStacks/blProjectionImage.h"
#include <blGeometry>

using namespace std;

// ////////////////////////////////////////// //
//            blPfMultipleObject
// ////////////////////////////////////////// //
blPfOptimizerMultipleObjectMultipleStart::blPfOptimizerMultipleObjectMultipleStart(blPfModelInterface *particleModel, int startingFrameIdx){
    m_particleModel = particleModel;
    m_startingFrameIdx = startingFrameIdx;
}


blPfOptimizerMultipleObjectMultipleStart::blPfOptimizerMultipleObjectMultipleStart(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, string resamplingMethod, int startingFrameIdx){
    m_particleModel = particleModel;
    m_particleNumber = particleNumber;
    m_resamplingThreshold = resamplingThreshold;
    m_resamplingMethod = resamplingMethod;
    m_startingFrameIdx = startingFrameIdx;
    m_alreadyUsedNeighborThreshold = 4;
}

void blPfOptimizerMultipleObjectMultipleStart::setResamplingMethod(string resamplingMethod){
    m_resamplingMethod = resamplingMethod;
}

void blPfOptimizerMultipleObjectMultipleStart::setParticleNumber(int particleNumber){
    m_particleNumber = particleNumber;
}

void blPfOptimizerMultipleObjectMultipleStart::setResamplingThreshold(int resamplingThreshold){
    m_resamplingThreshold = resamplingThreshold;
}

void blPfOptimizerMultipleObjectMultipleStart::setalreadyUsedNeighborThreshold(float alreadyUsedNeighborThreshold){
    m_alreadyUsedNeighborThreshold = alreadyUsedNeighborThreshold;
}

void blPfOptimizerMultipleObjectMultipleStart::run(){

    for (int t = 0 ; t < m_particleModel->getFrameNumber()-1 ; ++t){

        int objectsNumber = m_particleModel->getDataManager()->getObjectNumberAtFrame(t);
        if (objectsNumber > 0){
            // Calculate tracks on frame t
            vector<blPfTrack*> tracksInter = runMultiplePFonFrame(t);

            // save the tracks
            //cout << "save the tracks starting from " << t << endl;
            for (unsigned int i  = 0 ; i < tracksInter.size() ; ++i){
                m_tracks.push_back(tracksInter[i]);
            }

            // remove the used objects
            //cout << "remove the used objects from " << t << endl;
            for (int p = 0 ; p < int(tracksInter.size()) ; ++p ){
                blPfTrack* curentTrack = tracksInter[p];
                int trackSize = curentTrack->getTrackSize();
                for (int e = 0 ; e < trackSize ; ++e){
                    blPfState* state = curentTrack->getStateAt(e);
                    vector<int> objectsIdx = findObject(state);
                    //cout << "number of objects to remove = " << objectsIdx.size() << endl;
                    if (!objectsIdx.empty()){
                        for (int obj = 0 ; obj < int(objectsIdx.size()) ; ++obj){
                            //cout << "remove object " << objectsIdx[obj] << ", in frame " << state->getStateAt(0) << endl;
                            m_particleModel->getDataManager()->removeMultipleTimeObject(state->getStateAt(0), objectsIdx[obj]);
                        }
                    }
                }
            }
        }
    }
}

vector<int> blPfOptimizerMultipleObjectMultipleStart::findObject(blPfState* state){

    if (m_particleModel->getDataManager()->is3DImage()){
        float frameid = state->getStateAt(0);
        float x = state->getStateAt(1);
        float y = state->getStateAt(2);
        float z = state->getStateAt(3);

        vector<vector<float> > objects = m_particleModel->getDataManager()->getObjectsAt(frameid);

        float eucl;
        vector<int> neighbors;
        for (unsigned int i = 0 ; i < objects.size() ; ++i){
            if (objects[i][2] == z){
                eucl = pow(x - objects[i][0],2) + pow(y-objects[i][1],2);
                if (eucl < m_alreadyUsedNeighborThreshold){
                    neighbors.push_back(i);
                }
            }
        }
        return neighbors;
    }
    else{
        float frameid = state->getStateAt(0);
        float x = state->getStateAt(1);
        float y = state->getStateAt(2);

        vector<vector<float> > objects = m_particleModel->getDataManager()->getObjectsAt(frameid);

        float eucl;
        vector<int> neighbors;
        for (unsigned int i = 0 ; i < objects.size() ; ++i){
            eucl = pow(x - objects[i][0],2) + pow(y-objects[i][1],2);
            if (eucl < m_alreadyUsedNeighborThreshold){
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }
}

vector<blPfTrack*> blPfOptimizerMultipleObjectMultipleStart::runMultiplePFonFrame(int frameIdx){
    // Initialize containers
    int frameNumber = m_particleModel->getFrameNumber();
    int objectsNumber = m_particleModel->getDataManager()->getObjectNumberAtFrame(frameIdx);
    vector<blPfTrack*> tracksInter; tracksInter.resize(objectsNumber); // to store the results
    vector<blPfOptimizerInterface*> blPfinterface; blPfinterface.resize(objectsNumber);
    //cout << "add blPfinterface for each objectsNumber" << endl;
    for (int obj = 0 ; obj < objectsNumber ; ++obj){
        //tracksInter[obj] = new blPfTrack();
        //cout << "add blPfinterface for objectsNumber " << obj << endl;
        blPfinterface[obj] = new blPfOptimizerInterface(m_particleModel, m_particleNumber, m_resamplingThreshold, m_resamplingMethod, frameIdx, obj);
        //cout << "add state for objectsNumber " << obj << endl;
        //tracksInter[obj]->addState(blPfinterface[obj]->GetTrack()->getStateAt(0));
    }

    // main loop
    cout << "blPfOptimizerMultipleObjectMultipleStart -> main loop on frame: " << frameIdx << ", detection number = " << objectsNumber << endl;
    for (int t=frameIdx+1 ; t<frameNumber ; ++t){
        this->notify("process frame " + blCast::to_string(t));

        m_particleModel->setCurentFrameIdx(t); // to load the data
        // proced one step
        //cout << "proced one step. Tracks to process: " << objectsNumber << endl;
        for (int obj = 0 ; obj < objectsNumber ; ++obj){
            //cout << "start tracking object " << obj << endl;
            //m_particleModel->SetCurentObjectNeighborhood(tracksInter[obj]->getLastState());
            //cout << "OneStepParticleFilter " << t << endl;
            blPfinterface[obj]->oneStepParticleFilter(t);
            //blPfState* selectedState = blPfinterface[obj]->OneStepParticleFilter(t);
            //cout << "addState " << obj << endl;
            //tracksInter[obj]->addState(selectedState->copy());
            //delete selectedState;
            //cout << "done " << obj << endl;
        }
    }

    for (int i  = 0 ; i < objectsNumber ; ++i){
        tracksInter[i] = blPfinterface[i]->getTrack();
        //cout << "delete interface " << i << endl;
        delete blPfinterface[i];
        //cout << "delete interface " << i << "done" << endl;
    }
    // plot tracks
    //cout << "blPfOptimizerMultipleObjectMultipleStart -> endframe: " << frameIdx << endl;
    return tracksInter;
}

/*
bool blPfOptimizerMultipleObjectMultipleStart::checkParameters(){
    m_particleNumber = m_processParameters->getValueOfKey<int>("blPfparticleNumber", 200);
    m_resamplingThreshold = m_processParameters->getValueOfKey<float>("blPfdegeneracyTh", 10000.0);
    m_resamplingMethod = m_processParameters->getValueOfKey<string>("blPfResampling", "ResampleResidual");
    m_alreadyUsedNeighborThreshold = m_processParameters->getValueOfKey<float>("blPfalreadyUsedNeighborThreshold", 2);
    m_alreadyUsedNeighborThreshold = m_alreadyUsedNeighborThreshold*m_alreadyUsedNeighborThreshold;
    return true;
}
*/

vector<blPfTrack*> blPfOptimizerMultipleObjectMultipleStart::getTracks(){
    return m_tracks;
}

void blPfOptimizerMultipleObjectMultipleStart::saveTrackRepresentation(string imagePath){

    cout << "tracks size: " << m_tracks.size() << endl;
    for (unsigned int obj = 0 ; obj < m_tracks.size() ; ++obj){
        cout << m_tracks[obj]->getTrackSize() << endl;
        for (int i = 0 ; i < m_tracks[obj]->getTrackSize() ; ++i){
            cout << "\t" << m_tracks[obj]->getStateAt(i)->getStateAt(1) << ", " << m_tracks[obj]->getStateAt(i)->getStateAt(2) << ", " << m_tracks[obj]->getStateAt(i)->getStateAt(0) << endl;
        }
    }

    // get the image 2D or 3D
    bool is3Dimage = m_particleModel->getDataManager()->is3DImage();
    int frameNum = m_particleModel->getDataManager()->getTotalFrameNumber();
    cout << "blPfMultipleObject::SaveTrackRepresentation, use 3D image " <<  is3Dimage << endl;

    if (is3Dimage){
        cout << "blPfOptimizerMultipleObjectMultipleStart::SaveTrackRepresentation -> 3D case" << endl;
        // Create a 3d image to save the representation
        int nl = m_particleModel->getDataManager()->getFrameNl();
        int nc = m_particleModel->getDataManager()->getFrameNc();
        FloatColor3DImage::Pointer outputImage = blImageCreate::createNewFloatColor3DImage(nl, nc, frameNum);

        // copy the frames
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;
        for (int i = 0 ; i < m_particleModel->getDataManager()->getTotalFrameNumber() ; ++i){

            index3d[2] = i;
            blZProjection projection;
            projection.setInput( m_particleModel->getDataManager()->getFrameAt(i) );
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

        // plot
        for (unsigned int obj = 0 ; obj < m_tracks.size() ; ++obj){ // for each track

            // get random color
            vector<int> rgb = blColor::GetRandRGB();
            pixel3d[0] = rgb[0]; pixel3d[1] = rgb[1]; pixel3d[2] = rgb[2];

            //cout << "plot track " << obj << endl;

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
}

FloatColor2DImage::Pointer blPfOptimizerMultipleObjectMultipleStart::getTracksRepresentationOnLastImage(){

    int frameNumber = m_particleModel->getFrameNumber();

    Float2DImage::Pointer imageN_1;

    FloatColor2DImage::Pointer resImage;
    bool is3Dimage = m_particleModel->getDataManager()->is3DImage();
    if (is3Dimage){
        cout << "blPfOptimizerMultipleObjectMultipleStart::GetTracksRepresentationOnLastImage -> 3D case" << endl;

        blZProjection projection;
        projection.setInput(m_particleModel->getDataManager()->getFrameAt(frameNumber-1));
        projection.setProjectionType(blZProjection::Maximum);
        projection.run();
        imageN_1 = projection.output()->itkFloat2DImagePointer();
        //imageN_1 = blIntegrateImage::Integrate3DToMip(curentFrame);
    }
    else{
        cout << "blPfOptimizerMultipleObjectMultipleStart::GetTracksRepresentationOnLastImage -> 2D case" << endl;
        imageN_1 = m_particleModel->getDataManager()->getFrameAt(frameNumber-1)->itkFloat2DImagePointer();
    }

    typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(imageN_1);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    imageN_1 = rescaleFilter->GetOutput();

    int nl = imageN_1->GetLargestPossibleRegion().GetSize()[0];
    int nc = imageN_1->GetLargestPossibleRegion().GetSize()[1];
    resImage = blImageCreate::createNewFloatColor2DImage(nl, nc);

    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageN_1, imageN_1->GetLargestPossibleRegion());

    //itk::Vector<float> pixel(3);
    itk::RGBPixel<float> pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    FloatColor2DImage::IndexType pixelIndex;

    // Plot the tracks
    //cout << "blPfOptimizerMultipleObjectMultipleStart::GetTracksRepresentationOnLastImage: Plot the tracks" << endl;
    for (unsigned int track = 0 ; track<m_tracks.size() ; ++track){

        // get a color
        vector<int> randColor = blColor::GetRandRGB();
        pixel[0] = randColor[0]; pixel[1] = randColor[1]; pixel[2] = randColor[2];

        //cout << "track " << track;
        // plot the link on each frames
        for (int t = 1 ; t < m_tracks[track]->getTrackSize() ; ++t){
            //cout << "get state at " << t << endl;
            int xStart = m_tracks[track]->getStateAt(t-1)->getStateAt(1);
            int yStart = m_tracks[track]->getStateAt(t-1)->getStateAt(2);
            int xEnd = m_tracks[track]->getStateAt(t)->getStateAt(1);
            int yEnd = m_tracks[track]->getStateAt(t)->getStateAt(2);
            //cout << "state = " << xStart << ", " << yStart << ", "  << xEnd << ", "  << yEnd << endl;

            vector<int> px, py;
            blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
            for (unsigned int i=0 ; i<px.size() ; ++i){
                pixelIndex[0]=px[i]; pixelIndex[1]=py[i];
                resImage->SetPixel(pixelIndex, pixel);
            }
            //cout << "end state at " << t << endl;
        }
    }

    return resImage;
}

void blPfOptimizerMultipleObjectMultipleStart::saveTracksToTxtFile(string fileName){

    // 1- Open file
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (int i = 0 ; i < int(m_tracks.size()) ; ++i){
            file << "track " << i << endl;
            for (int st = 0 ; st < m_tracks[i]->getTrackSize() ; ++st ){
                blPfState* stateInter = m_tracks.at(i)->getStateAt(st);

                for (int s = 1 ; s < int(stateInter->size()) ; ++s){
                    file << stateInter->getStateAt(s) << "\t";
                }
                file << stateInter->getStateAt(0) << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}
