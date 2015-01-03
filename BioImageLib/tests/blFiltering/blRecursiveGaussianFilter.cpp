#include <blFiltering/blRecursiveGaussianFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blRecursiveGaussianFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setSigma(5);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideRecursiveGaussian.tif");

    std::string refRecursiveGaussianGrayImageFile(BLORCHIDEIMAGERGF);// = "../../tests/blTestData/orchideRecursiveGaussian.tif";
    blImage* refRecursiveGaussianGrayImage = new blImage(refRecursiveGaussianGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refRecursiveGaussianGrayImage);
    comparisonDiffGray.setTestImage(filteredImageGray);
    comparisonDiffGray.setDifferenceThreshold( 0.01 );
    comparisonDiffGray.setToleranceRadius( 0 );
    comparisonDiffGray.run();
    unsigned long testGrayEqualDiff = comparisonDiffGray.numberOfPixelsWithDifferences();
    if (testGrayEqualDiff > 0){
        return 1;
    }


    // test on a color image
    std::string colorImageFile(BLORCHIDECOLORIMAGE);// = "../../tests/blTestData/orchideColor.tif";
    blImage* imagecolor = new blImage(colorImageFile);

    blRecursiveGaussianFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setSigma(5);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideRecursiveGaussianColor.tif");

    std::string refRecursiveGaussianColorImageFile(BLORCHIDECOLORIMAGERGF);// = "../../tests/blTestData/orchideRecursiveGaussianColor.tif";
    blImage* refRecursiveGaussianColorImage = new blImage(refRecursiveGaussianColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refRecursiveGaussianColorImage);
    comparisonDiffColor.setTestImage(filteredImageColor);
    comparisonDiffColor.setDifferenceThreshold( 0.01 );
    comparisonDiffColor.setToleranceRadius( 0 );
    comparisonDiffColor.run();
    unsigned long testColorEqualDiff = comparisonDiffColor.numberOfPixelsWithDifferences();
    if (testColorEqualDiff > 0){
        return 1;
    }

    return 0;
}
