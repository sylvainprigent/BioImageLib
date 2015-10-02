/// \file blMppResultRepresentation.cpp
/// \brief blMppResultRepresentation class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppResultRepresentation.h"

#include "itkRescaleIntensityImageFilter.h"
#include <blCastStacks>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

blImage* blMppResultRepresentation::imageRepresentation(std::vector<blMppShape*> shapes,
                                                        std::string representationType,
                                                        blImage* image, bool useRandColor){

    if (image->is2D()){
        return imageRepresentation2D(shapes, representationType, image, useRandColor);
    }
    else if (image->is3D()){
        return imageRepresentation3D(shapes, representationType, image, useRandColor);
    }
    return NULL;

}

blImage* blMppResultRepresentation::imageRepresentation2D(std::vector<blMppShape*> shapes,
                                                        std::string representationType,
                                                        blImage* image, bool useRandColor){

    std::cout << "representation image type = " << image->imageType() << std::endl;
    FloatColor2DImage::Pointer resImage = NULL;

    if (image->imageType() == blImage::TypeIntColor2D){
        blCastImageToFloat caster;
        caster.setInput(image);
        caster.run();
        blImage* imageC = caster.output();
        delete image;
        image = imageC;
        imageC = NULL;
    }

    if (image->imageType() == blImage::TypeFloat2D){
        Float2DImage::Pointer imageRes = image->itkFloat2DImagePointer();

        // normalize intensity
        typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
        RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
        rescaleFilter->SetInput(imageRes);
        rescaleFilter->SetOutputMinimum(0);
        rescaleFilter->SetOutputMaximum(255);
        rescaleFilter->Update();
        imageRes = rescaleFilter->GetOutput();

        // create image
        resImage = FloatColor2DImage::New();
        FloatColor2DImage::RegionType region;
        FloatColor2DImage::IndexType start;
        start[0] = 0;
        start[1] = 0;
        region.SetIndex(start);

        itk::Image<itk::RGBPixel<float>, 2>::SizeType size;
        size[0] = image->imageSize().xSize();
        size[1] = image->imageSize().ySize();
        region.SetSize(size);

        resImage->SetRegions(region);
        resImage->Allocate();

        // copy the input image to the rgb output
        typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorType;
        typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
        IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
        IteratorTypeFloat iteratorInitImage(imageRes, imageRes->GetLargestPossibleRegion());

        PixelColorFloatType pixel;
        while(!iteratorInitImage.IsAtEnd()){
            pixel[0] = iteratorInitImage.Get();
            pixel[1] = pixel[0]; pixel[2] = pixel[0];
            iteratorResImage.Set(pixel);
            ++iteratorResImage;
            ++iteratorInitImage;
        }
    }
    else if(image->imageType() == blImage::TypeFloatColor2D){
        resImage = image->itkFloatColor2DImagePointer();
    }
    else{
        return new blImage();
    }

    // draw pixels on image
    if (shapes.size() == 0){
        blImage* finalImage = new blImage(resImage);
        return finalImage;
    }
    else{
        // draw the objects
        FloatColor2DImage::IndexType pixelIndex;
        PixelColorFloatType pixel;
        std::vector<blPixel> *pix;
        pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
        for (unsigned int i = 0 ; i < shapes.size() ; ++i ){
            pix = shapes[i]->representation(representationType);

            if (useRandColor){
                std::vector<int> rgb = blColor::GetRandRGB();
                pixel[0] = rgb[0]; pixel[1] = rgb[1]; pixel[2] = rgb[2];
            }

            for (unsigned int k = 0 ; k < pix->size() ; k++){
                pixelIndex[0] = pix->operator [](k).x();
                pixelIndex[1] = pix->operator [](k).y();
                resImage->SetPixel(pixelIndex, pixel);
            }
        }

        blImage* finalImage = new blImage(resImage);
        return finalImage;
    }
}

blImage* blMppResultRepresentation::imageRepresentation3D(std::vector<blMppShape*> shapes,
                                                        std::string representationType,
                                                        blImage* image, bool useRandColor){

    Float3DImage::Pointer imageRes = image->itkFloat3DImagePointer();

    // normalize intensity
    typedef itk::RescaleIntensityImageFilter< Float3DImage, Float3DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(imageRes);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    imageRes = rescaleFilter->GetOutput();

    // create image
    FloatColor3DImage::Pointer resImage = FloatColor3DImage::New();

    FloatColor3DImage::RegionType region;
    FloatColor3DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    region.SetIndex(start);

    itk::Image<itk::RGBPixel<float>, 3>::SizeType size;
    size[0] = image->imageSize().xSize();
    size[1] = image->imageSize().ySize();
    size[2] = image->imageSize().zSize();
    region.SetSize(size);

    resImage->SetRegions(region);
    resImage->Allocate();

    // copy the input image to the rgb output
    //std::cout << "copy the input image to the rgb output" << std::endl;
    typedef itk::ImageRegionIterator<FloatColor3DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float3DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageRes, imageRes->GetLargestPossibleRegion());

    PixelColorFloatType pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    // draw pixels on image
    //std::cout << "draw pixels on image" << std::endl;
    if (shapes.size() == 0){
        blImage* finalImage = new blImage(resImage);
        return finalImage;
    }
    else{
        // draw the objects
        //std::cout << "draw the objects" << std::endl;
        FloatColor3DImage::IndexType pixelIndex;
        PixelColorFloatType pixel;
        std::vector<blPixel> *pix;
        pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
        for (unsigned int i = 0 ; i < shapes.size() ; ++i ){

            pix = shapes[i]->representation(representationType);

            if (useRandColor){
                std::vector<int> rgb = blColor::GetRandRGB();
                pixel[0] = rgb[0]; pixel[1] = rgb[1]; pixel[2] = rgb[2];
            }

            for (unsigned int k = 0 ; k < pix->size() ; k++){
                pixelIndex[0] = pix->operator [](k).x();
                pixelIndex[1] = pix->operator [](k).y();
                pixelIndex[2] = pix->operator [](k).z();
                resImage->SetPixel(pixelIndex, pixel);
            }
        }

        blImage* finalImage = new blImage(resImage);
        return finalImage;
    }
}


void blMppResultRepresentation::saveShapeListToFile(std::vector<blMppShape*> shapes, std::string filePath){
    ofstream file(filePath.c_str(), ios::out | ios::trunc);
    if(file)
    {
        if (shapes.size() > 1){
            for (unsigned int i = 0 ; i < shapes.size() ; ++i){
                vector<float> parameters = shapes[i]->getParameters();
                for (unsigned int j = 0 ; j < parameters.size() ; ++j){
                    file << parameters[j] << "\t";
                }
                file << shapes[i]->dataTerm() << "\t";
                file << endl;

            }
        }
        file.close();
    }
    else{
        throw blException("blMppResultRepresentation -> Impossible to open the file");
    }
}

void blMppResultRepresentation::saveShapeListToFileInsidePixels(std::vector<blMppShape*> shapes){
  string fileName = "insidePix.txt";
  int sizeInsPix;
  blPixel center;
  vector<float> parameters;
  std::vector<blPixel>* insidePix;
  ofstream file(fileName.c_str());
    if(file)
    {
        if (shapes.size() > 1){

            for (unsigned int i = 0 ; i < shapes.size() ; ++i){
            insidePix = shapes[i]->insidePixels();
        sizeInsPix = insidePix->size();
        file << "new Shape" << "\n";

        for (unsigned int j = 0; j<sizeInsPix; j++){
          file << (*insidePix)[j].x() << "\t";
          file << (*insidePix)[j].y() << "\t";
          file << (*insidePix)[j].z() << "\t";
          file <<  "\n" ;
                }

        center = shapes[i]->getCenter();
        parameters = shapes[i]->getParameters();
        file << "center" << "\n";
        file << center.x() << "\t";
        file << center.y() << "\t";
        file << center.z() << "\t";
        file <<  "\n" ;
        file << "radius" << "\n";
        file << parameters[3] << "\t";
        file << parameters[4] << "\t";
        file <<  "\n" ;
        //file << "Rayon" << "\n";
        //file << rayonCir << "\n";
            }
        }
        file.close();
    }
    else{
        throw blException("blMppResultRepresentation -> Impossible to create the file");
    }
}

