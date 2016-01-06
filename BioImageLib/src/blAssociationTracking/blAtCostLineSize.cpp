/// \file blAtCostLineSize.cpp
/// \brief blAtCostLineSize class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blAtCostLineSize.h"
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
//                    blAtCostLineSize
// ////////////////////////////////////////////////////////// //
blAtCostLineSize::blAtCostLineSize(): blAtCostInterface(){
    m_featureImage = NULL;
    initParametersWithDefaultsValues();
}

// ///////////////// parameters //////////////////////
void blAtCostLineSize::initParametersWithDefaultsValues(){
    m_maxMoveX = 10.0;
    m_maxMoveY = 10.0;
    m_minMove = 2;
}


// setters
bool blAtCostLineSize::isProbability(){
    return true;
}

void blAtCostLineSize::setMaxMove(float maxMoveX, float maxMoveY){
    m_maxMoveX = maxMoveX;
    m_maxMoveY = maxMoveY;
}

void blAtCostLineSize::setMinMove(float minMove){
    m_minMove = minMove;
}

void blAtCostLineSize::setLineImageDir(std::string lineImageDir){
    m_lineImageDir = lineImageDir;
}

void blAtCostLineSize::setCoefficients(float alpha, float beta, float gamma){
    m_alpha = alpha;
    m_beta = beta;
    m_gamma = gamma;
}

// ///////////////////////////////////////////////////
void blAtCostLineSize::loadDataCurentFrames(int frame1, int frame2){
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
        std::string message = "blAtCostLineSize can only process " + blImage::TypeFloat2D + "files. The image you gave are " + m_featureImage->imageType();
        throw blException(message.c_str());
    }
}

float blAtCostLineSize::calculateCost(blAtConnection* connection){

    // 1- check the distance between detection is shall enough
    float euclX = connection->stateStart().at(0) - connection->stateEnd().at(0);
    float euclY = connection->stateStart().at(1) - connection->stateEnd().at(1);

    int delta_t = (m_frame2-m_frame1);
    if ((fabs(euclX) > float(delta_t*m_maxMoveX) ) || (fabs(euclY) > float(delta_t*m_maxMoveY)) ){
        return 1.0;
    }
    else{
        // 2- Calculate the cost = vesselness
        float vesselness = exp(-m_alpha*calculateLine(connection));
        float observationDiff = exp(-m_beta*(1.0-calculateObservationDiff(connection)));
        float angleDiff = exp(-m_gamma*(1.0-calculateAngleDiff(connection)));

        //return 1.0-angleDiff;
        //return 1.0-calculateAngleDiff(connection);
        //return 1.0-calculateObservationDiff(connection);
        //return calculateLine(connection);
        return 1-vesselness*observationDiff*angleDiff;
    }
}

float blAtCostLineSize::calculateLine(blAtConnection *connection){

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

void blAtCostLineSize::get2DMeanLine(vector<int> startPoint, vector<int> endPoint, float &v_mean){

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

float blAtCostLineSize::calculateObservationDiff(blAtConnection *connection){

    float areaBefore = connection->stateStart().at(3)*connection->stateStart().at(4);
    float area = connection->stateEnd().at(3)*connection->stateEnd().at(4);


    if (area > areaBefore){
        //std::cout << "areas = " <<  area << ", " << areaBefore << ", ratio = " << areaBefore/area << ", energy = " << exp(-m_delta*(1.0-areaBefore/area)) << std::endl;
        return areaBefore/area;
    }
    //std::cout << "areas = " <<  area << ", " << areaBefore << ", ratio = " << area/areaBefore << ", energy = " << exp(-m_delta*(1.0-area/areaBefore)) << std::endl;
    return area/areaBefore;
}

float blAtCostLineSize::calculateAngleDiff(blAtConnection *connection){

    float angleBefore = connection->stateStart().at(5);
    float angle = connection->stateEnd().at(5);

    //std::cout << "angle 1 = " << angleBefore << "angle 2 = " << angle <<  "angle diff = " <<  1-fabs(angleBefore - angle)/M_PI << std::endl;
    return 1-fabs(angleBefore - angle)/M_PI;
}
