#include <blFiltering/blMedianFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blMedianFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setRadius(5);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();

    std::string refMedianGrayImageFile(BLORCHIDEIMAGEMED);// = "../../tests/blTestData/orchideMedian.tif";
    blImage* refMedianGrayImage = new blImage(refMedianGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refMedianGrayImage);
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

    blMedianFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setRadius(5);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();

    std::string refMedianColorImageFile(BLORCHIDECOLORIMAGEMED);// = "../../tests/blTestData/orchideMedianColor.tif";
    blImage* refMedianColorImage = new blImage(refMedianColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refMedianColorImage);
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
