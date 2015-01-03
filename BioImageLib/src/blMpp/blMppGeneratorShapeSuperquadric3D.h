/// \file blMppGeneratorShapeSuperquadric3D.h
/// \brief blMppGeneratorShapeSuperquadric3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShape.h"
#include "blMppShapeSuperquadric3D.h"

/// \class blMppGeneratorShapeSuperquadric3D
/// \brief Generator of 2D superquadric defined by blMppShapeSuperquadric2D
class blMppGeneratorShapeSuperquadric3D : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeSuperquadric3D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric3D();
    /// \fn blMppGeneratorShapeSuperquadric3D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric3D(blImage* birthMap);
    /// \fn blMppGeneratorShapeSuperquadric3D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric3D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeSuperquadric3D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeSuperquadric3D();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:
    // setters
    /// \fn void setAxisRange_a(int aMin, int aMax);
    /// \param[in] aMin minimum possible value of a
    /// \param[in] aMax maximum possible value of a
    void setAxisRange_a(int aMin, int aMax);
    /// \fn void setAxisRange_b(int bMin, int bMax);
    /// \param[in] bMin minimum possible value of b
    /// \param[in] bMax maximum possible value of b
    void setAxisRange_b(int bMin, int bMax);
    /// \fn void setAxisRange_c(int cMin, int cMax);
    /// \param[in] bMin minimum possible value of c
    /// \param[in] bMax maximum possible value of c
    void setAxisRange_c(int cMin, int cMax);
    /// \fn void setExposantRange_m(float mMin, float mMax, float mStep);
    /// \param[in] mMin Minimum value of m
    /// \param[in] mMax Maximum value of m
    /// \param[in] mStep Discretisation of m between mMin and mMax
    void setExposantRange_m(float mMin, float mMax, float mStep);
    /// \fn void setExposantRange_n(float nMin, float nMax, float nStep);
    /// \param[in] nMin Minimum value of n
    /// \param[in] nMax Maximum value of n
    /// \param[in] nStep Discretisation of m between nMin and nMax
    void setExposantRange_n(float nMin, float nMax, float nStep);
    /// \fn void setExposantRange_p(float nMin, float nMax, float nStep);
    /// \param[in] pMin Minimum value of n
    /// \param[in] pMax Maximum value of n
    /// \param[in] pStep Discretisation of m between pMin and pMax
    void setExposantRange_p(float pMin, float pMax, float pStep);
    /// \fn void setPhiRange(float phiMin, float phiMax, float phiStep);
    /// \param[in] phiMin Minimum value of the shape orientation
    /// \param[in] phiMax Maximum value of of the shape orientation
    /// \param[in] phiStep Discretisation of theta between phiMin and phiMax
    void setPhiRange(float phiMin, float phiMax, float phiStep);
    /// \fn void setThetaRange(float thetaMin, float thetaMax, float thetaStep);
    /// \param[in] thetaMin Minimum value of the shape orientation
    /// \param[in] thetaMax Maximum value of of the shape orientation
    /// \param[in] thetaStep Discretisation of theta between thetaMin and thetaMax
    void setThetaRange(float thetaMin, float thetaMax, float thetaStep);
    /// \fn void setGammaRange(float gammaMin, float gammaMax, float gammaStep);
    /// \param[in] gammaMin Minimum value of the shape orientation
    /// \param[in] gammaMax Maximum value of of the shape orientation
    /// \param[in] gammaStep Discretisation of theta between gammaMin and gammaMax
    void setGammaRange(float gammaMin, float gammaMax, float gammaStep);

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
    void copyNormals(blMppShapeSuperquadric3D* templateShape, blMppShapeSuperquadric3D *newShape);
    void calculateShapeData(blPixel centerPixel, blMppShapeSuperquadric3D* templateShape, blMppShapeSuperquadric3D* newShape);
    int getIdx(int* idx, int* n, int size);

    // parameters
    int m_aMin; int m_aMax; int m_aRange;
    int m_bMin; int m_bMax; int m_bRange;
    int m_cMin; int m_cMax; int m_cRange;
    float m_mMin; float m_mMax; float m_mStep;
    float m_nMin; float m_nMax; float m_nStep;
    float m_pMin; float m_pMax; float m_pStep;
    unsigned int m_exposantXSpace;
    unsigned int m_exposantYSpace;
    unsigned int m_exposantZSpace;
    float m_phiMin, m_phiMax, m_phiStep;
    float m_thetaMin, m_thetaMax, m_thetaStep;
    float m_gammaMin, m_gammaMax, m_gammaStep;
    int m_anglesNumberPhi;
    int m_anglesNumberTheta;
    int m_anglesNumberGamma;

    // internal data
    std::vector<blMppShapeSuperquadric3D*> m_shapeLibrary;
};
