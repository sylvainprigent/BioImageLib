#include <blFiltering/blVectorCurvatureAnisotropicDiffusionFilter.h>
#include <blComparisonImage/blComparisonImageFilter.h>
#include <iostream>
#include "blFilteringTestConfig.h"

int main(int argc, char* argv[])
{
    // test on a color image
    std::string colorImageFile(BLORCHIDECOLORIMAGE);// = "../../tests/blTestData/orchideColor.tif";
    blImage* imagecolor = new blImage(colorImageFile);

    blVectorCurvatureAnisotropicDiffusionFilter filterColor;
    filterColor.setInput(imagecolor);
    filterColor.setNumberOfIterations(10);
    filterColor.setTimeStep(0.125);
    filterColor.setConductanceParameter(1.5);
    filterColor.run();
    blImage* filteredImageColor = filterColor.output();
    std::cout << "image type = " << filteredImageColor->imageType() << std::endl;
    //filteredImageColor->save("../../tests/blTestData/orchideVectorCurvatureAnisotropicDiffusionColor.tif");

    std::string refVectorCurvatureAnisotropicDiffusionColorImageFile(BLORCHIDEIMAGEVCAD);// = "../../tests/blTestData/orchideVectorCurvatureAnisotropicDiffusionColor.tif";
    blImage* refVectorCurvatureAnisotropicDiffusionColorImage = new blImage(refVectorCurvatureAnisotropicDiffusionColorImageFile);
    blComparisonImageFilter comparisonDiffColor;
    comparisonDiffColor.setReferenceImage(refVectorCurvatureAnisotropicDiffusionColorImage);
    comparisonDiffColor.setTestImage(filteredImageColor);
    comparisonDiffColor.setDifferenceThreshold( 0.01 );
    comparisonDiffColor.setToleranceRadius( 0 );
    comparisonDiffColor.run();
    unsigned long testColorEqualDiff = comparisonDiffColor.numberOfPixelsWithDifferences();
    if (testColorEqualDiff > 67103){ // save float problem ?
        return 1;
    }

    return 0;
}
