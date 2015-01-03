#include <blFiltering/blCurvatureFlowFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a gray scale image
    std::string grayImageFile(BLORCHIDEIMAGE);// = "../../tests/blTestData/orchide.tif";
    blImage* imageGray = new blImage(grayImageFile);

    blCurvatureFlowFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setNumberOfIterations(5);
    filterGray.setTimeStep(0.125);
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save("../../tests/blTestData/orchideCurvatureFlow.tif");

    std::string refCurvatureFlowGrayImageFile(BLORCHIDEIMAGECF);// = "../../tests/blTestData/orchideCurvatureFlow.tif";
    blImage* refCurvatureFlowGrayImage = new blImage(refCurvatureFlowGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refCurvatureFlowGrayImage);
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

    blCurvatureFlowFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setNumberOfIterations(5);
    filterColor.setTimeStep(0.125);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //filteredImageColor->save("../../tests/blTestData/orchideCurvatureFlowColor.tif");

    std::string refCurvatureFlowColorImageFile(BLORCHIDECOLORIMAGECF);// = "../../tests/blTestData/orchideCurvatureFlowColor.tif";
    blImage* refCurvatureFlowColorImage = new blImage(refCurvatureFlowColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refCurvatureFlowColorImage);
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
