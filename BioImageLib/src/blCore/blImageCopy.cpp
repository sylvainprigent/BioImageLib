/// \file blImageCopy.cpp
/// \brief blImageCopy class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageCopy.h"

Int2DImage::Pointer blImageCopy::copyInt2DImage(Int2DImage::Pointer image){

    typedef itk::ImageDuplicator< Int2DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

Float2DImage::Pointer blImageCopy::copyFloat2DImage(Float2DImage::Pointer image){
    typedef itk::ImageDuplicator< Float2DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

Int3DImage::Pointer blImageCopy::copyInt3DImage(Int3DImage::Pointer image){
    typedef itk::ImageDuplicator< Int3DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

Float3DImage::Pointer blImageCopy::copyFloat3DImage(Float3DImage::Pointer image){
    typedef itk::ImageDuplicator< Float3DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}



IntColor2DImage::Pointer blImageCopy::copyIntColor2DImage(IntColor2DImage::Pointer image){
    typedef itk::ImageDuplicator< IntColor2DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

FloatColor2DImage::Pointer blImageCopy::copyFloatColor2DImage(FloatColor2DImage::Pointer image){
    typedef itk::ImageDuplicator< FloatColor2DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

IntColor3DImage::Pointer blImageCopy::copyIntColor3DImage(IntColor3DImage::Pointer image){
    typedef itk::ImageDuplicator< IntColor3DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}

FloatColor3DImage::Pointer blImageCopy::copyFloatColor3DImage(FloatColor3DImage::Pointer image){
    typedef itk::ImageDuplicator< FloatColor3DImage > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();
    return duplicator->GetOutput();
}
