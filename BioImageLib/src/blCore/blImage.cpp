/// \file blImage.cpp
/// \brief blImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImage.h"
#include "blImageIOInfo.h"
#include "blImageIO.h"
#include "blImageCreate.h"
#include "blImageCopy.h"
#include "blException.h"

#include <iostream>

// images types
const std::string blImage::TypeInt2D = "Int2DImage";
const std::string blImage::TypeFloat2D = "Float2DImage";
const std::string blImage::TypeInt3D = "Int3DImage";
const std::string blImage::TypeFloat3D = "Float3DImage";
const std::string blImage::TypeIntColor2D = "IntColor2DImage";
const std::string blImage::TypeFloatColor2D = "FloatColor2DImage";
const std::string blImage::TypeIntColor3D = "IntColor3DImage";
const std::string blImage::TypeFloatColor3D = "FloatColor3DImage";
const std::string blImage::TypeIntVector2D = "IntVector2DImage";
const std::string blImage::TypeFloatVector2D = "FloatVector2DImage";

// Constructors
void blImage::initPointers(){
    m_itkInt2DImage = nullptr;
    m_itkFloat2DImage = nullptr;
    m_itkInt3DImage = nullptr;
    m_itkFloat3DImage = nullptr;
    m_itkIntColor2DImage = nullptr;
    m_itkFloatColor2DImage = nullptr;
    m_itkIntColor3DImage = nullptr;
    m_itkFloatColor3DImage = nullptr;
}

blImage::blImage() : blData(){
    initPointers();
}

blImage::blImage(std::string file) : blData(){
    initPointers();
    load(file);
}

blImage::blImage(std::string type, blImageSize size) : blData(){
    initPointers();

    if (type == blImage::TypeInt2D){
        setItkImage(blImageCreate::createNewInt2DImage(size.xSize(), size.ySize()));
    }
    else if (type == blImage::TypeFloat2D){
        setItkImage(blImageCreate::createNewFloat2DImage(size.xSize(), size.ySize()));
    }
    else if (type == blImage::TypeInt3D){
        setItkImage(blImageCreate::createNewInt3DImage(size.xSize(), size.ySize(),size.zSize()));
    }
    else if (type == blImage::TypeFloat3D){
        setItkImage(blImageCreate::createNewFloat3DImage(size.xSize(), size.ySize(),size.zSize()));
    }
    else if (type == blImage::TypeIntColor2D){
        setItkImage(blImageCreate::createNewIntColor2DImage(size.xSize(), size.ySize()));
    }
    else if (type == blImage::TypeFloatColor2D){
        setItkImage(blImageCreate::createNewFloatColor2DImage(size.xSize(), size.ySize()));
    }
    else if (type == blImage::TypeIntColor3D){
        setItkImage(blImageCreate::createNewIntColor3DImage(size.xSize(), size.ySize(), size.zSize()));
    }
    else if (type == blImage::TypeFloatColor3D){
        setItkImage(blImageCreate::createNewFloatColor3DImage(size.xSize(), size.ySize(), size.zSize()));
    }
}

blImage::blImage( blImage* image){
    initPointers();

    if (image->imageType() == blImage::TypeInt2D){
        setItkImage(blImageCopy::copyInt2DImage(image->itkInt2DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeFloat2D){
        setItkImage(blImageCopy::copyFloat2DImage(image->itkFloat2DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeInt3D){
        setItkImage(blImageCopy::copyInt3DImage(image->itkInt3DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeFloat3D){
        setItkImage(blImageCopy::copyFloat3DImage(image->itkFloat3DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeIntColor2D){
        setItkImage(blImageCopy::copyIntColor2DImage(image->itkIntColor2DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeFloatColor2D){
        setItkImage(blImageCopy::copyFloatColor2DImage(image->itkFloatColor2DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeIntColor3D){
        setItkImage(blImageCopy::copyIntColor3DImage(image->itkIntColor3DImagePointer()));
    }
    else if (image->imageType() == blImage::TypeFloatColor3D){
        setItkImage(blImageCopy::copyFloatColor3DImage(image->itkFloatColor3DImagePointer()));
    }
}

// Constructors from itk data
blImage::blImage(Int2DImage::Pointer itkInt2DImage) : blData(){
    setItkImage(itkInt2DImage);
}

blImage::blImage(Float2DImage::Pointer itkFloat2DImage) : blData(){
    setItkImage(itkFloat2DImage);
}

blImage::blImage(Int3DImage::Pointer itkInt3DImage) : blData(){
    setItkImage(itkInt3DImage);
}

blImage::blImage(Float3DImage::Pointer itkFloat3DImage) : blData(){
    setItkImage(itkFloat3DImage);
}

blImage::blImage(IntColor2DImage::Pointer itkIntColor2DImage) : blData(){
    setItkImage(itkIntColor2DImage);
}

blImage::blImage(FloatColor2DImage::Pointer itkFloatColor2DImage) : blData(){
    setItkImage(itkFloatColor2DImage);
}

blImage::blImage(IntColor3DImage::Pointer itkIntColor3DImage) : blData(){
    setItkImage(itkIntColor3DImage);
}

blImage::blImage(FloatColor3DImage::Pointer itkFloatColor3DImage) : blData(){
    setItkImage(itkFloatColor3DImage);
}

blImage::~blImage(){
    initPointers();
}

std::string blImage::name(){
    return "blImage";
}

void blImage::load(std::string file){

    blImageIOInfo imageInfo;
    imageInfo.updateInfo(file);
    std::string pixelType = imageInfo.pixelType();
    int dim = imageInfo.dimension();
    int colorNum = imageInfo.colorDimension();

    /// \todo fix this format problem
    if (colorNum == 3){colorNum = 1;}

    //std::cout << "blImage::load pixelType = " << pixelType << std::endl;
    //std::cout << "blImage::load colorNum = " << colorNum << std::endl;
    //std::cout << "blImage::load dim = " << dim << std::endl;

    if (colorNum ==1 && dim == 2 && (pixelType == "unsigned_char" || pixelType == "char" || pixelType == "short" || pixelType == "unsigned_short") ){
        m_dimension = 2;
        m_imageType = blImage::TypeInt2D;
        m_pixelType = "int";
        m_itkInt2DImage = blImageIO::loadToInt2DImage(file);
        int xSize = m_itkInt2DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkInt2DImage->GetLargestPossibleRegion().GetSize()[1];
        int colorSize = 1;
        m_imageSize.setSize(xSize, ySize, colorSize);
    }
    else if (colorNum ==1 && dim == 2 && (pixelType == "float")){
        m_dimension = 2;
        m_imageType = blImage::TypeFloat2D;
        m_pixelType = "float";
        m_itkFloat2DImage = blImageIO::loadToFloat2DImage(file);
        int xSize = m_itkFloat2DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkFloat2DImage->GetLargestPossibleRegion().GetSize()[1];
        int colorSize = 1;
        m_imageSize.setSize(xSize, ySize, colorSize);
    }
    else if (colorNum ==1 && dim == 3 && (pixelType == "unsigned_char" || pixelType == "char" || pixelType == "short" || pixelType == "unsigned_short")){
        m_dimension = 3;
        m_imageType = blImage::TypeInt3D;
        m_pixelType = "int";
        m_itkInt3DImage = blImageIO::loadToInt3DImage(file);
        int xSize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[1];
        int zSize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[2];
        int colorSize = 1;
        m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    }
    else if (colorNum ==1 && dim == 3 && pixelType == "float"){
        m_dimension = 3;
        m_imageType = blImage::TypeFloat3D;
        m_pixelType = "float";
        m_itkFloat3DImage = blImageIO::loadToFloat3DImage(file);
        int xSize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[1];
        int zSize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[2];
        int colorSize = 1;
        m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    }
    // Color images
    else if (colorNum == 2 && dim == 2 && (pixelType == "unsigned_char" || pixelType == "char" || pixelType == "short" || pixelType == "unsigned_short")){
        m_dimension = 2;
        m_imageType = blImage::TypeIntColor2D;
        m_pixelType = "int";
        m_itkIntColor2DImage = blImageIO::loadToIntColor2DImage(file);
        int xSize = m_itkIntColor2DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkIntColor2DImage->GetLargestPossibleRegion().GetSize()[1];
        int colorSize = 3;
        m_imageSize.setSize(xSize, ySize, colorSize);
    }
    else if (colorNum == 2 && dim == 2 && (pixelType == "float")){
        m_dimension = 2;
        m_imageType = blImage::TypeFloatColor2D;
        m_pixelType = "float";
        m_itkFloatColor2DImage = blImageIO::loadToFloatColor2DImage(file);
        int xSize = m_itkFloatColor2DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkFloatColor2DImage->GetLargestPossibleRegion().GetSize()[1];
        int colorSize = 3;
        m_imageSize.setSize(xSize, ySize, colorSize);
    }
    else if (colorNum == 2 && dim == 3 && (pixelType == "unsigned_char" || pixelType == "char" || pixelType == "short" || pixelType == "unsigned_short")){
        m_dimension = 3;
        m_imageType = blImage::TypeIntColor3D;
        m_pixelType = "int";
        m_itkIntColor3DImage = blImageIO::loadToIntColor3DImage(file);
        int xSize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[1];
        int zSize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[2];
        int colorSize = 3;
        m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    }
    else if (colorNum == 2 && dim == 3 && (pixelType == "float")){
        m_dimension = 3;
        m_imageType = blImage::TypeFloatColor3D;
        m_pixelType = "float";
        m_itkFloatColor3DImage = blImageIO::loadToFloatColor3DImage(file);
        int xSize = m_itkFloatColor3DImage->GetLargestPossibleRegion().GetSize()[0];
        int ySize = m_itkFloatColor3DImage->GetLargestPossibleRegion().GetSize()[1];
        int zSize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[2];
        int colorSize = 3;
        m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    }
    else{
        throw( blException("blImage: Cannot load this image format"));
    }
}

// get image info
std::string blImage::imageType(){
    return m_imageType;
}

blImageSize blImage::imageSize(){
    return m_imageSize;
}

std::string blImage::pixelType(){
    return m_pixelType;
}

unsigned int blImage::dimension(){
    return m_dimension;
}

bool blImage::is2D(){
    if (m_imageType == TypeInt2D
        || m_imageType == TypeFloat2D
        || m_imageType == TypeIntColor2D
        || m_imageType == TypeFloatColor2D
        || m_imageType == TypeIntVector2D
        || m_imageType == TypeFloatVector2D ){
        return true;
    }
    return false;
}

bool blImage::is3D(){
    if (m_imageType == TypeInt3D
        || m_imageType == TypeFloat3D
        || m_imageType == TypeIntColor3D
        || m_imageType == TypeFloatColor3D ){
        return true;
    }
    return false;
}

// save
void blImage::save(std::string file, bool castToChar){
    //std::cout << "blImage -> save image, type = " << m_imageType << std::endl;

    if (m_imageType == blImage::TypeInt2D){
        saveInt2DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeFloat2D){
        saveFloat2DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeInt3D){
        saveInt3DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeFloat3D){
        saveFloat3DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeIntColor2D){
        saveIntColor2DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeFloatColor2D){
        saveFloatColor2DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeIntColor3D){
        saveIntColor3DImage(file, castToChar);
    }
    else if (m_imageType == blImage::TypeFloatColor3D){
        saveFloatColor3DImage(file, castToChar);
    }
}

void blImage::saveInt2DImage(std::string file, bool castToChar){
    blImageIO::saveInt2DImage(file, m_itkInt2DImage, castToChar);
}

void blImage::saveFloat2DImage(std::string file, bool castToChar){
    blImageIO::saveFloat2DImage(file, m_itkFloat2DImage, castToChar);
}

void blImage::saveInt3DImage(std::string file, bool castToChar){
    blImageIO::saveInt3DImage(file, m_itkInt3DImage, castToChar);
}

void blImage::saveFloat3DImage(std::string file, bool castToChar){
    blImageIO::saveFloat3DImage(file, m_itkFloat3DImage, castToChar);
}

void blImage::saveIntColor2DImage(std::string file, bool castToChar){
    blImageIO::saveIntColor2DImage(file, m_itkIntColor2DImage, castToChar);
}

void blImage::saveFloatColor2DImage(std::string file, bool castToChar){
    blImageIO::saveFloatColor2DImage(file, m_itkFloatColor2DImage, castToChar);
}

void blImage::saveIntColor3DImage(std::string file, bool castToChar){
    blImageIO::saveIntColor3DImage(file, m_itkIntColor3DImage, castToChar);
}

void blImage::saveFloatColor3DImage(std::string file, bool castToChar){
    blImageIO::saveFloatColor3DImage(file, m_itkFloatColor3DImage, castToChar);
}

// get itk data
Int2DImage::Pointer blImage::itkInt2DImagePointer(){
    return m_itkInt2DImage;
}

Float2DImage::Pointer blImage::itkFloat2DImagePointer(){
    return m_itkFloat2DImage;
}

Int3DImage::Pointer blImage::itkInt3DImagePointer(){
    return m_itkInt3DImage;
}

Float3DImage::Pointer blImage::itkFloat3DImagePointer(){
    return m_itkFloat3DImage;
}

IntColor2DImage::Pointer blImage::itkIntColor2DImagePointer(){
    return m_itkIntColor2DImage;
}

FloatColor2DImage::Pointer blImage::itkFloatColor2DImagePointer(){
    return m_itkFloatColor2DImage;
}

IntColor3DImage::Pointer blImage::itkIntColor3DImagePointer(){
    return m_itkIntColor3DImage;
}

FloatColor3DImage::Pointer blImage::itkFloatColor3DImagePointer(){
    return m_itkFloatColor3DImage;
}


// set itk data
void blImage::setItkImage(Int2DImage::Pointer itkImage){
    initPointers();
    m_itkInt2DImage = itkImage;
    int xSize = m_itkInt2DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkInt2DImage->GetLargestPossibleRegion().GetSize()[1];
    int colorSize = 1;
    m_imageSize.setSize(xSize, ySize, colorSize);
    m_pixelType = "int";
    m_imageType = blImage::TypeInt2D;
    m_dimension = 2;
}

void blImage::setItkImage(Float2DImage::Pointer itkImage){
    initPointers();
    m_itkFloat2DImage = itkImage;
    int xSize = m_itkFloat2DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkFloat2DImage->GetLargestPossibleRegion().GetSize()[1];
    int colorSize = 1;
    m_imageSize.setSize(xSize, ySize, colorSize);
    m_pixelType = "float";
    m_imageType = blImage::TypeFloat2D;
    m_dimension = 2;
}

void blImage::setItkImage(Int3DImage::Pointer itkImage){
    initPointers();
    m_itkInt3DImage = itkImage;
    int xSize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[1];
    int zSize = m_itkInt3DImage->GetLargestPossibleRegion().GetSize()[2];
    int colorSize = 1;
    m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    m_pixelType = "int";
    m_imageType = blImage::TypeInt3D;
    m_dimension = 3;
}

void blImage::setItkImage(Float3DImage::Pointer itkImage){
    initPointers();
    m_itkFloat3DImage = itkImage;
    int xSize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[1];
    int zSize = m_itkFloat3DImage->GetLargestPossibleRegion().GetSize()[2];
    int colorSize = 1;
    m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    m_pixelType = "float";
    m_imageType = blImage::TypeFloat3D;
    m_dimension = 3;
}

void blImage::setItkImage(IntColor2DImage::Pointer itkImage){
    initPointers();
    m_itkIntColor2DImage = itkImage;
    int xSize = m_itkIntColor2DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkIntColor2DImage->GetLargestPossibleRegion().GetSize()[1];
    int colorSize = 3;
    m_imageSize.setSize(xSize, ySize, colorSize);
    m_pixelType = "int";
    m_imageType = blImage::TypeIntColor2D;
}

void blImage::setItkImage(FloatColor2DImage::Pointer itkImage){
    initPointers();
    m_itkFloatColor2DImage = itkImage;
    int xSize = m_itkFloatColor2DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkFloatColor2DImage->GetLargestPossibleRegion().GetSize()[1];
    int colorSize = 3;
    m_imageSize.setSize(xSize, ySize, colorSize);
    m_pixelType = "float";
    m_imageType = blImage::TypeFloatColor2D;
    m_dimension = 2;
}

void blImage::setItkImage(IntColor3DImage::Pointer itkImage){
    initPointers();
    m_itkIntColor3DImage = itkImage;
    int xSize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[1];
    int zSize = m_itkIntColor3DImage->GetLargestPossibleRegion().GetSize()[2];
    int colorSize = 3;
    m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    m_pixelType = "int";
    m_imageType = blImage::TypeIntColor3D;
    m_dimension = 3;
}

void blImage::setItkImage(FloatColor3DImage::Pointer itkImage){
    initPointers();
    m_itkFloatColor3DImage = itkImage;
    int xSize = m_itkFloatColor3DImage->GetLargestPossibleRegion().GetSize()[0];
    int ySize = m_itkFloatColor3DImage->GetLargestPossibleRegion().GetSize()[1];
    int zSize = m_itkFloatColor3DImage->GetLargestPossibleRegion().GetSize()[2];
    int colorSize = 3;
    m_imageSize.setSize(xSize, ySize, zSize, colorSize);
    m_pixelType = "int";
    m_imageType = blImage::TypeFloatColor3D;
    m_dimension = 3;
}

// pixels access
int blImage::getPixelInt(unsigned int i, unsigned int j){

    Int2DImage::IndexType index;
    index[0] = i;
    index[1] = j;
    return m_itkInt2DImage->GetPixel(index);
}

void blImage::setPixelInt(unsigned int i, unsigned int j, int value){
    Int2DImage::IndexType index;
    index[0] = i;
    index[1] = j;
    m_itkInt2DImage->SetPixel(index, value);
}

int blImage::getPixelInt(unsigned int i, unsigned int j, unsigned int z){

    Int3DImage::IndexType index;
    index[0] = i;
    index[1] = j;
    index[2] = z;
    return m_itkInt3DImage->GetPixel(index);
}

int blImage::xSize(){
    return m_imageSize.xSize();
}

int blImage::ySize(){
    return m_imageSize.ySize();
}

int blImage::zSize(){
    return m_imageSize.zSize();
}
