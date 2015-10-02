/// \file blMppGeneratorShapeTestClusters.h
/// \brief blMppGeneratorShapeTestClusters class
/// \author Emmanuel Soubies (Emmanuel.Soubies@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShape.h"
#include "blMppShapeSuperquadric2D.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeTestClusters
/// \brief Generator of 2D superquadric defined by blMppShapeSuperquadric2D
class BLMPP_EXPORT blMppGeneratorShapeTestClusters : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeTestClusters();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeTestClusters();
    /// \fn blMppGeneratorShapeTestClusters(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeTestClusters(blImage* birthMap);
    /// \fn blMppGeneratorShapeTestClusters(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeTestClusters(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeTestClusters();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeTestClusters();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:

    void setShapeFile(std::string fileUrl);

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

    void initParametersWithDefaultsValues();
    void translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated);


    std::string m_fileUrl;
    std::vector<std::vector<float> > m_Shapes;
    int m_shapeCounter;
    int m_maxSize;
};
