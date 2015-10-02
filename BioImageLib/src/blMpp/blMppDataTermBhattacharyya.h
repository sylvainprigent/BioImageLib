/// \file blMppDataTermBhattacharyya.h
/// \brief blMppDataTermBhattacharyya class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "blMppExport.h"

/// \class blMppDataTermBhattacharyya
/// \brief Class that define an Marked Point Process data term
/// as the Bhattacharyya distance calculated between the distributions
/// of the pixels inside and the pixels in the border of a shape
class BLMPP_EXPORT blMppDataTermBhattacharyya : public blMppDataTerm{

public:
    /// \fn blMppDataTermBhattacharyya();
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermBhattacharyya();
    /// \fn blMppDataTermBhattacharyya(blImage* image);
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermBhattacharyya(blImage* image);
    /// \fn virtual ~blMppDataTermBhattacharyya();
    /// \brief Reimplement from blMppDataTerm
    virtual ~blMppDataTermBhattacharyya();

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

protected:
    float m_D; ///< slop of the threshold function
    int m_gradientDirection; ///< see setGradientDirection

protected:
    bool m_is2DImage; ///< internal attribut to know the image type

protected:
    // internal methods
    /// \fn void computeStats2D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder);
    /// \brief Compute the statistics of a 2D shapes pixels values for the Bhattacharyya distance
    void computeStats2D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder);
    /// \fn void computeStats3D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder);
    /// \brief Compute the statistics of a 3D shapes pixels values for the Bhattacharyya distance
    void computeStats3D(blMppShape *shape, float &meanIn, float &meanBorder, float &varIn, float &varBorder);

};
