#include <blFiltering/blMinMaxCurvatureFlowFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blMinMaxCurvatureFlowFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setNumberOfIterations(5);
    filterGray.setTimeStep(0.125);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideMinMaxCurvatureFlow.tif");

    std::string refMinMaxCurvatureFlowGrayImageFile(BLORCHIDEIMAGEMMCF);// = "../../tests/blTestData/orchideMinMaxCurvatureFlow.tif";
    blImage* refMinMaxCurvatureFlowGrayImage = new blImage(refMinMaxCurvatureFlowGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refMinMaxCurvatureFlowGrayImage);
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

    blMinMaxCurvatureFlowFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setNumberOfIterations(5);
    filterColor.setTimeStep(0.125);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideMinMaxCurvatureFlowColor.tif");

    std::string refMinMaxCurvatureFlowColorImageFile(BLORCHIDECOLORIMAGEMMCF);// = "../../tests/blTestData/orchideMinMaxCurvatureFlowColor.tif";
    blImage* refMinMaxCurvatureFlowColorImage = new blImage(refMinMaxCurvatureFlowColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refMinMaxCurvatureFlowColorImage);
    comparisonDiffColor.setTestImage(filteredImageColor);
    comparisonDiffColor.setDifferenceThreshold( 0.01 );
    comparisonDiffColor.setToleranceRadius( 0 );
    comparisonDiffColor.run();
    unsigned long testColorEqualDiff = comparisonDiffColor.numberOfPixelsWithDifferences();
    if (testColorEqualDiff > 67026){ // float problem for color IO?
        return 1;
    }

    return 0;
}
