/// \file blAtTrackerInterface.h
/// \brief blAtTrackerInterface class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#include "blAtTrackerInterface.h"
#include <blCore>

#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "blCastStacks/blProjectionImage.h"
#include "blGeometry/blMathGeometry.h"

using namespace std;

////////////////////////////////////////////////////////// //
//                   blAtTrackerInterface
// ////////////////////////////////////////////////////// //
blAtTrackerInterface::blAtTrackerInterface(blAtCostInterface *costFunction, vector<string> framesFiles, vector<vector<vector<float> > > objects) : blProcess(){
    setInputs(costFunction, framesFiles, objects);
    initParametersWithDefaultsValues();
}

blAtTrackerInterface::blAtTrackerInterface(){
    initParametersWithDefaultsValues();
}

blAtTrackerInterface::~blAtTrackerInterface(){

    if (!m_returnedOutput){
        for (int i = m_tracks.size()-1 ; i >=0  ; i--){
            delete m_tracks[i];
        }
    }
}

void blAtTrackerInterface::setInputs(blAtCostInterface *costFunction, vector<string> framesFiles, vector<vector<vector<float> > > objects){
    // copy inputs
    m_framesFiles = framesFiles;
    if (m_framesFiles.size() < 2){
        throw blException("They should be more than 2 input images to do tracking");
    }
    m_frameNumber = m_framesFiles.size();
    m_objects = objects;
    if (!objects.empty()){
        m_objectInfoSize = objects[0][0].size();
    }
    else{
        m_objectInfoSize = 0;
    }

    // initialize the cost funtion
    m_costFunction = costFunction;
    m_costFunction->setFramesPath(m_framesFiles);
    m_costFunction->setObjects(m_objects);
    m_useSorter = false;

    // get the image dimension
    blImageIOInfo info;
    info.updateInfo(m_framesFiles[0]);
    int dim = info.dimension();
    if (dim == 3 ) {m_is3D = true;}
    else {m_is3D = false;}

    m_tracksAlreadyCalculated = false;
}

// ////////////// parameters //////////////////////////////
void blAtTrackerInterface::initParametersWithDefaultsValues(){
    m_timeSpread = 1;
    m_returnedOutput = false;
}

// ///////////// setters //////////////////////////////////
void blAtTrackerInterface::setTimeSpread(int timeSpread){
    m_timeSpread = timeSpread;
}

void blAtTrackerInterface::setL(float L){
    m_L = L;
}

// ///////////////////////////////////////////////////////


// inputs
void blAtTrackerInterface::setTrackSorter(blAtTrackSorterInterface *sorter){
    m_useSorter = true;
    m_trackSorter = sorter;
}

// outputs
vector<blAtTrack *> blAtTrackerInterface::output(){
    m_returnedOutput = true;
    return m_tracks;
}

int blAtTrackerInterface::isConnectionStartingPointInTrack(vector<blAtTrack*> tracks, blAtConnection* connection){
    for (unsigned int i = 0 ; i < tracks.size() ; ++i){
        blAtConnection* trackLastConnection = tracks.at(i)->getLastConnection();
        if ( trackLastConnection->stateEnd() == connection->stateStart() && trackLastConnection->endFrameIdx() == connection->startFrameIdx() ){
            return i;
        }
    }
    return -1;
}

void blAtTrackerInterface::calculateTracks(){

    if (!m_tracksAlreadyCalculated){
        m_tracks.clear();

        // Get all the connections starting at initial frame
        for (unsigned int i = 0 ; i < m_connections.size() ; ++i){
            if (m_connections.at(i)->startFrameIdx()==0){
                m_tracks.push_back(new blAtTrack(m_connections.at(i)));
            }
        }

        // browse all frames to build connections
        int foundPos;
        for(unsigned int frame=1 ; frame<m_frameNumber ; ++frame){
            // search all the connections starting at frame
            vector<blAtConnection*> interConnections;
            for (unsigned int i = 0 ; i < m_connections.size() ; ++i){
                if (m_connections.at(i)->startFrameIdx()==int(frame)){
                    interConnections.push_back(m_connections.at(i));
                }
            }

            for (unsigned int con = 0 ; con < interConnections.size() ; ++con){
                foundPos = isConnectionStartingPointInTrack(m_tracks, interConnections.at(con));
                if ( foundPos > -1 ){
                    // add the link to an existing track
                    m_tracks.at(foundPos)->addConnection(interConnections.at(con));
                }
                else{
                    // create a new track
                    m_tracks.push_back(new blAtTrack(interConnections.at(con)));
                }
            }
            // find all the objects starting at frame-1
        }
    }

    cout << "m_useSorter = " << m_useSorter << endl;
    if (m_useSorter){
        m_trackSorter->setMovieImagesPath(m_framesFiles);
        m_trackSorter->setTracks(m_tracks);
        m_trackSorter->setTacksCosts(m_tracksCosts);
        m_trackSorter->run();
        m_tracks = m_trackSorter->getoutput();
    }
}

void blAtTrackerInterface::setUseSorter(bool useSorter){
    m_useSorter = useSorter;
}

vector<blAtConnection *> blAtTrackerInterface::getConnections(){
    //int maxi = m_connections.at(0)->endFrameIdx();
    //for (int i = 1 ; i < m_connections.size() ; ++i){
    //    if (m_connections.at(i)->endFrameIdx() > maxi){
    //        maxi = m_connections.at(i)->endFrameIdx();
    //    }
    //}
    //cout << "max frame = " << maxi << endl;
    return m_connections;
}

void blAtTrackerInterface::saveTimeTracksRepresentation(string rootFileName){

    if (m_is3D){
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;

        // Create the output image and copy the inputs
        Float3DImage::Pointer image3D = blImageIO::loadToFloat3DImage(m_framesFiles.at(0));
        blZProjection projection;
        projection.setInput(new blImage(image3D));
        projection.setProjectionType(blZProjection::Maximum);
        Float2DImage::Pointer image = projection.output()->itkFloat2DImagePointer();
        //Float2DImage::Pointer image = blIntegrateImage::Integrate3DToMip(image3D);

        int nl = image->GetLargestPossibleRegion().GetSize()[0];
        int nc = image->GetLargestPossibleRegion().GetSize()[1];
        FloatColor3DImage::Pointer resImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_framesFiles.size());

        for (unsigned int i = 0 ; i < m_framesFiles.size() ; ++i){

            if (i>0){
                Float3DImage::Pointer image3D = blImageIO::loadToFloat3DImage(m_framesFiles.at(i));
                blZProjection projection;
                projection.setInput(new blImage(image3D));
                projection.setProjectionType(blZProjection::Maximum);
                image = projection.output()->itkFloat2DImagePointer();
                //image = blIntegrateImage::Integrate3DToMip(image3D);
            }

            typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
            RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
            rescaleFilter->SetInput(image);
            rescaleFilter->SetOutputMinimum(0);
            rescaleFilter->SetOutputMaximum(255);
            image = rescaleFilter->GetOutput();

            index3d[2] = i;
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = image->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        // Plot the connexions on top of each projected frames
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

            // get a color
            vector<int> randColor = blColor::GetRandRGB();
            pixel3d[0] = randColor[0]; pixel3d[1] = randColor[1]; pixel3d[2] = randColor[2];

            // plot the link on each frames
            /*
            for (int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);

                vector<int> px, py;
                BasicGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                int frameIdx = connectionInter->endFrameIdx();
                //if (frameIdx> 12)
                //    cout << "plot m_tracks at " << l << "frame idx = " << frameIdx << endl;
                for (int rep = frameIdx ; rep < m_framesNames.size()-1 ; ++rep){
                    for (int i=0 ; i<px.size() ; ++i){
                        index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = rep;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
            */
            for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);
                int frameIdx = connectionInter->endFrameIdx();
                index3d[0]=xEnd; index3d[1]=yEnd; index3d[2] = frameIdx;
                resImage->SetPixel(index3d, pixel3d);

                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                if (conn == 0){
                    int frameIdx = connectionInter->startFrameIdx();
                    index3d[0]=xStart; index3d[1]=yStart; index3d[2] = frameIdx;
                    resImage->SetPixel(index3d, pixel3d);
                }

                // plot the line between the two points
                vector<int> px, py;
                blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                for (unsigned int i=0 ; i<px.size() ; ++i){
                    index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = frameIdx;
                    resImage->SetPixel(index3d, pixel3d);
                }

            }
        }

        blImageIO::saveFloatColor3DImage(rootFileName, resImage, true);
    }
    else{
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;

        // Create the output image and copy the inputs

        Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesFiles.at(0));
        // normalize intensity
        typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        rescaleFilter->SetInput(image);
        rescaleFilter->SetOutputMinimum(0);
        rescaleFilter->SetOutputMaximum(255);
        rescaleFilter->Update();
        image = rescaleFilter->GetOutput();

        int nl = image->GetLargestPossibleRegion().GetSize()[0];
        int nc = image->GetLargestPossibleRegion().GetSize()[1];
        FloatColor3DImage::Pointer resImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_framesFiles.size());

        // Copy input frames in a 3D image
        for (unsigned int i = 0 ; i < m_framesFiles.size() ; ++i){

            if (i>0){
                image = blImageIO::loadToFloat2DImage(m_framesFiles.at(i));
                // normalize intensity
                typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
                RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
                rescaleFilter->SetInput(image);
                rescaleFilter->SetOutputMinimum(0);
                rescaleFilter->SetOutputMaximum(255);
                rescaleFilter->Update();
                image = rescaleFilter->GetOutput();
            }
            index3d[2] = i;
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = image->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        // Plot the connexions on top of each projected frames
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

            // get a color
            vector<int> randColor = blColor::GetRandRGB();
            pixel3d[0] = randColor[0]; pixel3d[1] = randColor[1]; pixel3d[2] = randColor[2];

            // plot the link on each frames
            /*
            for (int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);
                vector<int> px, py;
                BasicGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                int frameIdx = connectionInter->endFrameIdx();
                for (int rep = frameIdx ; rep < m_framesNames.size()-1 ; ++rep){
                    for (int i=0 ; i<px.size() ; ++i){
                        index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = rep;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
            */
            for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);
                int frameIdx = connectionInter->endFrameIdx();
                index3d[0]=xEnd; index3d[1]=yEnd; index3d[2] = frameIdx;
                resImage->SetPixel(index3d, pixel3d);

                if (conn == 0){
                    int xStart = connectionInter->stateStart().at(0);
                    int yStart = connectionInter->stateStart().at(1);
                    int frameIdx = connectionInter->startFrameIdx();
                    index3d[0]=xStart; index3d[1]=yStart; index3d[2] = frameIdx;
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }
        blImageIO::saveFloatColor3DImage(rootFileName, resImage, true);
    }
}

void blAtTrackerInterface::saveTimeTracksRepresentationOneFrame(string rootFileName){

    if (m_is3D){
        // \todo implement the 3D case
    }
    else{

        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;

        // Create the output image and copy the inputs
        Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesFiles.at(0));
        // normalize intensity
        typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        rescaleFilter->SetInput(image);
        rescaleFilter->SetOutputMinimum(0);
        rescaleFilter->SetOutputMaximum(255);
        rescaleFilter->Update();
        image = rescaleFilter->GetOutput();

        int nl = image->GetLargestPossibleRegion().GetSize()[0];
        int nc = image->GetLargestPossibleRegion().GetSize()[1];
        FloatColor3DImage::Pointer resImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_framesFiles.size());

        // Copy input frames in a 3D image
        //        for (int i = 0 ; i < 1 ; ++i){

        //            if (i>0){
        //                image = blImageIO::LoadToFloat2DImage(m_framesFiles.at(0));
        //                // normalize intensity
        //                typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        //                RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        //                rescaleFilter->SetInput(image);
        //                rescaleFilter->SetOutputMinimum(0);
        //                rescaleFilter->SetOutputMaximum(255);
        //                rescaleFilter->Update();
        //                image = rescaleFilter->GetOutput();
        //            }
        //            index3d[2] = i;
        //            for (int m = 0 ; m < nl ; ++m){
        //                for (int n = 0 ; n < nc ; ++n){
        //                    index3d[0] = m; index3d[1] = n;
        //                    index2d[0] = m; index2d[1] = n;
        //                    float value = image->GetPixel(index2d);
        //                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
        //                    resImage->SetPixel(index3d, pixel3d);
        //                }
        //            }
        //        }

        // Plot the connexions on top of each projected frames
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

            // get a color
            vector<int> randColor = blColor::GetRandRGB();
            pixel3d[0] = randColor[0]; pixel3d[1] = randColor[1]; pixel3d[2] = randColor[2];

            // plot the link on each frames
            /*
            for (int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                AT_Connection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);
                vector<int> px, py;
                BasicGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                int frameIdx = connectionInter->endFrameIdx();
                for (int rep = frameIdx ; rep < m_framesNames.size()-1 ; ++rep){
                    for (int i=0 ; i<px.size() ; ++i){
                        index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = rep;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
            */
            for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);
                int frameIdx = connectionInter->endFrameIdx();
                index3d[0]=xEnd; index3d[1]=yEnd; index3d[2] = frameIdx;
                resImage->SetPixel(index3d, pixel3d);

                if (conn == 0){
                    int xStart = connectionInter->stateStart().at(0);
                    int yStart = connectionInter->stateStart().at(1);
                    int frameIdx = connectionInter->startFrameIdx();
                    index3d[0]=xStart; index3d[1]=yStart; index3d[2] = frameIdx;
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }
        blImageIO::saveFloatColor3DImage(rootFileName, resImage);
    }
}


void blAtTrackerInterface::saveTimeTracksRepresentationIndividual(string rootFileName){

    if (m_is3D){
        // todo implement this
    }
    else{

        Float2DImage::IndexType index2d;
        FloatColor2DImage::PixelType pixel2d;

        // Create the output image and copy the inputs
        blImage* image = new blImage(m_framesFiles.at(0));
        int nl = image->imageSize().xSize();
        int nc = image->imageSize().ySize();
        std::string imageType = image->imageType();
        delete image;


        // get a random color for each track
        vector<vector<int> > randColors;
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){
            vector<int> randColor = blColor::GetRandRGB();
            randColors.push_back(randColor);
        }


        for (int f = 0 ; f < int(m_framesFiles.size()) ; ++f){

            FloatColor2DImage::Pointer resImage = this->getIndividualRepresentationImage(imageType, nl, nc, f);

            // Plot the connexions on top of each projected frames
            for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

                // get a color
                vector<int> randColor = randColors[l];
                pixel2d[0] = randColor[0]; pixel2d[1] = randColor[1]; pixel2d[2] = randColor[2];

                // plot the link on each frames
                for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                    blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                    int xStart = connectionInter->stateStart().at(0);
                    int yStart = connectionInter->stateStart().at(1);
                    int xEnd = connectionInter->stateEnd().at(0);
                    int yEnd = connectionInter->stateEnd().at(1);

                    vector<int> px, py;
                    blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                    int frameIdx = connectionInter->endFrameIdx();
                    if (frameIdx <= f && frameIdx >= f-10){
                        for (unsigned int i=0 ; i<px.size() ; ++i){
                            index2d[0]=px[i]; index2d[1]=py[i];

                            if (index2d[0] >= 0 && index2d[0] < nl && index2d[1] >= 0 && index2d[1] < nc){
                                resImage->SetPixel(index2d, pixel2d);
                            }
                        }
                    }

                    if (conn == 0 && connectionInter->startFrameIdx() == f){
                        index2d[0]=xStart; index2d[1]=yStart;
                        if (index2d[0] >= 0 && index2d[0] < nl && index2d[1] >= 0 && index2d[1] < nc){
                            resImage->SetPixel(index2d, pixel2d);
                        }
                    }
                }
            }
            string fileName = blStringOperations::getFileNameFromPath(m_framesFiles[f]);
            blImageIO::saveFloatColor2DImage(rootFileName + fileName, resImage, true);
        }

    }
}

void blAtTrackerInterface::saveTimeTracksRepresentationFullIndividual(string rootFileName){

    if (m_is3D){
        // todo implement this
    }
    else{

        Float2DImage::IndexType index2d;
        FloatColor2DImage::PixelType pixel2d;

        // Create the output image and copy the inputs
        blImage* image = new blImage(m_framesFiles.at(0));
        int nl = image->imageSize().xSize();
        int nc = image->imageSize().ySize();
        std::string imageType = image->imageType();
        delete image;


        // get a random color for each track
        vector<vector<int> > randColors;
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){
            vector<int> randColor = blColor::GetRandRGB();
            randColors.push_back(randColor);
        }


        for (int f = 0 ; f < m_framesFiles.size() ; ++f){

            FloatColor2DImage::Pointer resImage = this->getIndividualRepresentationImage(imageType, nl, nc, f);

            // Plot the connexions on top of each projected frames
            for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

                // get a color
                vector<int> randColor = randColors[l];
                pixel2d[0] = randColor[0]; pixel2d[1] = randColor[1]; pixel2d[2] = randColor[2];

                // plot the link on each frames
                for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                    blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                    int xStart = connectionInter->stateStart().at(0);
                    int yStart = connectionInter->stateStart().at(1);
                    int xEnd = connectionInter->stateEnd().at(0);
                    int yEnd = connectionInter->stateEnd().at(1);

                    vector<int> px, py;
                    blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                    int frameIdx = connectionInter->endFrameIdx();
                    if (f >= frameIdx){
                        for (unsigned int i=0 ; i<px.size() ; ++i){
                            index2d[0]=px[i]; index2d[1]=py[i];

                            if (index2d[0] >= 0 && index2d[0] < nl && index2d[1] >= 0 && index2d[1] < nc){
                                resImage->SetPixel(index2d, pixel2d);
                            }
                        }
                    }

                    if (conn == 0 && connectionInter->startFrameIdx() == f){
                        index2d[0]=xStart; index2d[1]=yStart;
                        if (index2d[0] >= 0 && index2d[0] < nl && index2d[1] >= 0 && index2d[1] < nc){
                            resImage->SetPixel(index2d, pixel2d);
                        }
                    }
                }
            }
            string fileName = blStringOperations::getFileNameFromPath(m_framesFiles[f]);
            blImageIO::saveFloatColor2DImage(rootFileName + fileName, resImage, true);
        }

    }
}

FloatColor2DImage::Pointer blAtTrackerInterface::getIndividualRepresentationImage(std::string imageType, int nl, int nc, int i){

    Float2DImage::IndexType index2d;
    FloatColor2DImage::PixelType pixel2d;
    // get the image
    // for gray scaled images
    FloatColor2DImage::Pointer resImage;
    if (imageType == blImage::TypeFloat2D || imageType == blImage::TypeInt2D){
        Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesFiles.at(i));
        // normalize intensity
        typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        rescaleFilter->SetInput(image);
        rescaleFilter->SetOutputMinimum(0);
        rescaleFilter->SetOutputMaximum(255);
        rescaleFilter->Update();
        image = rescaleFilter->GetOutput();

        resImage = blImageCreate::createNewFloatColor2DImage(nl, nc);
        for (int m = 0 ; m < nl ; ++m){
            for (int n = 0 ; n < nc ; ++n){
                index2d[0] = m; index2d[1] = n;
                float value = image->GetPixel(index2d);
                pixel2d[0] = value; pixel2d[1] = value; pixel2d[2] = value;
                resImage->SetPixel(index2d, pixel2d);
            }
        }
    }
    else if(imageType == blImage::TypeFloatColor2D || imageType == blImage::TypeIntColor2D){
        resImage = blImageIO::loadToFloatColor2DImage(m_framesFiles.at(i));
    }
    return resImage;
}

void blAtTrackerInterface::saveTimeTracksRepresentationFull(string rootFileName){

    if (m_is3D){
        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;

        // Create the output image and copy the inputs

        Float3DImage::Pointer image3D = blImageIO::loadToFloat3DImage(m_framesFiles.at(0));
        blZProjection projection;
        projection.setInput(new blImage(image3D));
        projection.setProjectionType(blZProjection::Maximum);
        Float2DImage::Pointer image = projection.output()->itkFloat2DImagePointer();
        //Float2DImage::Pointer image = blIntegrateImage::Integrate3DToMip(image3D);

        int nl = image->GetLargestPossibleRegion().GetSize()[0];
        int nc = image->GetLargestPossibleRegion().GetSize()[1];
        FloatColor3DImage::Pointer resImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_framesFiles.size());

        for (unsigned int i = 0 ; i < m_framesFiles.size() ; ++i){

            if (i>0){
                Float3DImage::Pointer image3D = blImageIO::loadToFloat3DImage(m_framesFiles.at(i));
                blZProjection projection;
                projection.setInput(new blImage(image3D));
                projection.setProjectionType(blZProjection::Maximum);
                image = projection.output()->itkFloat2DImagePointer();
                //image = blIntegrateImage::Integrate3DToMip(image3D);
            }

            typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
            RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
            rescaleFilter->SetInput(image);
            rescaleFilter->SetOutputMinimum(0);
            rescaleFilter->SetOutputMaximum(255);
            rescaleFilter->Update();
            image = rescaleFilter->GetOutput();

            index3d[2] = i;
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = image->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        // Plot the connexions on top of each projected frames
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

            // get a color
            vector<int> randColor = blColor::GetRandRGB();
            pixel3d[0] = randColor[0]; pixel3d[1] = randColor[1]; pixel3d[2] = randColor[2];

            // plot the link on each frames
            for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);

                vector<int> px, py;
                blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                int frameIdx = connectionInter->endFrameIdx();
                //if (frameIdx> 12)
                //    cout << "plot m_tracks at " << l << "frame idx = " << frameIdx << endl;
                for (unsigned int rep = frameIdx ; rep < m_framesFiles.size() ; ++rep){
                    for (unsigned int i=0 ; i<px.size() ; ++i){
                        index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = rep;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }

                if (conn == 0){
                    index3d[0]=xStart; index3d[1]=yStart; index3d[2] = connectionInter->startFrameIdx();
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }

        blImageIO::saveFloatColor3DImage(rootFileName, resImage, true);
    }
    else{

        FloatColor3DImage::IndexType index3d;
        Float2DImage::IndexType index2d;
        FloatColor3DImage::PixelType pixel3d;

        // Create the output image and copy the inputs
        blImage* image = new blImage(m_framesFiles.at(0));
        int nl = image->imageSize().xSize();
        int nc = image->imageSize().ySize();
        std::string imageType = image->imageType();
        delete image;
        /*
        Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesFiles.at(0));
        // normalize intensity
        typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        rescaleFilter->SetInput(image);
        rescaleFilter->SetOutputMinimum(0);
        rescaleFilter->SetOutputMaximum(255);
        rescaleFilter->Update();
        image = rescaleFilter->GetOutput();

        int nl = image->GetLargestPossibleRegion().GetSize()[0];
        int nc = image->GetLargestPossibleRegion().GetSize()[1];
        */
        FloatColor3DImage::Pointer resImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_framesFiles.size());

        // Copy input frames in a 3D image
        // for gray scaled images
        if (imageType == blImage::TypeFloat2D || imageType == blImage::TypeInt2D){
            for (unsigned int i = 0 ; i < m_framesFiles.size() ; ++i){

                Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesFiles.at(i));
                // normalize intensity
                typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
                RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
                rescaleFilter->SetInput(image);
                rescaleFilter->SetOutputMinimum(0);
                rescaleFilter->SetOutputMaximum(255);
                rescaleFilter->Update();
                image = rescaleFilter->GetOutput();

                index3d[2] = i;
                for (int m = 0 ; m < nl ; ++m){
                    for (int n = 0 ; n < nc ; ++n){
                        index3d[0] = m; index3d[1] = n;
                        index2d[0] = m; index2d[1] = n;
                        float value = image->GetPixel(index2d);
                        pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
        }
        else if(imageType == blImage::TypeFloatColor2D || imageType == blImage::TypeIntColor2D){

            for (unsigned int i = 0 ; i < m_framesFiles.size() ; ++i){

                FloatColor2DImage::Pointer image = blImageIO::loadToFloatColor2DImage(m_framesFiles.at(i));
                index3d[2] = i;
                FloatColor2DImage::PixelType value;
                for (int m = 0 ; m < nl ; ++m){
                    for (int n = 0 ; n < nc ; ++n){
                        index3d[0] = m; index3d[1] = n;
                        index2d[0] = m; index2d[1] = n;
                        value = image->GetPixel(index2d);
                        pixel3d[0] = value.GetRed(); pixel3d[1] = value.GetGreen(); pixel3d[2] = value.GetBlue();
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }
            }
        }

        // Plot the connexions on top of each projected frames
        for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

            // get a color
            vector<int> randColor = blColor::GetRandRGB();
            pixel3d[0] = randColor[0]; pixel3d[1] = randColor[1]; pixel3d[2] = randColor[2];

            // plot the link on each frames
            for (unsigned int conn = 0 ; conn < m_tracks.at(l)->getTrackSize() ; ++conn){
                blAtConnection* connectionInter = m_tracks.at(l)->getConnectionAt(conn);
                int xStart = connectionInter->stateStart().at(0);
                int yStart = connectionInter->stateStart().at(1);
                int xEnd = connectionInter->stateEnd().at(0);
                int yEnd = connectionInter->stateEnd().at(1);

                vector<int> px, py;
                blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                int frameIdx = connectionInter->endFrameIdx();
                //if (frameIdx> 12)
                //    cout << "plot m_tracks at " << l << "frame idx = " << frameIdx << endl;
                for (unsigned int rep = frameIdx ; rep < m_framesFiles.size() ; ++rep){
                    for (unsigned int i=0 ; i<px.size() ; ++i){
                        index3d[0]=px[i]; index3d[1]=py[i]; index3d[2] = rep;
                        resImage->SetPixel(index3d, pixel3d);
                    }
                }

                if (conn == 0){
                    index3d[0]=xStart; index3d[1]=yStart; index3d[2] = connectionInter->startFrameIdx();
                    resImage->SetPixel(index3d, pixel3d);
                }
            }
        }
        blImageIO::saveFloatColor3DImage(rootFileName, resImage, true);
    }
}

blImage* blAtTrackerInterface::getTracksRepresentationOnFirstImage(){
    return new blImage(getTracksRepresentationOnFirstImageImp());
}

FloatColor2DImage::Pointer blAtTrackerInterface::getTracksRepresentationOnFirstImageImp(){

    Float2DImage::Pointer imageN_1;

    FloatColor2DImage::Pointer resImage;
    if (m_is3D){
        Float3DImage::Pointer curentFrame = blImageIO::loadToFloat3DImage(m_framesFiles.at(0));
        blZProjection projection;
        projection.setInput(new blImage(curentFrame));
        projection.setProjectionType(blZProjection::Maximum);
        imageN_1 = projection.output()->itkFloat2DImagePointer();
        //imageN_1 = blIntegrateImage::Integrate3DToMip(curentFrame);
    }
    else{
        imageN_1 = blImageIO::loadToFloat2DImage(m_framesFiles[0]);
        /*
        // Create the image
        vector<string> path;
        for (int i=0 ; i<m_framesNames.size() ; ++i){
            path.push_back(m_framesPath + m_framesNames[i]);
        }
        // project
        imageN_1 = IntegrateImage::Integrate3DToMip(allFrames);
        */
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
    for (unsigned int track = 0 ; track<m_tracks.size() ; ++track){

        // get a color
        vector<int> randColor = blColor::GetRandRGB();
        pixel[0] = randColor[0]; pixel[1] = randColor[1]; pixel[2] = randColor[2];

        // plot the link on each frames
        for (unsigned int conn = 0 ; conn < m_tracks.at(track)->getTrackSize() ; ++conn){
            blAtConnection* connectionInter = m_tracks.at(track)->getConnectionAt(conn);
            int xStart = connectionInter->stateStart().at(0);
            int yStart = connectionInter->stateStart().at(1);
            int xEnd = connectionInter->stateEnd().at(0);
            int yEnd = connectionInter->stateEnd().at(1);

            vector<int> px, py;
            blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
            for (unsigned int i=0 ; i<px.size() ; ++i){
                pixelIndex[0]=px[i]; pixelIndex[1]=py[i];
                resImage->SetPixel(pixelIndex, pixel);
            }
        }
    }

    return resImage;
}

void blAtTrackerInterface::saveAllConnectionsOnFirstFrame(string rootFileName){

    Float2DImage::Pointer imageN_1;

    FloatColor2DImage::Pointer resImage;
    if (m_is3D){
        Float3DImage::Pointer curentFrame = blImageIO::loadToFloat3DImage(m_framesFiles.at(0));
        blZProjection projection;
        projection.setInput(new blImage(curentFrame));
        projection.setProjectionType(blZProjection::Maximum);
        imageN_1 = projection.output()->itkFloat2DImagePointer();
        //imageN_1 = blIntegrateImage::Integrate3DToMip(curentFrame);
    }
    else{

        imageN_1 = blImageIO::loadToFloat2DImage(m_framesFiles[0]);
        /*
        // Create the image
        vector<string> path;
        for (int i=0 ; i<m_framesNames.size() ; ++i){
            path.push_back(m_framesPath + m_framesNames[i]);
        }
        Float3DImage::Pointer allFrames =  blImageIO::LoadToFloat3DImage(path);
        imageN_1 = IntegrateImage::Integrate3DToMip(allFrames);
        */
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

    // plot the connections
    pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;

    for (unsigned int conn = 0 ; conn < m_connections.size() ; ++conn){
        int xStart = m_connections[conn]->stateStart().at(0);
        int yStart = m_connections[conn]->stateStart().at(1);
        int xEnd = m_connections[conn]->stateEnd().at(0);
        int yEnd = m_connections[conn]->stateEnd().at(1);
        vector<int> px, py;
        blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
        for (unsigned int i=0 ; i<px.size() ; ++i){
            pixelIndex[0]=px[i]; pixelIndex[1]=py[i];
            resImage->SetPixel(pixelIndex, pixel);
        }
    }
    blImageIO::saveFloatColor2DImage(rootFileName, resImage);

}

void blAtTrackerInterface::saveTracksToTxtFile(string fileName){

    // 1- Open file
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < m_tracks.size() ; ++i){
            file << "track " << i << endl;
            for (unsigned int conn = 0 ; conn < m_tracks[i]->getTrackSize() ; ++conn ){
                blAtConnection* connectionInter = m_tracks.at(i)->getConnectionAt(conn);
                if (conn == 0){
                    vector<float> stateStart = connectionInter->stateStart();
                    int frameIdx = connectionInter->startFrameIdx();
                    for (unsigned int s = 0 ; s < stateStart.size() ; ++s){
                        file << stateStart.at(s) << "\t";
                    }
                    file << frameIdx << endl;
                }

                vector<float> stateEnd =  connectionInter->stateEnd();
                int frameIdx = connectionInter->endFrameIdx();
                for (unsigned int s = 0 ; s < stateEnd.size() ; ++s){
                    file << stateEnd.at(s) << "\t";
                }
                file << frameIdx << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }
}

void blAtTrackerInterface::saveTracksToTxtFile2(string fileName){

    // 1- Open file
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {

        file << "fileName\tObject #\tFrame #\tX\tY" << endl;
        // 2- Save each track
        for (unsigned int i = 0 ; i < m_tracks.size() ; ++i){
            //file << "track " << i << endl;
            for (unsigned int conn = 0 ; conn < m_tracks[i]->getTrackSize() ; ++conn ){
                blAtConnection* connectionInter = m_tracks.at(i)->getConnectionAt(conn);
                if (conn == 0){

                    file << "\t" << i+1 << "\t";

                    vector<float> stateStart = connectionInter->stateStart();
                    int frameIdx = connectionInter->startFrameIdx();
                    file << frameIdx+1 << "\t";
                    for (unsigned int s = 0 ; s < stateStart.size() ; ++s){
                        file << stateStart.at(s) << "\t";
                    }
                    file << endl;
                }

                file << "\t" << i+1 << "\t";
                vector<float> stateEnd =  connectionInter->stateEnd();
                int frameIdx = connectionInter->endFrameIdx();
                file << frameIdx+1 << "\t";
                for (unsigned int s = 0 ; s < stateEnd.size() ; ++s){
                    file << stateEnd.at(s) << "\t";
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

void blAtTrackerInterface::loadTracksFromTxtFile(string fileName){


    // 1- Open file
    ifstream file( fileName.c_str() );

    if ( file )
    {
        string line;
        string track = "track";
        string readedword; float val;
        vector<vector<float> > curentpoints;
        int pass = -1;
        while ( std::getline( file, line ) )
        {
            pass++;
            std::size_t found = line.find(track);
            if (found!=std::string::npos){
                if (pass > 0){
                    addTrack(curentpoints);
                    curentpoints.clear();
                }
            }
            else{

                // split the line
                vector<float> inter;
                stringstream stream(line);
                while( getline(stream, readedword, '\t')){
                    //val = std::stod(readedword);
                    blCast::from_string(readedword, val);
                    inter.push_back(val);
                }
                curentpoints.push_back(inter);
            }
        }
        addTrack(curentpoints);
        file.close();
        m_tracksAlreadyCalculated = true;
    }
    else{
        cerr << "Unable to open the file !" << fileName << endl;
    }
}

void blAtTrackerInterface::addTrack(vector<vector<float> > curentpoints){
    blAtTrack *track = new blAtTrack;
    for (unsigned int i = 0 ; i < curentpoints.size()-1 ; ++i){
        vector<float> stateStart, stateEnd; int startFrameId, endFrameId;
        stateStart.resize(curentpoints[i].size()-1);
        stateEnd.resize(curentpoints[i].size()-1);
        for (unsigned int k = 0 ; k < curentpoints[i].size()-1 ; ++k){
            stateStart[k] = curentpoints[i][k];
            stateEnd[k] = curentpoints[i+1][k];
        }
        startFrameId = curentpoints[i][curentpoints[i].size()-1];
        endFrameId = curentpoints[i+1][curentpoints[i].size()-1];
        blAtConnection* connection = new blAtConnection(stateStart, stateEnd, startFrameId, endFrameId);
        track->addConnection(connection);
    }
    m_tracks.push_back(track);
}

vector<vector<int> > blAtTrackerInterface::getTracksCosts(){
    return m_tracksCosts;
}

std::vector<blAtTrack* > blAtTrackerInterface::getTracks(){
    return m_tracks;
}
