/// \file blImageTypes.h
/// \brief blImageTypes class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#ifndef blIMAGETYPES_H
#define blIMAGETYPES_H

#include <itkVector.h>
#include <itkImage.h>
#include <itkImageDuplicator.h>
#include <itkCastImageFilter.h>
#include <itkPNGImageIO.h>
#include <itkTIFFImageIO.h>
#include <itkVectorCastImageFilter.h>
#include <itkVectorImage.h>

// pixels types
typedef float                           PixelFloatType; ///< PixelFloatType
typedef unsigned short int              PixelIntType; ///< PixelIntType
typedef itk::Vector<PixelIntType,3>     PixelV3IntType; ///< PixelV3IntType
typedef itk::Vector<PixelFloatType,3>   PixelV3FloatType; ///< PixelV3FloatType
typedef unsigned char                   PixelUCharType; ///< PixelUCharType
typedef itk::Vector<PixelUCharType,3>   PixelV3UCharType; ///< PixelV3UCharType
typedef itk::RGBPixel<PixelUCharType>   PixelColorUCharType; ///< PixelColorUCharType
typedef itk::RGBPixel<PixelFloatType>   PixelColorFloatType; ///< PixelColorFloatType
typedef itk::RGBPixel<PixelIntType>     PixelColorIntType; ///< PixelColorIntType

// GrayScale images Types
// 2D
typedef itk::Image<PixelUCharType,2>             UChar2DImage; /*!< 2D int image */
typedef itk::Image<PixelIntType,2>               Int2DImage; /*!< 2D int image */
typedef itk::Image<PixelFloatType,2>             Float2DImage; /*!< 2D float image */
// 3D
typedef itk::Image<PixelUCharType,3>             UChar3DImage; /*!< 3D int image */
typedef itk::Image<PixelIntType,3>               Int3DImage; /*!< 3D int image */
typedef itk::Image<PixelFloatType,3>             Float3DImage; /*!< 3D float image */

// Color Image Types
// 2D
typedef itk::Image<itk::RGBPixel<PixelUCharType>, 2> UCharColor2DImage; ///< 2D Color uChar image using using a vector per pixel
typedef itk::Image<itk::RGBPixel<PixelIntType>, 2> IntColor2DImage; ///< 2D Color int image using using a vector per pixel
typedef itk::Image<itk::RGBPixel<PixelFloatType>, 2>              FloatColor2DImage; ///< 2D Color float image using using a vector per pixel

// 3D
typedef itk::Image<itk::RGBPixel<PixelUCharType>, 3> UCharColor3DImage; /*!< 3D Color uChar image using using a vector per pixel */
typedef itk::Image<itk::RGBPixel<PixelIntType>, 3> IntColor3DImage; /*!< 3D Color int image using using a vector per pixel */
typedef itk::Image<itk::RGBPixel<PixelFloatType>, 3>              FloatColor3DImage; /*!< 3D Color float image using using a vector per pixel */

// Spectral Images
// 2D
typedef itk::VectorImage<PixelIntType, 2>           VectorInt2DImage; /*!< 2D vector int image */
typedef itk::VectorImage<PixelFloatType, 2>                        VectorFloat2DImage; /*!< 2D vector float image */


#include <itkConfigure.h>

#endif // blIMAGETYPES_H
