/// \file blMppGeneratorShapeRectangleIn3D.h
/// \brief blMppGeneratorShapeRectangleIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShapeRectangle.h"
#include "blMppShapeRectangle.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeRectangleIn3D
/// \brief Generator of 2D rectangle defined by blMppShapeRectangle in any
/// (x,y) layer of a 3D image
class BLMPP_EXPORT blMppGeneratorShapeRectangleIn3D : public blMppGeneratorShapeRectangle {

public:
    /// \fn blMppGeneratorShapeRectangleIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangleIn3D();
    /// \fn blMppGeneratorShapeRectangleIn3D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangleIn3D(blImage* birthMap);
    /// \fn blMppGeneratorShapeRectangleIn3D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeRectangleIn3D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeRectangleIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeRectangleIn3D();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:
    /// \fn virtual blMppShape* generateRandomUsingMask();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingMask();
    /// \fn virtual blMppShape* generateRandomUsingArea();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingArea();

    /// \fn blMppShape* translate(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* translate(blMppShape* shape);

};
