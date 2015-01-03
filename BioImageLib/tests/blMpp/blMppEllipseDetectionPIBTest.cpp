//#include <blCore/blColor.h>

#include <blCore>
#include <blMpp>
#include "blTestConfig.h"
#include <blRand>
#include <blCastStacks>

#include <iostream>

int main(int argc, char* argv[])
{
  std::cout<<"Begin test "<<std::endl;
  std::cout<<BLMPPTESTIMAGE<<std::endl;

  // UpdateMpp
  std::string imageFile(BLMPPTESTIMAGE);

  blRandomGeneratorPM::srand( 10 ); // initialize the random

  // load image
  blImage* imageToCast = new blImage(imageFile);
  blCastImageToFloat castFilter;
  castFilter.setInput(imageToCast);
  castFilter.run();
  blImage* image = castFilter.output();
  delete imageToCast;

  if (!image)
    return 1;

  blMppGeneratorShapeEllipse* generator = new blMppGeneratorShapeEllipse();
  generator->setMajorAxisRange(14, 18);
  generator->setMinorAxisRange(8, 12);
  generator->setThetaRange(0, 3.14, 0.2);
  generator->setPerturbationsProba(0.33, 0.33, 0.34);
  generator->setDeltaResize(1);
  generator->setDeltaTranslation(1);
  generator->setDeltaRotation(0.2);
  generator->setBirthArea(image->imageSize());
  if (!generator)
    return 1;

  blMppDataTermGradientCompt* dataTerm = new blMppDataTermGradientCompt();
  dataTerm->setCompt(2);
  dataTerm->setGradientDirection(-1);
  dataTerm->setThreshold(-0.7);
  dataTerm->setBorderWidth(1);
  dataTerm->setImage(image);
  if (!dataTerm)
    return 1;

  blMppInteractionNoOverlap* interaction = new blMppInteractionNoOverlap();
  interaction->setOverlapCoef(0.0);
  if (!interaction)
    return 1;

  blMppAlgorithmMBCR *algorithm = new blMppAlgorithmMBCR(generator, dataTerm, interaction);
  algorithm->setMaxIterations(20000);
  algorithm->setPerturbationProba(0.5);
  algorithm->setNewShapesPerIterations(50);
  algorithm->setIntegerCoef(1000);
  algorithm->setIterationsVerboseRatio(100);
  if (!algorithm)
    return 1;

  // run
  dataTerm->initialize();
  generator->initialize();
  interaction->initialize();
  algorithm->compute();

  std::vector<blMppShape*> shapes = algorithm->GetShapes();

  unsigned int nbEllipses = shapes.size();
  //std::cout << "nbEllipses = " << nbEllipses << std::endl;
  int returnVal = 0;
  if (nbEllipses != 108){
      returnVal = 1;
  }

  // save output
  /*
  blImage* output = blMppResultRepresentation::imageRepresentation(shapes, "border", image, false);
  output->save(std::string(BLMPPTESTIMAGE) + "_res.tif", true);
  delete output;
  */

  for (int i = 0 ; i < shapes.size() ; ++i){
      delete shapes[i];
  }

  delete algorithm;
  delete interaction;
  delete dataTerm;
  delete generator;
  delete image;
  return returnVal;
}
