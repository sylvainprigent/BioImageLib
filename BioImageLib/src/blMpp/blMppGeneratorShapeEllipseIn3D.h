/// \file blMppGeneratorShapeEllipseIn3D.h
/// \brief blMppGeneratorShapeEllipseIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShapeEllipse.h"
#include "blMppShapeEllipse.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeEllipseIn3D
/// \brief Generator of 2D ellipse defined by blMppShapeEllipse in any
/// (x,y) layer of a 3D image
class BLMPP_EXPORT blMppGeneratorShapeEllipseIn3D : public blMppGeneratorShapeEllipse {

public:
    /// \fn blMppGeneratorShapeEllipseIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeEllipseIn3D();
    /// \fn blMppGeneratorShapeEllipseIn3D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeEllipseIn3D(blImage* birthMap);
    /// \fn blMppGeneratorShapeEllipseIn3D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeEllipseIn3D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeEllipseIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeEllipseIn3D();

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
