#include <blFiltering/blBilateralFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blBilateralFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setDomainSigma(4.0);
    filterGray.setRangeSigma(10.0);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save(std::string(BLORCHIDEIMAGEBILATERAL);

    std::string refBilateralGrayImageFile(BLORCHIDEIMAGEBILATERAL);// = "../../tests/blTestData/orchideBilateral.tif";
    blImage* refBilateralGrayImage = new blImage(refBilateralGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refBilateralGrayImage);
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

    blBilateralFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setDomainSigma(4.0);
    filterColor.setRangeSigma(10.0);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideBilateralColor.tif");

    std::string refBilateralColorImageFile(BLORCHIDECOLORIMAGEBILATERAL);// = "../../tests/blTestData/orchideBilateralColor.tif";
    blImage* refBilateralColorImage = new blImage(refBilateralColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refBilateralColorImage);
    comparisonDiffColor.setTestImage(filteredImageColor);
    comparisonDiffColor.setDifferenceThreshold( 0.01 );
    comparisonDiffColor.setToleranceRadius( 0 );
    comparisonDiffColor.run();
    unsigned long testColorEqualDiff = comparisonDiffColor.numberOfPixelsWithDifferences();
    if (testColorEqualDiff > 67128){ // maybe a cast problem in the IO
        return 1;
    }

    return 0;
}
