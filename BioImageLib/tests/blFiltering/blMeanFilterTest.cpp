#include <blFiltering/blMeanFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blMeanFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideMean.tif");

    std::string refMeanGrayImageFile(BLORCHIDEIMAGEMEAN);// = "../../tests/blTestData/orchideMean.tif";
    blImage* refMeanGrayImage = new blImage(refMeanGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refMeanGrayImage);
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

    blMeanFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideMeanColor.tif");

    std::string refMeanColorImageFile(BLORCHIDECOLORIMAGEMEAN);// = "../../tests/blTestData/orchideMeanColor.tif";
    blImage* refMeanColorImage = new blImage(refMeanColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refMeanColorImage);
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
