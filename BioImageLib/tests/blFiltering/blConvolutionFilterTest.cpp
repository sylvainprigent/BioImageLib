#include <blFiltering/blConvolutionFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <blFiltering/blKernelConvolution.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test 2D kernel
    blMatrix matrix(3,3);
    matrix(0,0) = 1;
    matrix(0,1) = 1;
    matrix(0,2) = 1;
    matrix(1,0) = 1;
    matrix(1,1) = 1;
    matrix(1,2) = 1;
    matrix(2,0) = 1;
    matrix(2,1) = 1;
    matrix(2,2) = 1;

    blKernelConvolution kernel2D(matrix);
    blImage* kernelImage2D = kernel2D.kernel();


    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blConvolutionFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setKernel(kernelImage2D);
    filterGray.setNormalizeKernel(true);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideConvolution.tif");

    std::string refConvolutionGrayImageFile(BLORCHIDEIMAGECONV);// = "../../tests/blTestData/orchideConvolution.tif";
    blImage* refConvolutionGrayImage = new blImage(refConvolutionGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refConvolutionGrayImage);
    comparisonDiffGray.setTestImage(filteredImageGray);
    comparisonDiffGray.setDifferenceThreshold( 0.01 );
    comparisonDiffGray.setToleranceRadius( 0 );
    comparisonDiffGray.run();
    unsigned long testGrayEqualDiff = comparisonDiffGray.numberOfPixelsWithDifferences();
    if (testGrayEqualDiff > 0){
        return 1;
    }

    return 0;
}
