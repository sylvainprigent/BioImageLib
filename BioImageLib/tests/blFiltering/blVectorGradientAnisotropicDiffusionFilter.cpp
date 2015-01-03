#include <blFiltering/blVectorGradientAnisotropicDiffusionFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a color image
    std::string colorImageFile(BLORCHIDECOLORIMAGE);// = "../../tests/blTestData/orchideColor.tif";
    blImage* imagecolor = new blImage(colorImageFile);

    blVectorGradientAnisotropicDiffusionFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setNumberOfIterations(10);
    filterColor.setTimeStep(0.125);
    filterColor.setConductanceParameter(1.5);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    //std::cout << "image type = " << filteredImageColor->imageType() << std::endl;
    //filteredImageColor->save("../../tests/blTestData/orchideVectorGradientAnisotropicDiffusionColor.tif");

    std::string refVectorGradientAnisotropicDiffusionColorImageFile(BLORCHIDECOLORIMAGEVGAD);// = "../../tests/blTestData/orchideVectorGradientAnisotropicDiffusionColor.tif";
    blImage* refVectorGradientAnisotropicDiffusionColorImage = new blImage(refVectorGradientAnisotropicDiffusionColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refVectorGradientAnisotropicDiffusionColorImage);
    comparisonDiffColor.setTestImage(filteredImageColor);
    comparisonDiffColor.setDifferenceThreshold( 0.01 );
    comparisonDiffColor.setToleranceRadius( 0 );
    comparisonDiffColor.run();
    unsigned long testColorEqualDiff = comparisonDiffColor.numberOfPixelsWithDifferences();
    //std::cout << "testColorEqualDiff = " << testColorEqualDiff << std::endl;
    if (testColorEqualDiff > 67128){ // save float problem ?
        return 1;
    }

    return 0;
}
