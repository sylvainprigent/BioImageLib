/// \file blImageCreate.h
/// \brief blImageCreate class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blImageTypes.h"
#include "blCoreExport.h"

/// \class blImageCreate
/// \brief class that define static function to create images of various types
class BLCORE_EXPORT blImageCreate{
public:
    /// \fn static Int2DImage::Pointer createNewInt2DImage(int nl, int nc);
    /// \brief Function that create a 2D Int image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    static Int2DImage::Pointer createNewInt2DImage(int nl, int nc);

    /// \fn static Float2DImage::Pointer createNewFloat2DImage(int nl, int nc);
    /// \brief Function that create a 2D Float image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    static Float2DImage::Pointer createNewFloat2DImage(int nl, int nc);

    /// \fn static Int3DImage::Pointer createNewInt3DImage(int nl, int nc, int nz);
    /// \brief Function that create a 3D Int image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    /// \param[in] nz Number of z
    static Int3DImage::Pointer createNewInt3DImage(int nl, int nc, int nz);

    /// \fn static Float3DImage::Pointer createNewFloat3DImage(int nl, int nc, int nz);
    /// \brief Function that create a 3D Float image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    /// \param[in] nz Number of z
    static Float3DImage::Pointer createNewFloat3DImage(int nl, int nc, int nz);

    /// \fn static IntColor2DImage::Pointer createNewIntColor2DImage(int nl, int nc);
    /// \brief Function that create a 2D Color Int image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    static IntColor2DImage::Pointer createNewIntColor2DImage(int nl, int nc);

    /// \fn static FloatColor2DImage::Pointer createNewFloatColor2DImage(int nl, int nc);
    /// \brief Function that create a 2D ColorV Float image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    static FloatColor2DImage::Pointer createNewFloatColor2DImage(int nl, int nc);

    /// \fn static IntColor3DImage::Pointer createNewIntColor3DImage(int nl, int nc, int nz);
    /// \brief Function that create a 3D Color Int image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    /// \param[in] nz z dimension
    static IntColor3DImage::Pointer createNewIntColor3DImage(int nl, int nc, int nz);

    /// \fn static FloatColor3DImage::Pointer createNewFloatColor3DImage(int nl, int nc, int nz);
    /// \brief Function that create a 3D Color Float image
    /// \param[in] nl Number of rows
    /// \param[in] nc Number of columns
    /// \param[in] nz z dimension
    static FloatColor3DImage::Pointer createNewFloatColor3DImage(int nl, int nc, int nz);
};
