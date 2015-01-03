/// \file blMppResultRepresentation.h
/// \brief blMppResultRepresentation class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blMppShape.h"

/// \class blMppResultRepresentation
/// \brief Static function that allows to plot the blMpp detection result into an image
class blMppResultRepresentation{

public:
    static blImage *imageRepresentation(std::vector<blMppShape*> shapes, std::string representationType, blImage* image, bool useRandColor = false);
    static void saveShapeListToFile(std::vector<blMppShape*> shapes, std::string file);
    static void saveShapeListToFileInsidePixels(std::vector<blMppShape*> shapes);

private:
    static blImage *imageRepresentation2D(std::vector<blMppShape*> shapes, std::string representationType, blImage* image, bool useRandColor = false);
    static blImage *imageRepresentation3D(std::vector<blMppShape*> shapes, std::string representationType, blImage* image, bool useRandColor = false);
};
