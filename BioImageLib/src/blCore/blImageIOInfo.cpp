/// \file blImageIOInfo.cpp
/// \brief blImageIOInfo class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blImageIOInfo.h"
#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"
#include "blException.h"

#include <fstream>
#include <string>
#include <iostream>

blImageIOInfo::blImageIOInfo(){
    m_pixelType = "";
    m_dimension = 0;
    m_dimColor = 0;
}

void blImageIOInfo::updateInfo(std::string inputFilename){

    // test if the image file exists
    std::ifstream fichier( inputFilename.c_str() );
    if(fichier.fail()){
        std::string message = "the file " + inputFilename + " is not accessible";
        throw blException(message.c_str());
    }

    // run itk to read image info
    try{
        typedef itk::ImageIOBase::IOComponentType ScalarPixelType;

        //cout << "ImageIOInfo -> create IO pointer " << endl;
        itk::ImageIOBase::Pointer imageIO =
                itk::ImageIOFactory::CreateImageIO(
                    inputFilename.c_str(), itk::ImageIOFactory::ReadMode);
        //cout << "ImageIOInfo -> finish to create IO pointer " << endl;

        imageIO->SetFileName(inputFilename);
        //cout << "ImageIOInfo -> Set filename " << endl;
        imageIO->ReadImageInformation();
        const ScalarPixelType pixelType = imageIO->GetComponentType();

        m_pixelType = imageIO->GetComponentTypeAsString(pixelType); // float, double...
        m_dimension = imageIO->GetNumberOfDimensions(); // 2D, 3D
        m_dimColor = imageIO->GetPixelType(); // 1-grayscale...

        // Check if the z dimension is more than 1, otherwhise set image info to 2D
        if (m_dimension == 3){
            itk::ImageIOBase::SizeValueType size_z = imageIO->GetDimensions(2);
            if (size_z == 1){
                m_dimension = 2;
            }
        }
    }
    catch( itk::ExceptionObject & err ){
        throw blException(err.GetDescription());
    }

}
