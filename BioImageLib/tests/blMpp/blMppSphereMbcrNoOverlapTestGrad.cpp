//#include <blCore/blColor.h>

#include <blCore>
#include <blMpp>
#include <blRand>
#include <blCastStacks>
#include "blTestConfig.h"

#include <iostream>

std::vector<blMppShape*> runMpp(std::string imageFile, std::string birthMap);

int main(int argc, char* argv[])
{

    std::cout << "input image = " << BLMPPEMBRYONTESTIMAGE << std::endl;
    std::cout << "input image = " << BLMPPEMBRYONBMTESTIMAGE << std::endl;

    int returnVal = 0;

    // test 1 without birth map
    //std::cout << "test 1" << std::endl;
    std::vector<blMppShape*> shapes = runMpp(BLMPPEMBRYONTESTIMAGE, "");
    //std::cout << "size no bm" << shapes.size() << std::endl;
    if (shapes.size() != 92){
        returnVal = 1;
    }
    for (int i = 0 ; i < shapes.size() ; ++i){
        delete shapes[i];
    }

    // test 1 without birth map
    //std::cout << "test 2" << std::endl;
    std::vector<blMppShape*> shapes2 = runMpp(BLMPPEMBRYONTESTIMAGE, BLMPPEMBRYONBMTESTIMAGE);
    if (shapes2.size() != 109){
        returnVal = 1;
    }
    //std::cout << "size bm" << shapes2.size() << std::endl;
    for (int i = 0 ; i < shapes2.size() ; ++i){
        delete shapes2[i];
    }

    return returnVal;

}

std::vector<blMppShape*> runMpp(std::string imageFile, std::string birthMap){

    blRandomGeneratorPM::srand( 10 ); // initialize the random

    // load image
    blImage* imageToCast = new blImage(imageFile);
    blCastImageToFloat castFilter;
    castFilter.setInput(imageToCast);
    castFilter.run();
    blImage* image = castFilter.output();
    delete imageToCast;

    // initialize classes
    blMppGeneratorShapeSphere* generator = new blMppGeneratorShapeSphere();
    generator->setRadiusRange(5, 8);
    generator->setPerturbationsProba(0.33, 0.33, 0.34);
    generator->setDeltaResize(1);
    generator->setDeltaTranslation(1);
    generator->setDeltaRotation(0.2);

    blMppDataTermGradient *dataTerm = new blMppDataTermGradient();
    dataTerm->setD(100);
    dataTerm->setGradientDirection(1);
    dataTerm->setSigma(1);
    dataTerm->setThreshold(5);
    dataTerm->setBorderWidth(1);
    dataTerm->setImage(image);

    blMppInteractionNoOverlap* interaction = new blMppInteractionNoOverlap();
    interaction->setOverlapCoef(0.1);

    blMppAlgorithmMBCR *algorithm = new blMppAlgorithmMBCR(generator, dataTerm, interaction);
    algorithm->setMaxIterations(200);
    algorithm->setPerturbationProba(0.4);
    algorithm->setNewShapesPerIterations(100);
    algorithm->setIntegerCoef(1000);
    algorithm->setIterationsVerboseRatio(100);

    // set birt map
    blImage* birthMapImage = NULL;
    if (!birthMap.empty()){
        birthMapImage = new blImage(birthMap);
        generator->setBirthMap(birthMapImage);
    }
    else{
        generator->setBirthArea(image->imageSize());
    }

    // run
    dataTerm->initialize();
    generator->initialize();
    interaction->initialize();
    algorithm->compute();

    std::vector<blMppShape*> shapes = algorithm->GetShapes();

    // save outputs
    /*
    if (birthMap.empty()){
        blImage* output = blMppResultRepresentation::imageRepresentation(shapes, "border", image, false);
        output->save(std::string(BLMPPCELLTESTIMAGE) + "_res.tif", true);
        delete output;
    }
    else{
        blImage* output = blMppResultRepresentation::imageRepresentation(shapes, "border", image, false);
        output->save(std::string(BLMPPCELLTESTIMAGE) + "_resbm.tif", true);
        delete output;
    }
    */

    // delete
    delete algorithm;
    delete interaction;
    delete dataTerm;
    delete generator;
    delete image;
    if (!birthMap.empty()){delete birthMapImage;}

    // return
    return shapes;
}
