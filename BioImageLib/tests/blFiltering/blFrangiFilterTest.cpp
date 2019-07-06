#include <blFiltering/blFrangiFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    std::cout << "input image = " << BLFRANGIIMAGE << std::endl;
    std::cout << "input image = " << BLFRANGIIMAGERES << std::endl;
    // test on a gray scale image
    std::string grayImageFile(BLFRANGIIMAGE);// = "../../tests/blTestData/Sidestream_dark_field_image.png";
    blImage* imageGray = new blImage(grayImageFile);

    blFrangiFilter filterGray;
    filterGray.setInput(imageGray);
    filterGray.setAlpha( 0.5 );
    filterGray.setBeta( 1.0 );
    filterGray.setGamma( 5.0 );
    filterGray.setBrightObject(false);
    filterGray.setSigmaMinimum( 1.0 );
    filterGray.setSigmaMaximum( 10.0 );
    filterGray.setNumberOfSigmaSteps( 10 );
    filterGray.run();
    blImage* filteredImageGray = filterGray.output();
    //filteredImageGray->save(std::string(BLFRANGIIMAGERES));

    std::string refFrangiGrayImageFile(BLFRANGIIMAGERES);// = "../../tests/blTestData/Sidestream_dark_field_image_Frangi.tif";
    blImage* refFrangiGrayImage = new blImage(refFrangiGrayImageFile);
    blComparisonImageFilter comparisonDiffGray;
    comparisonDiffGray.setReferenceImage(refFrangiGrayImage);
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
