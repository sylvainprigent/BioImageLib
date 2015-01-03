/// \file blImage.h
/// \brief blImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include "blImageTypes.h"
#include "blImageSize.h"

/// \class blImage
/// \brief Container that store the images types
/// used in biolib
class blImage : public blData{

public:
    // images types
    static const std::string TypeInt2D;         ///< Int 2D image type
    static const std::string TypeFloat2D;       ///< Float 2D image type
    static const std::string TypeInt3D;         ///< Int 3D image type
    static const std::string TypeFloat3D;       ///< Float 3D image type
    static const std::string TypeIntColor2D;    ///< Int Color 2D image type
    static const std::string TypeFloatColor2D;  ///< Float color 2D image type
    static const std::string TypeIntColor3D;    ///< Int color 3D image type
    static const std::string TypeFloatColor3D;  ///< Float color 3D image type
    static const std::string TypeIntVector2D;   ///< Int vector 2D image type
    static const std::string TypeFloatVector2D; ///< Float vector 2D image type

public:
    // Constructors
    /// \fn blImage();
    /// \brief Constructor
    blImage();
    /// \fn blImage(std::string file);
    /// \brief Constructor
    /// \param[in] file Image file to load
    blImage(std::string file);
    /// \fn blImage( blImage* image);
    /// \brief Copy constructor
    /// \param[in] image Image to copy
    blImage( blImage* image);

    /// \fn blImage(std::string type, blImageSize size);
    /// \brief Constructor
    /// \param[in] type Image type (see types list in static const)
    /// \param[in] size Size of the image
    blImage(std::string type, blImageSize size);

    // Constructors from itk data
    /// \fn blImage(Int2DImage::Pointer itkInt2DImage);
    /// \brief Constructor
    /// \param[in] itkInt2DImage itk image to store
    blImage(Int2DImage::Pointer itkInt2DImage);

    /// \fn blImage(Float2DImage::Pointer itkFloat2DImage);
    /// \brief Constructor
    /// \param[in] itkFloat2DImage itk image to store
    blImage(Float2DImage::Pointer itkFloat2DImage);

    /// \fn blImage(Int3DImage::Pointer itkInt3DImage);
    /// \brief Constructor
    /// \param[in] itkInt3DImage itk image to store
    blImage(Int3DImage::Pointer itkInt3DImage);

    /// \fn blImage(Float3DImage::Pointer itkFloat3DImage);
    /// \brief Constructor
    /// \param[in] itkFloat3DImage itk image to store
    blImage(Float3DImage::Pointer itkFloat3DImage);

    /// \fn blImage(IntColor2DImage::Pointer itkIntColor2DImage);
    /// \brief Constructor
    /// \param[in] itkIntColor2DImage itk image to store
    blImage(IntColor2DImage::Pointer itkIntColor2DImage);

    /// \fn blImage(FloatColor2DImage::Pointer itkFloatColor2DImage);
    /// \brief Constructor
    /// \param[in] itkFloatColor2DImage itk image to store
    blImage(FloatColor2DImage::Pointer itkFloatColor2DImage);

    /// \fn blImage(IntColor3DImage::Pointer itkIntColor3DImage);
    /// \brief Constructor
    /// \param[in] itkIntColor3DImage itk image to store
    blImage(IntColor3DImage::Pointer itkIntColor3DImage);

    /// \fn blImage(FloatColor3DImage::Pointer itkFloatColor3DImage);
    /// \brief Constructor
    /// \param[in] itkFloatColor3DImage itk image to store
    blImage(FloatColor3DImage::Pointer itkFloatColor3DImage);

    /// \fn virtual ~blImage();
    /// \brief Destructor
    virtual ~blImage();

public:
    /// \fn virtual std::string name();
    /// \return the class name
    virtual std::string name();

public:
    // load
    /// \fn void load(std::string file);
    /// \brief load image from file
    /// \param[in] file Url of the image file
    void load(std::string file);

public:
    // get image info
    /// \fn std::string imageType();
    /// \return the image type
    std::string imageType();
    /// \fn blImageSize imageSize();
    /// \return the image size
    blImageSize imageSize();
    /// \fn std::string pixelType();
    /// \return the pixel type
    std::string pixelType();
    /// \fn unsigned int dimension();
    /// \return the image dimension (2D or 3D)
    unsigned int dimension();
    /// \fn bool is2D();
    /// \return True if the image is 2D, false otherwise
    bool is2D();
    /// \fn bool is3D();
    /// \return True if the image is 3D, false otherwise
    bool is3D();

public:
    // Save image
    /// \fn void save(std::string file, bool castToChar = false);
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void save(std::string file, bool castToChar = false);
    /// \fn void saveInt2DImage(std::string file, bool castToChar = false);
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveInt2DImage(std::string file, bool castToChar = false);
    /// \fn void saveFloat2DImage(std::string file, bool castToChar = false);
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveFloat2DImage(std::string file, bool castToChar = false);
    /// \fn void saveInt3DImage(std::string file, bool castToChar = false );
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveInt3DImage(std::string file, bool castToChar = false );
    /// \fn void saveFloat3DImage(std::string file, bool castToChar = false );
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveFloat3DImage(std::string file, bool castToChar = false );
    /// \fn void saveIntColor2DImage(std::string file, bool castToChar = false);
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveIntColor2DImage(std::string file, bool castToChar = false);
    /// \fn void saveFloatColor2DImage(std::string file, bool castToChar = false);
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveFloatColor2DImage(std::string file, bool castToChar = false);
    /// \fn void saveIntColor3DImage(std::string file , bool castToChar = false );
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveIntColor3DImage(std::string file , bool castToChar = false );
    /// \fn void saveFloatColor3DImage(std::string file , bool castToChar = false );
    /// \brief Save image to file
    /// \param[in] file URL
    /// \param[in] castToChar True if cast the image to char False to keep the same format
    void saveFloatColor3DImage(std::string file , bool castToChar = false );

public:
    // get itk data
    /// \fn Int2DImage::Pointer itkInt2DImagePointer();
    /// \return itk image pointer
    Int2DImage::Pointer itkInt2DImagePointer();
    /// \fn Float2DImage::Pointer itkFloat2DImagePointer();
    /// \return itk image pointer
    Float2DImage::Pointer itkFloat2DImagePointer();
    /// \fn Int3DImage::Pointer itkInt3DImagePointer();
    /// \return itk image pointer
    Int3DImage::Pointer itkInt3DImagePointer();
    /// \fn Float3DImage::Pointer itkFloat3DImagePointer();
    /// \return itk image pointer
    Float3DImage::Pointer itkFloat3DImagePointer();
    /// \fn IntColor2DImage::Pointer itkIntColor2DImagePointer();
    /// \return itk image pointer
    IntColor2DImage::Pointer itkIntColor2DImagePointer();
    /// \fn FloatColor2DImage::Pointer itkFloatColor2DImagePointer();
    /// \return itk image pointer
    FloatColor2DImage::Pointer itkFloatColor2DImagePointer();
    /// \fn IntColor3DImage::Pointer itkIntColor3DImagePointer();
    /// \return itk image pointer
    IntColor3DImage::Pointer itkIntColor3DImagePointer();
    /// \fn FloatColor3DImage::Pointer itkFloatColor3DImagePointer();
    /// \return itk image pointer
    FloatColor3DImage::Pointer itkFloatColor3DImagePointer();

public:
    // get itk data
    /// \fn void setItkImage(Int2DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(Int2DImage::Pointer itkImage);
    /// \fn void setItkImage(Float2DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(Float2DImage::Pointer itkImage);
    /// \fn void setItkImage(Int3DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(Int3DImage::Pointer itkImage);
    /// \fn void setItkImage(Float3DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(Float3DImage::Pointer itkImage);
    /// \fn void setItkImage(IntColor2DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(IntColor2DImage::Pointer itkImage);
    /// \fn void setItkImage(FloatColor2DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(FloatColor2DImage::Pointer itkImage);
    /// \fn void setItkImage(IntColor3DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(IntColor3DImage::Pointer itkImage);
    /// \fn void setItkImage(FloatColor3DImage::Pointer itkImage);
    /// \param[in] itkImage itk image pointer to store
    void setItkImage(FloatColor3DImage::Pointer itkImage);

public:
    // pixels access
    /// \fn int xSize();
    /// \brief getter
    /// \return the image size along X
    int xSize();
    /// \fn int ySize();
    /// \brief getter
    /// \return the image size along Y
    int ySize();
    /// \fn int zSize();
    /// \brief getter
    /// \return the image size along Z
    int zSize();
    /// \fn int getPixelInt(unsigned int i, unsigned int j);
    /// \brief get a pixel value
    /// \param[in] i coordinate along X
    /// \param[in] j coordinate along Y
    /// \return the pixel value
    int getPixelInt(unsigned int i, unsigned int j);

    /// \fn void setPixelInt(unsigned int i, unsigned int j, int value);
    /// \brief set a pixel value
    /// \param[in] i coordinate along X
    /// \param[in] j coordinate along Y
    /// \param[in] value Value to set
    void setPixelInt(unsigned int i, unsigned int j, int value);
    /// \fn int getPixelInt(unsigned int i, unsigned int j, unsigned int z);
    /// \brief get a pixel value
    /// \param[in] i coordinate along X
    /// \param[in] j coordinate along Y
    /// \param[in] z coordinate along Z
    /// \return the pixel value
    int getPixelInt(unsigned int i, unsigned int j, unsigned int z);


protected:
    // image info
    std::string m_imageType;    ///< type of the image (see types in static const attributs)
    blImageSize m_imageSize;    ///< size of the image
    std::string m_pixelType;    ///< pixel types
    unsigned int m_dimension;   ///< image dimension (2D or 3D)

protected:
    // images data
    Int2DImage::Pointer m_itkInt2DImage;               ///< storage itk image pointer
    Float2DImage::Pointer m_itkFloat2DImage;           ///< storage itk image pointer
    Int3DImage::Pointer m_itkInt3DImage;               ///< storage itk image pointer
    Float3DImage::Pointer m_itkFloat3DImage;           ///< storage itk image pointer
    IntColor2DImage::Pointer m_itkIntColor2DImage;     ///< storage itk image pointer
    FloatColor2DImage::Pointer m_itkFloatColor2DImage; ///< storage itk image pointer
    IntColor3DImage::Pointer m_itkIntColor3DImage;     ///< storage itk image pointer
    FloatColor3DImage::Pointer m_itkFloatColor3DImage; ///< storage itk image pointer

private:
    /// \fn void initPointers();
    /// \brief Initialize the storage pointer
    void initPointers();
};
