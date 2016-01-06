/// \file blRATFramesLoader.cpp
/// \brief blRATFramesLoader class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blRATFramesLoader.h"

#include "blCore/blImage.h"
#include "blCore/blColor.h"
#include <blGeometry>

#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
using namespace std;

blRATFramesLoader::blRATFramesLoader(){

}

blRATFramesLoader::~blRATFramesLoader(){

}

void blRATFramesLoader::setFramesUrls(std::vector<std::string> framesUrls){
    m_framesUrls = framesUrls;
}

unsigned int blRATFramesLoader::frameNumber(){
    return unsigned int(m_framesUrls.size());
}

void blRATFramesLoader::setTracks(std::vector<blRATTrack*> tracks){
    m_tracks = tracks;
}

void blRATFramesLoader::saveTimeTracksRepresentation(std::string dirURL){
    float m_is3D = false;
    if (m_is3D){
        // todo implement this
    }
    else{

        Float2DImage::IndexType index2d;
        FloatColor2DImage::PixelType pixel2d;

        // Create the output image and copy the inputs
        blImage* image = new blImage(m_framesUrls.at(0));
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


        for (int f = 0 ; f < m_framesUrls.size() ; ++f){

            FloatColor2DImage::Pointer resImage = this->getIndividualRepresentationImage(imageType, nl, nc, f);

            // Plot the connexions on top of each projected frames
            for (unsigned int l = 0 ; l < m_tracks.size() ; ++l){

                // get a color
                vector<int> randColor = randColors[l];
                pixel2d[0] = randColor[0]; pixel2d[1] = randColor[1]; pixel2d[2] = randColor[2];

                // plot the link on each frames
                for (unsigned int st = 1 ; st < m_tracks.at(l)->getTrackSize() ; ++st){
                    blRATState* stateBefore = m_tracks.at(l-1)->getStateAt(st);
                    blRATState* stateCurent = m_tracks.at(l)->getStateAt(st);
                    int xStart = stateBefore->getStateAt(0);
                    int yStart = stateBefore->getStateAt(1);
                    int xEnd = stateCurent->getStateAt(0);
                    int yEnd = stateCurent->getStateAt(1);
                    int trackFrameIdx = m_tracks.at(l)->getFrameIdxAt(st);


                    vector<int> px, py;
                    blMathGeometry::Calculate2DLineCoordinates(xStart,yStart,xEnd,yEnd, px, py);
                    if (f >= trackFrameIdx){
                        for (unsigned int i=0 ; i<px.size() ; ++i){
                            index2d[0]=px[i]; index2d[1]=py[i];
                            resImage->SetPixel(index2d, pixel2d);
                        }
                    }

                    if (l-1 == 0 && m_tracks.at(l)->getFrameIdxAt(st-1) == f){
                        index2d[0]=xStart; index2d[1]=yStart;
                        resImage->SetPixel(index2d, pixel2d);
                    }
                }
            }
            string fileName = blStringOperations::getFileNameFromPath(m_framesUrls[f]);
            blImageIO::saveFloatColor2DImage(dirURL + fileName, resImage, true);
        }
    }
}

FloatColor2DImage::Pointer blRATFramesLoader::getIndividualRepresentationImage(std::string imageType, int nl, int nc, int i){

    Float2DImage::IndexType index2d;
    FloatColor2DImage::PixelType pixel2d;
    // get the image
    // for gray scaled images
    FloatColor2DImage::Pointer resImage;
    if (imageType == blImage::TypeFloat2D || imageType == blImage::TypeInt2D){
            Float2DImage::Pointer image = blImageIO::loadToFloat2DImage(m_framesUrls.at(i));
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
            resImage = blImageIO::loadToFloatColor2DImage(m_framesUrls.at(i));
    }
    return resImage;
}
