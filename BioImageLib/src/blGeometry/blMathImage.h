/// \file blMathImage.h
/// \brief blMathImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blGeometryExport.h"

/// \class blMathImage
/// \brief Calculate pixels values for geometrical
/// shapes located in an image
class BLGEOMETRY_EXPORT blMathImage
{
public:
    /// \fn static vector<float> CalculatePixelsValuesInsideSphere(vector<float> sphere, Float3DImage::Pointer image);
    /// \brief Function that calculates the coordinates of points defining a sphere
    /// \param[in] sphere vector containing the sphere parameters
    /// \param[in] image image to get the values
    /// \return vector contining the pixels values
  static std::vector<float> CalculatePixelsValuesInsideSphere(const std::vector<float>& sphere, const Float3DImage::Pointer& image);

  static std::vector<float> CalculatePixelsValuesInsideDiskIn3D(const std::vector<float>& objects, const Float3DImage::Pointer& image);
};
