#include <blFiltering/blBinomialBlurFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blBinomialBlurFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setRepetitions(5);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideBinomialBlur.tif");

    std::string refBinomialBlurGrayImageFile(BLORCHIDEIMAGEBILATERALB);// = "../../tests/blTestData/orchideBinomialBlur.tif";
    blImage* refBinomialBlurGrayImage = new blImage(refBinomialBlurGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refBinomialBlurGrayImage);
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

    blBinomialBlurFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setRepetitions(5);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideBinomialBlurColor.tif");

    std::string refBinomialBlurColorImageFile(BLORCHIDECOLORIMAGEBILATERALB);// = "../../tests/blTestData/orchideBinomialBlurColor.tif";
    blImage* refBinomialBlurColorImage = new blImage(refBinomialBlurColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refBinomialBlurColorImage);
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
