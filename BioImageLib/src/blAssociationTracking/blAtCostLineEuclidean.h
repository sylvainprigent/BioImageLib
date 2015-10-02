/// \file blAtCostAxons.h
/// \brief blAtCostAxonVesselness2 class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#pragma once

#include "blAtCostInterface.h"
#include <blCore>
#include "blAssociationTrackingExport.h"

// ////////////////////////////////////////////////////////// //
//                    blAtCostLine
// ////////////////////////////////////////////////////////// //
/// \class blAtCostLine
/// \brief Define a cost function for axon tracking using the vesselness between
/// two detections
class BLASSOCIATIONTRACKING_EXPORT blAtCostLineEuclidean : public blAtCostInterface{
public:

    /// \fn blAtCostLine(ParamManager* paramManager);
    /// \brief Construcor
    blAtCostLineEuclidean();

public:
    // parameters
    void initParametersWithDefaultsValues();

public:
    //setters
    void setMaxMove(float maxMoveX, float maxMoveY);
    void setMinMove(float minMove);
    void setCoefficients(float alpha, float beta);
    void setLineImageDir(std::string lineImageDir);

public:
    void loadDataCurentFrames(int frame1, int frame2);
    float calculateCost(blAtConnection *connection);
    bool isProbability();

private:
    // attributs
    int m_frame1, m_frame2;
    blImage* m_featureImage;

    std::string m_lineImageDir;
    float m_maxMoveX, m_maxMoveY;
    float m_minMove;

    float m_alpha, m_beta;

    // methods    
    float calculateLine(blAtConnection *connection);
    void get2DMeanLine(std::vector<int> startPoint, std::vector<int> endPoint, float &v_mean);
};
