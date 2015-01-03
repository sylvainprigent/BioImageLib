/// \file blMppGeneratorShapeSuperquadric2D.h
/// \brief blMppGeneratorShapeSuperquadric2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShape.h"
#include "blMppShapeSuperquadric2D.h"

/// \class blMppGeneratorShapeSuperquadric2D
/// \brief Generator of 2D superquadric defined by blMppShapeSuperquadric2D
class blMppGeneratorShapeSuperquadric2D : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeSuperquadric2D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2D();
    /// \fn blMppGeneratorShapeSuperquadric2D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2D(blImage* birthMap);
    /// \fn blMppGeneratorShapeSuperquadric2D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeSuperquadric2D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeSuperquadric2D();

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
    /// \fn void setMajorExposantRange(float mMin, float mMax, float mStep);
    /// \param[in] mMin Minimum value of m
    /// \param[in] mMax Maximum value of m
    /// \param[in] mStep Discretisation of m between mMin and mMax
    void setMajorExposantRange(float mMin, float mMax, float mStep);
    /// \fn void setMinorExposantRange(float nMin, float nMax, float nStep);
    /// \param[in] nMin Minimum value of n
    /// \param[in] nMax Maximum value of n
    /// \param[in] nStep Discretisation of m between nMin and nMax
    void setMinorExposantRange(float nMin, float nMax, float nStep);
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
    void copyNormals(blMppShapeSuperquadric2D* templateShape, blMppShapeSuperquadric2D *newShape);
    void calculateShapeData(blPixel centerPixel, blMppShapeSuperquadric2D* templateShape, blMppShapeSuperquadric2D* newShape);
    int getIdx(int* idx, int* n, int size);

    // parameters
    int m_aMin; int m_aMax; int m_aRange;
    int m_bMin; int m_bMax; int m_bRange;
    float m_mMin; float m_mMax; float m_mStep;
    float m_nMin; float m_nMax; float m_nStep;
    unsigned int m_exposantXSpace;
    unsigned int m_exposantYSpace;
    float m_thetaMin, m_thetaMax, m_thetaStep;
    int m_anglesNumber;

    // internal data
    std::vector<blMppShapeSuperquadric2D*> m_shapeLibrary;
};
