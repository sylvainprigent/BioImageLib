/// \file blKernelConvolution.cpp
/// \brief blKernelConvolution class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blKernelConvolution.h"

blKernelConvolution::blKernelConvolution(const blMatrix &matrix){

    typedef Float2DImage ImageType;
    // Allocate image
    ImageType::RegionType region;
    ImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;

    ImageType::SizeType size;
    unsigned int NumRows = matrix.numberOfRows();
    unsigned int NumCols = matrix.numberOfColumns();
    size[0] = NumRows;
    size[1] = NumCols;

    region.SetSize(size);
    region.SetIndex(start);

    ImageType::Pointer image = ImageType::New();
    image->SetRegions(region);
    image->Allocate();

    // fill data
    for(unsigned int r = 0; r < NumRows; r++){
        for(unsigned int c = 0; c < NumCols; c++){
            ImageType::IndexType pixelIndex;
            pixelIndex[0] = r;
            pixelIndex[1] = c;

            image->SetPixel(pixelIndex, matrix(r,c));
        }
    }
    m_kernel = new blImage(image);
}

blKernelConvolution::blKernelConvolution(const blMatrix3D &matrix){

    typedef Float3DImage ImageType;
    // Allocate image
    ImageType::RegionType region;
    ImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;

    ImageType::SizeType size;
    unsigned int dimX = matrix.dimX();
    unsigned int dimY = matrix.dimY();
    unsigned int dimZ = matrix.dimZ();
    size[0] = dimX;
    size[1] = dimY;
    size[2] = dimZ;

    region.SetSize(size);
    region.SetIndex(start);

    ImageType::Pointer image = ImageType::New();
    image->SetRegions(region);
    image->Allocate();

    // fill data
    for(unsigned int r = 0; r < dimX; r++){
        for(unsigned int c = 0; c < dimY; c++){
            for(unsigned int z = 0; z < dimZ; z++){
                ImageType::IndexType pixelIndex;
                pixelIndex[0] = r;
                pixelIndex[1] = c;
                pixelIndex[2] = z;

                image->SetPixel(pixelIndex, matrix(r,c,z));
            }
        }
    }
    m_kernel = new blImage(image);
}

blImage* blKernelConvolution::kernel(){
    return m_kernel;
}
