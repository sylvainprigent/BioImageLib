/// \file blMppGeneratorShapeCircleIn3D.h
/// \brief blMppGeneratorShapeCircleIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppShapeCircle.h"
#include "blMppGeneratorShapeCircle.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeCircleIn3D
/// \brief Generator of 2D circles in each (x,y) layers of a 3D image
class BLMPP_EXPORT blMppGeneratorShapeCircleIn3D : public blMppGeneratorShapeCircle {

public:
    /// \fn blMppGeneratorShapeCircleIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeCircleIn3D();
    /// \fn blMppGeneratorShapeCircleIn3D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeCircleIn3D(blImage* birthMap);
    /// \fn blMppGeneratorShapeCircleIn3D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeCircleIn3D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeCircleIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeCircleIn3D();

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
