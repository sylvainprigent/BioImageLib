/// \file blMppGeneratorShapeSphere.h
/// \brief blMppGeneratorShapeSphere class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppShapeSphere.h"
#include "blMppGeneratorShape.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeSphere
/// \brief Generator of 2D superquadric defined by blMppShapeSuperquadric2D
class BLMPP_EXPORT blMppGeneratorShapeSphere : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeSphere();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSphere();
    /// \fn blMppGeneratorShapeSphere(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSphere(blImage* birthMap);
    /// \fn blMppGeneratorShapeSphere(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSphere(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeSphere();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeSphere();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:
    // setters
    /// \fn void setRadiusRange(int minR, int maxR);
    /// \param[in] minR Minimum radius
    /// \param[in] maxR Maximum radius
    void setRadiusRange(int minR, int maxR);

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
    void copyNormals(blMppShapeSphere* templateShape, blMppShapeSphere *newShape);
    void calculateShapeData(blPixel centerPixel, blMppShapeSphere *templateShape, blMppShapeSphere *newShape);

    // parameters
    int m_rMin; int m_rMax; int m_rRange;

    // internal data
    std::vector<blMppShapeSphere*> m_shapeLibrary;
};
