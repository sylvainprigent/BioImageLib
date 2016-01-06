/// \file blAtCostLine.cpp
/// \brief blAtCostLine class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blAtCostLine.h"
#include "blFiltering/blFrangiFilter.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkGrayscaleDilateImageFilter.h"
#include "itkGrayscaleErodeImageFilter.h"

#include <blCore>
#include <blCastStacks>
#include "blGeometry/blMathGeometry.h"
#include "blMath/blMath.h"
#include "blCastStacks/blProjectionImage.h"

using namespace std;

// ////////////////////////////////////////////////////////// //
//                    blAtCostLine
// ////////////////////////////////////////////////////////// //
blAtCostLine::blAtCostLine(): blAtCostInterface(){
    m_featureImage = NULL;
    initParametersWithDefaultsValues();
}

// ///////////////// parameters //////////////////////
void blAtCostLine::initParametersWithDefaultsValues(){
    m_maxMoveX = 10.0;
    m_maxMoveY = 10.0;
    m_minMove = 2;
}


// setters
bool blAtCostLine::isProbability(){
    return true;
}

void blAtCostLine::setMaxMove(float maxMoveX, float maxMoveY){
    m_maxMoveX = maxMoveX;
    m_maxMoveY = maxMoveY;
}

void blAtCostLine::setMinMove(float minMove){
    m_minMove = minMove;
}

void blAtCostLine::setLineImageDir(std::string lineImageDir){
    m_lineImageDir = lineImageDir;
}

// ///////////////////////////////////////////////////
void blAtCostLine::loadDataCurentFrames(int frame1, int frame2){
    m_frame1 = frame1;
    m_frame2 = frame2;

    if (m_featureImage){
        delete m_featureImage;
    }
    string fileName = blStringOperations::getFileNameFromPath(m_framesFiles[frame2]);
    fileName.replace(fileName.end()-4,fileName.end(),".png");

    m_featureImage = new blImage(m_lineImageDir + fileName);
    if ( m_featureImage->imageType() == blImage::TypeInt2D){
        blCastImageToFloat caster;
        caster.setInput(m_featureImage);
        caster.run();
        blImage* imageC = caster.output();
        delete m_featureImage;
        m_featureImage = imageC;
        imageC = NULL;
    }
    if (m_featureImage->imageType() != blImage::TypeFloat2D){
        std::string message = "blAtCostLine can only process " + blImage::TypeFloat2D + "files. The image you gave are " + m_featureImage->imageType();
        throw blException(message.c_str());
    }
}

float blAtCostLine::calculateCost(blAtConnection* connection){

    // 1- check the distance between detection is shall enough
    float euclX = connection->stateStart().at(0) - connection->stateEnd().at(0);
    float euclY = connection->stateStart().at(1) - connection->stateEnd().at(1);

    int delta_t = (m_frame2-m_frame1);
    if ((fabs(euclX) > float(delta_t*m_maxMoveX) ) || (fabs(euclY) > float(delta_t*m_maxMoveY)) ){
        return 1.0;
    }
    else{
        // 2- Calculate the cost = vesselness
        float vesselness = calculateLine(connection);
        return vesselness;
    }
}

float blAtCostLine::calculateLine(blAtConnection *connection){

    // 1- Get starting and ending point
    vector<int> startPoint;
    startPoint.push_back(connection->stateStart()[0]);
    startPoint.push_back(connection->stateStart()[1]);
    vector<int> endPoint;
    endPoint.push_back(connection->stateEnd()[0]);
    endPoint.push_back(connection->stateEnd()[1]);

    double pow1 = pow((double)(startPoint[0]-endPoint[0]),2);
    double pow2 = pow((double)(startPoint[1]-endPoint[1]),2);
    if ( sqrt( pow1 + pow2 ) < m_minMove){
        return 0.0;
    }

    // 2- Calcualte the mean intensities between points
    float v_mean = 0.0;
    get2DMeanLine(startPoint, endPoint, v_mean);

    v_mean = v_mean/255;
    if (blMath::isNan(v_mean)){
        return 1.0;
    }
    else{
        return 1-v_mean;
    }
}

void blAtCostLine::get2DMeanLine(vector<int> startPoint, vector<int> endPoint, float &v_mean){

    // 2.1- Calculate the line between the two detections
    vector<int> lineX, lineY;
    blMathGeometry::Calculate2DLineCoordinates(startPoint[0], startPoint[1], endPoint[0], endPoint[1], lineX, lineY);

    // 2.2- Calculate the mean intensities in the line
    Float2DImage::Pointer lineImagePointer = m_featureImage->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;
    float val, n;
    for (unsigned int i = 0 ; i < lineX.size() ; ++i){
        pixelIndex[0] = lineX[i];
        pixelIndex[1] = lineY[i];
        val = lineImagePointer->GetPixel(pixelIndex);
        v_mean += val;
    }
    n = float(lineX.size());

    v_mean /= n;
}
