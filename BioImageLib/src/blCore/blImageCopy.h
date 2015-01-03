/// \file blImageCopy.h
/// \brief blImageCopy class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blImageTypes.h"

/// \class blImageCopy
/// \brief class that define static functions to copy different types of images
class blImageCopy{
public:
    /// \fn static Int2DImage::Pointer copyInt2DImage(Int2DImage::Pointer image);
    /// \brief Function that copy a Int2DImage image into another
    static Int2DImage::Pointer copyInt2DImage(Int2DImage::Pointer image);

    /// \fn static Float2DImage::Pointer copyFloat2DImage(Float2DImage::Pointer image);
	/// \brief Function that copy a Float2DImage image into another
    static Float2DImage::Pointer copyFloat2DImage(Float2DImage::Pointer image);

    /// \fn static Int3DImage::Pointer copyInt3DImage(Int3DImage::Pointer image);
    /// \brief Function that copy a Int3DImage image into another
    static Int3DImage::Pointer copyInt3DImage(Int3DImage::Pointer image);

    /// \fn static Float3DImage::Pointer copyFloat3DImage(Float3DImage::Pointer image);
	/// \brief Function that copy a Float3DImage image into another
    static Float3DImage::Pointer copyFloat3DImage(Float3DImage::Pointer image);


    /// \fn static IntColor2DImage::Pointer copyIntColor2DImage(IntColor2DImage::Pointer image);
    /// \brief Function that copy a IntColor2DImage image into another
    static IntColor2DImage::Pointer copyIntColor2DImage(IntColor2DImage::Pointer image);

    /// \fn static FloatColor2DImage::Pointer copyFloatColor2DImage(FloatColor2DImage::Pointer image);
    /// \brief Function that copy a FloatColor2DImage image into another
    static FloatColor2DImage::Pointer copyFloatColor2DImage(FloatColor2DImage::Pointer image);

    /// \fn static IntColor3DImage::Pointer copyIntColor3DImage(IntColor3DImage::Pointer image);
    /// \brief Function that copy a IntColor3DImage image into another
    static IntColor3DImage::Pointer copyIntColor3DImage(IntColor3DImage::Pointer image);

    /// \fn static FloatColor3DImage::Pointer copyFloatColor3DImage(FloatColor3DImage::Pointer image);
    /// \brief Function that copy a FloatColor3DImage image into another
    static FloatColor3DImage::Pointer copyFloatColor3DImage(FloatColor3DImage::Pointer image);
};
