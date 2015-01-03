/// \file blMppGeneratorShapeSuperquadric2DIn3D.h
/// \brief blMppGeneratorShapeSuperquadric2DIn3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppShapeSuperquadric2D.h"
#include "blMppGeneratorShapeSuperquadric2D.h"

/// \class blMppGeneratorShapeSuperquadric2DIn3D
/// \brief Generator of 2D superquadric defined by blMppShapeSuperquadric2D in
/// any (x,y) layer of a 3D image
class blMppGeneratorShapeSuperquadric2DIn3D : public blMppGeneratorShapeSuperquadric2D {

public:
    /// \fn blMppGeneratorShapeSuperquadric2DIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2DIn3D();
    /// \fn blMppGeneratorShapeSuperquadric2DIn3D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2DIn3D(blImage* birthMap);
    /// \fn blMppGeneratorShapeSuperquadric2DIn3D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSuperquadric2DIn3D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeSuperquadric2DIn3D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeSuperquadric2DIn3D();

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
