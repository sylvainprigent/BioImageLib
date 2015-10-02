/// \file blImageIO.cpp
/// \brief blImageIO class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageIO.h"
#include "blException.h"

#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>

#include <itkPNGImageIO.h>
#include <itkTIFFImageIO.h>

using namespace std;

// /////////////////////////////////////////////////////////////////////////
//                            blImageIO
// /////////////////////////////////////////////////////////////////////////
Int2DImage::Pointer blImageIO::loadToInt2DImage(const string filePath){

    typedef itk::ImageFileReader<Int2DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveInt2DImage(const string filePath, const Int2DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<Int2DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<Int2DImage,UChar2DImage > CasterType;

			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UChar2DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UChar2DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }

    }
}

Int3DImage::Pointer blImageIO::loadToInt3DImage(const std::string filePath){
    typedef itk::ImageFileReader<Int3DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveInt3DImage(const string filePath, const Int3DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<Int3DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<Int3DImage,UChar3DImage > CasterType;
      
			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UChar3DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UChar3DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}

Float2DImage::Pointer blImageIO::loadToFloat2DImage(const string filePath){
    typedef itk::ImageFileReader<Float2DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveFloat2DImage(const string filePath, const Float2DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<Float2DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<Float2DImage,UChar2DImage > CasterType;

			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UChar2DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UChar2DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}

FloatColor2DImage::Pointer blImageIO::loadToFloatColor2DImage(const string filePath){
    typedef itk::ImageFileReader<FloatColor2DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveFloatColor2DImage(const string filePath, const FloatColor2DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<FloatColor2DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<FloatColor2DImage,UCharColor2DImage > CasterType;
          
			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UCharColor2DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UCharColor2DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}

IntColor2DImage::Pointer blImageIO::loadToIntColor2DImage(const string filePath){
    typedef itk::ImageFileReader<IntColor2DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveIntColor2DImage(const string filePath, const IntColor2DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<IntColor2DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<IntColor2DImage,UCharColor2DImage > CasterType;

			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UCharColor2DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UCharColor2DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}

Float3DImage::Pointer blImageIO::loadToFloat3DImage(const std::vector< std::string > filePaths){

    itk::ImageSeriesReader<Float3DImage >::Pointer
            reader = itk::ImageSeriesReader<Float3DImage >::New();

    // Get extension of the first image
    std::string firstImage = filePaths.at(0);
    size_t n;

    n = firstImage.find(".png");
    if (n != std::string::npos )
        reader->SetImageIO( itk::PNGImageIO::New() );
    n = firstImage.find(".tif");
    if (n != std::string::npos )
        reader->SetImageIO( itk::TIFFImageIO::New() );

    try{
        reader->SetFileNames(filePaths);
        reader->Update();
        return reader->GetOutput();
    }
    catch( itk::ExceptionObject &err)
    {
        throw blException(err.GetDescription());
    }

}

Float3DImage::Pointer blImageIO::loadToFloat3DImage(const std::string filePath){
    typedef itk::ImageFileReader<Float3DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveFloat3DImage(const string filePath, const Float3DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<Float3DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<Float3DImage,UChar3DImage > CasterType;
           
			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UChar3DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UChar3DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}

IntColor3DImage::Pointer blImageIO::loadToIntColor3DImage(const string filePath){
    typedef itk::ImageFileReader<IntColor3DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveIntColor3DImage(const string filePath, const IntColor3DImage::Pointer image, bool castTochar){
    if (!castTochar){
        typedef itk::ImageFileWriter<IntColor3DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<IntColor3DImage,UCharColor3DImage > CasterType;
            
			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UCharColor3DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UCharColor3DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
}

FloatColor3DImage::Pointer blImageIO::loadToFloatColor3DImage(const string filePath){
    typedef itk::ImageFileReader<FloatColor3DImage> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    try {
        reader->SetFileName(filePath);
        reader->Update();
        return reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err){
        throw blException(err.GetDescription());
    }
}

void blImageIO::saveFloatColor3DImage(const string filePath, const FloatColor3DImage::Pointer image, bool castTochar){

    if (!castTochar){
        typedef itk::ImageFileWriter<FloatColor3DImage> WriterType;
        WriterType::Pointer writer = WriterType::New();
        try
        {
            writer->SetFileName(filePath);
            writer->SetInput(image);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }
    else{

        try
        {
            // cast
            typedef itk::CastImageFilter<FloatColor3DImage,UCharColor3DImage > CasterType;
           
			CasterType::Pointer caster = CasterType::New();
            caster->SetInput( image );
            caster->Update();
            UCharColor3DImage::Pointer castedImage = caster->GetOutput();

            // save
            typedef itk::ImageFileWriter<UCharColor3DImage> WriterType;
            WriterType::Pointer writer = WriterType::New();

            writer->SetFileName(filePath);
            writer->SetInput(castedImage);
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            throw blException(err.GetDescription());
        }
    }

}
