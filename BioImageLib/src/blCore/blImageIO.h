/// \file blImageIO.h
/// \brief blImageIO class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blImageTypes.h"

/// \class blImageIO
/// \brief class containing static function to load/write various type of images
class blImageIO{

public:
    /// \fn static Int2DImage::Pointer loadToInt2DImage(const std::string filePath);
    /// \brief Function that load a Int2DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a Char2DImage object
    static Int2DImage::Pointer loadToInt2DImage(const std::string filePath);
    /// \fn static void saveInt2DImage(const std::string filePath, const Int2DImage::Pointer image);
    /// \brief Function that save a Int2DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveInt2DImage(const std::string filePath, const Int2DImage::Pointer image, bool castTochar = false);

    /// \fn static Int3DImage::Pointer loadToInt3DImage(const std::string filePath);
    /// \brief Function that load a Char3DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a Char2DImage object
    static Int3DImage::Pointer loadToInt3DImage(const std::string filePath);
    /// \fn static void saveInt3DImage(const std::string filePath, const Int3DImage::Pointer image);
    /// \brief Function that save a Int3DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveInt3DImage(const std::string filePath, const Int3DImage::Pointer image, bool castTochar = false);


    /// \fn static Float2DImage::Pointer loadToFloat2DImage(const std::string filePath);
    /// \brief Function that load a Float2DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a Float2DImage object
    static Float2DImage::Pointer loadToFloat2DImage(const std::string filePath);
    /// \fn static void saveFloat2DImage(const std::string filePath, const Char2DImage::Pointer image);
    /// \brief Function that save a Float2DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveFloat2DImage(const std::string filePath, const Float2DImage::Pointer image, bool castTochar = true);

    /// \fn static FloatColor2DImage::Pointer loadToFloatColor2DImage(const std::string filePath);
    /// \brief Function that load a FloatColor2DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a FloatColor2DImage object
    static FloatColor2DImage::Pointer loadToFloatColor2DImage(const std::string filePath);
    /// \fn static void saveFloatColor2DImage(const std::string filePath, const Char2DImage::Pointer image);
    /// \brief Function that save a FloatColor2DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveFloatColor2DImage(const std::string filePath, const FloatColor2DImage::Pointer image, bool castTochar = false);


    /// \fn static IntColor2DImage::Pointer loadToIntColor2DImage(const std::string filePath);
    /// \brief Function that load a IntColor2DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a IntColor2DImage object
    static IntColor2DImage::Pointer loadToIntColor2DImage(const std::string filePath);
    /// \fn static void saveIntColor2DImage(const std::string filePath, const IntColor2DImage::Pointer image);
    /// \brief Function that save a FloatColorV2DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveIntColor2DImage(const std::string filePath, const IntColor2DImage::Pointer image, bool castTochar = false);

    /// \fn static Float3DImage::Pointer loadToFloat3DImage(const std::vector<std::string> filePaths);
    /// \brief Function that load a Float3DImage
    /// \param[in] filePaths path of the 2D image file series
    /// \return Pointer to the loaded image in a Float3DImage object
    static Float3DImage::Pointer loadToFloat3DImage(const std::vector<std::string> filePaths);

    /// \fn static Float3DImage::Pointer loadToFloat3DImage(const std::string filePath);
    /// \brief Function that load a Float3DImage
    /// \param[in] filePath path of the 2D image file series
    /// \return Pointer to the loaded image in a Float3DImage object
    static Float3DImage::Pointer loadToFloat3DImage(const std::string filePath);

    /// \fn static void saveFloat3DImage(const std::string filePath, const Float3DImage::Pointer image);
    /// \brief Function that load a Float3DImage
    /// \param[in] filePath path of the 3D image file series
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveFloat3DImage(const std::string filePath, const Float3DImage::Pointer image, bool castTochar = false);


    /// \fn static IntColor3DImage::Pointer loadToIntColor3DImage(const std::string filePath);
    /// \brief Function that load a IntColor3DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a FloatColor3DImage object
    static IntColor3DImage::Pointer loadToIntColor3DImage(const std::string filePath);
    /// \fn static void saveIntColor3DImage(const std::string filePath, const IntColor3DImage::Pointer image);
    /// \brief Function that save a IntColor3DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveIntColor3DImage(const std::string filePath, const IntColor3DImage::Pointer image, bool castTochar = false);



    /// \fn static FloatColor3DImage::Pointer loadToFloatColor3DImage(const std::string filePath);
    /// \brief Function that load a FloatColor3DImage
    /// \param[in] filePath path of the image file
    /// \return Pointer to the loaded image in a FloatColor3DImage object
    static FloatColor3DImage::Pointer loadToFloatColor3DImage(const std::string filePath);
    /// \fn static void saveFloatColor3DImage(const std::string filePath, const FloatColor3DImage::Pointer image);
    /// \brief Function that save a FloatColor3DImage
    /// \param[in] filePath Path where to save the file
    /// \param[in] image Pointer to the image to save
    /// \param[in] castTochar Set to true to cast the image to char
    static void saveFloatColor3DImage(const std::string filePath, const FloatColor3DImage::Pointer image, bool castTochar = false);

};
