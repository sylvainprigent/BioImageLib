/// \file blMppGeneratorShapeRectangle.h
/// \brief blMppGeneratorShapeRectangle class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShape.h"
#include "blMppShapeRectangle.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeRectangle
/// \brief Generator of 2D rectangle defined by blMppShapeRectangle
class BLMPP_EXPORT blMppGeneratorShapeRectangle : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeRectangle();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangle();
    /// \fn blMppGeneratorShapeRectangle(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangle(blImage* birthMap);
    /// \fn blMppGeneratorShapeRectangle(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangle(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeRectangle();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeRectangle();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:
    // setters
    /// \fn void setMajorAxisRange(int aMin, int aMax);
    /// \param[in] aMin minimum possible value of a
    /// \param[in] aMax maximum possible value of a
    void setMajorAxisRange(int aMin, int aMax);
    /// \fn void setMinorAxisRange(int bMin, int bMax);
    /// \param[in] bMin minimum possible value of b
    /// \param[in] bMax maximum possible value of b
    void setMinorAxisRange(int bMin, int bMax);
    /// \fn void setThetaRange(float thetaMin, float thetaMax, float thetaStep);
    /// \param[in] thetaMin Minimum value of the shape orientation
    /// \param[in] thetaMax Maximum value of of the shape orientation
    /// \param[in] thetaStep Discretisation of theta between thetaMin and thetaMax
    void setThetaRange(float thetaMin, float thetaMax, float thetaStep);

public:
    /// \fn void initialize();
    /// \brief reimplemented from blMppGeneratorShape
    virtual void initialize();
    /// \fn virtual blMppShape* generateRandomUsingMask();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingMask();
    /// \fn virtual blMppShape* generateRandomUsingArea();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingArea();
    /// \fn virtual int shapesMaxLength();
    /// \brief reimplemented from blMppGeneratorShape
    virtual int shapesMaxLength();

    /// \fn blMppShape* translate(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* translate(blMppShape* shape);
    /// \fn blMppShape* rotate(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* rotate(blMppShape* shape);
    /// \fn blMppShape* resize(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* resize(blMppShape* shape);

protected:
    // internal function
    void initParametersWithDefaultsValues();
    void translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated);
    void copyNormals(blMppShapeRectangle* templateShape, blMppShapeRectangle *newShape);
    void calculateShapeData(blPixel centerPixel, blMppShapeRectangle* templateShape, blMppShapeRectangle* newShape);
    int getIdx(int* idx, int* n, int size);

    // parameters
    int m_aMin; int m_aMax; int m_aRange;
    int m_bMin; int m_bMax; int m_bRange;
    float m_thetaMin, m_thetaMax, m_thetaStep;
    int m_anglesNumber;

    // internal data
    std::vector<blMppShapeRectangle*> m_shapeLibrary;
};
