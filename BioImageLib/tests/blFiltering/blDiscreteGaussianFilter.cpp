#include <blFiltering/blDiscreteGaussianFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blDiscreteGaussianFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setVariance(5);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideDiscreteGaussian.tif");

    std::string refDiscreteGaussianGrayImageFile(BLORCHIDEIMAGEDG);// = "../../tests/blTestData/orchideDiscreteGaussian.tif";
    blImage* refDiscreteGaussianGrayImage = new blImage(refDiscreteGaussianGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refDiscreteGaussianGrayImage);
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
