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
//                    blAtCostLineSize
// ////////////////////////////////////////////////////////// //
/// \class blAtCostLineSize
/// \brief Define a cost function for axon tracking using the vesselness between
/// two detections
class BLASSOCIATIONTRACKING_EXPORT blAtCostLineSize : public blAtCostInterface{
public:

    /// \fn blAtCostLineSize(ParamManager* paramManager);
    /// \brief Construcor
    blAtCostLineSize();

public:
    // parameters
    void initParametersWithDefaultsValues();

public:
    //setters
    void setMaxMove(float maxMoveX, float maxMoveY);
    void setMinMove(float minMove);
    void setLineImageDir(std::string lineImageDir);
    void setCoefficients(float alpha, float beta, float gamma);

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
    float m_alpha, m_beta, m_gamma;

    // methods    
    float calculateLine(blAtConnection *connection);
    void get2DMeanLine(std::vector<int> startPoint, std::vector<int> endPoint, float &v_mean);
    float calculateObservationDiff(blAtConnection *connection);
    float calculateAngleDiff(blAtConnection *connection);

};
