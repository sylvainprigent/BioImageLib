/// \file blMppDataTermBhattacharyyaCircleConstraint.h
/// \brief blMppDataTermBhattacharyyaCircleConstraint class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "blMppExport.h"

/// \class blMppDataTermBhattacharyyaCircleConstraint
/// \brief Class that define an Marked Point Process data term
/// as the Bhattacharyya distance calculated between the distributions
/// of the pixels inside and the pixels in the border of a shape
/// This dataterm is combined with a constraint that favor circle shapes
class BLMPP_EXPORT blMppDataTermBhattacharyyaCircleConstraint : public blMppDataTerm{

public:
    /// \fn blMppDataTermBhattacharyyaCircleConstraint();
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermBhattacharyyaCircleConstraint();
    /// \fn blMppDataTermBhattacharyyaCircleConstraint(blImage* image);
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermBhattacharyyaCircleConstraint(blImage* image);
    /// \fn virtual ~blMppDataTermBhattacharyyaCircleConstraint();
    /// \brief Reimplement from blMppDataTerm
    virtual ~blMppDataTermBhattacharyyaCircleConstraint();

public:
    /// \fn virtual void initialize();
    /// \brief Reimplement from blMppDataTerm
    virtual void initialize();
    /// \fn virtual float compute(blMppShape* shape);
    /// \brief Reimplement from blMppDataTerm
    virtual float compute(blMppShape* shape);
    /// \fn virtual void createCompatibleShapesList();
    /// \brief Reimplement from blMppDataTerm
    virtual void createCompatibleShapesList();

    /// \fn virtual bool useInsidePixels();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useInsidePixels();
    /// \fn virtual bool useBorderPixels();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useBorderPixels();
    /// \fn virtual bool useNormals();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useNormals();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // setters parameters
    /// \fn void setD(float D);
    /// \param[in] D Slope of the exponential function that
    /// fit the Bhattacharyya distance into [-1,1]
    void setD(float D);
    /// \fn void setGradientDirection(int gradientDirection);
    /// \param[in] gradientDirection Color of the object
    ///          - (1) Light shapes in dark background
    ///          - (-1)Dark shapes in light background
    ///          - (0) Both light and dark shapes
    void setGradientDirection(int gradientDirection);

    /// \fn void setMedianRadius(float radius);
    /// \param[in] radius Median radius of the proposed shapes
    void setMedianRadius(float radius);

    /// \fn void setLambda(float lambda);
    /// \param[in] lambda Weight parameter between bhattacharyya and shape constraints
    void setLambda(float lambda);

protected:
    float m_D; ///< see setD()
    int m_gradientDirection; ///< see setGradientDirection()
    float m_r_median; ///< median radius of the proposed shapes
    float m_lambda; ///< weight parameter between bhattacharyya and shape constraints

protected:
    // internal methods
    /// \fn void computeStats(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder);
    /// \brief Compute the statistics of a shapes pixels values for the Bhattacharyya distance
    void computeStats(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder,float& r1, float& r2);
};
