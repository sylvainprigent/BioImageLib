#include <blFiltering/blKernelConvolution.h>
#include <blCore/blMatrix.h>
#include <blCore/blMatrix3D.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{

    // test 2D kernel
    blMatrix matrix(3,3);
    matrix(0,0) = 0;
    matrix(0,1) = 1;
    matrix(0,2) = 2;
    matrix(1,0) = 3;
    matrix(1,1) = 4;
    matrix(1,2) = 5;
    matrix(2,0) = 6;
    matrix(2,1) = 7;
    matrix(2,2) = 8;

    blKernelConvolution kernel2D(matrix);
    blImage* kernelImage2D = kernel2D.kernel();
    Float2DImage::Pointer itk2dKernel = kernelImage2D->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    pixelIndex[0] = 0; pixelIndex[1] = 0;
    if (itk2dKernel->GetPixel(pixelIndex) != 0){return 1;}
    pixelIndex[0] = 0; pixelIndex[1] = 1;
    if (itk2dKernel->GetPixel(pixelIndex) != 1){return 1;}
    pixelIndex[0] = 0; pixelIndex[1] = 2;
    if (itk2dKernel->GetPixel(pixelIndex) != 2){return 1;}
    pixelIndex[0] = 1; pixelIndex[1] = 0;
    if (itk2dKernel->GetPixel(pixelIndex) != 3){return 1;}
    pixelIndex[0] = 1; pixelIndex[1] = 1;
    if (itk2dKernel->GetPixel(pixelIndex) != 4){return 1;}
    pixelIndex[0] = 1; pixelIndex[1] = 2;
    if (itk2dKernel->GetPixel(pixelIndex) != 5){return 1;}
    pixelIndex[0] = 2; pixelIndex[1] = 0;
    if (itk2dKernel->GetPixel(pixelIndex) != 6){return 1;}
    pixelIndex[0] = 2; pixelIndex[1] = 1;
    if (itk2dKernel->GetPixel(pixelIndex) != 7){return 1;}
    pixelIndex[0] = 2; pixelIndex[1] = 2;
    if (itk2dKernel->GetPixel(pixelIndex) != 8){return 1;}


    // test 3D kernel
    blMatrix3D matrix3D(2,2,2);
    matrix3D(0,0,0) = 0;
    matrix3D(0,1,0) = 1;
    matrix3D(1,0,0) = 2;
    matrix3D(1,1,0) = 3;
    matrix3D(0,0,1) = 4;
    matrix3D(0,1,1) = 5;
    matrix3D(1,0,1) = 6;
    matrix3D(1,1,1) = 7;

    blKernelConvolution kernel3D(matrix3D);
    blImage* kernelImage3D = kernel3D.kernel();
    Float3DImage::Pointer itk3dKernel = kernelImage3D->itkFloat3DImagePointer();
    Float3DImage::IndexType pixelIndex3D;

    pixelIndex3D[0] = 0; pixelIndex3D[1] = 0; pixelIndex3D[2] = 0;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 0){return 1;}
    pixelIndex3D[0] = 0; pixelIndex3D[1] = 1; pixelIndex3D[2] = 0;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 1){return 1;}
    pixelIndex3D[0] = 1; pixelIndex3D[1] = 0; pixelIndex3D[2] = 0;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 2){return 1;}
    pixelIndex3D[0] = 1; pixelIndex3D[1] = 1; pixelIndex3D[2] = 0;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 3){return 1;}
    pixelIndex3D[0] = 0; pixelIndex3D[1] = 0; pixelIndex3D[2] = 1;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 4){return 1;}
    pixelIndex3D[0] = 0; pixelIndex3D[1] = 1; pixelIndex3D[2] = 1;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 5){return 1;}
    pixelIndex3D[0] = 1; pixelIndex3D[1] = 0; pixelIndex3D[2] = 1;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 6){return 1;}
    pixelIndex3D[0] = 1; pixelIndex3D[1] = 1; pixelIndex3D[2] = 1;
    if (itk3dKernel->GetPixel(pixelIndex3D) != 7){return 1;}

    return 0;
}
