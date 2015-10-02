#include <iostream>
#include <string>
#include <vector>
#include <blMppFactories.h>
#include <blTxtIO>
#include <blMpp>
#include <blCastStacks>
#include "blRand/blRandomGeneratorPM.h"


#include <time.h>
#include <cstdlib>
#include <ctime>

std::string usage(std::string name)
{
    return     "" + name + " <option(s)> "
            + "Options:\n"
            + "\t-h\t\tShow this help message\n"
            + "\t-input INPUT\tInput image\n"
            + "\t-outputI OUTPUTIMAGE\tOutput image\n"
            + "\t-outputO OUTPUTTXT\tOutput object in txt file\n";
}

int main(int argc, char* argv[])
{
    try{
        blCmdParser parser(argc, argv);
        parser.addArg("InputImage");
        parser.addArg("OutputImage");
        parser.addArg("OutputTxt");
        parser.addArg("blMppAlgorithm");
        parser.addArg("blMppGenerator");
        parser.addArg("blMppDataTerm");
        parser.addArg("blMppInteraction");
        parser.addArg("blMppAlgorithmMBCR_threadNumber");
        parser.addArg("blMppAlgorithmMBCR_maxIterations");
        parser.addArg("blMppAlgorithmMBCR_perturbationProba");
        parser.addArg("blMppAlgorithmMBCR_newShapesPerIterations");
        parser.addArg("blMppAlgorithmMBCR_integerCoef");
        parser.addArg("blMppAlgorithmMBCR_iterationsVerboseRatio");
        parser.addArg("blMppGeneratorShapeEllipse_minMajorAxis");
        parser.addArg("blMppGeneratorShapeEllipse_maxMajorAxis");
        parser.addArg("blMppGeneratorShapeEllipse_minMinorAxis");
        parser.addArg("blMppGeneratorShapeEllipse_maxMinorAxis");
        parser.addArg("blMppGeneratorShapeEllipse_minTheta");
        parser.addArg("blMppGeneratorShapeEllipse_maxTheta");
        parser.addArg("blMppGeneratorShapeEllipse_thetaStep");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_translationProba");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_translationResize");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_translationRotate");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_deltaResize");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_deltaTranslate");
        parser.addArg("blMppGeneratorShapeSuperquadric2D_deltaRotate");
        parser.addArg("blMppDataTermBhattacharyya_D");
        parser.addArg("blMppDataTermBhattacharyya_gradientDirection");
        parser.addArg("blMppDataTermBhattacharyya_threshold");
        parser.addArg("blMppDataTerm_borderWidth");
        parser.addArg("blMppInteractionNoOverlap_overlapCoef");
        parser.addArg("blMppInteractionNoOverlap_minZDistance");

        parser.setUsage(usage(argv[0]));
        blProcessParameters* parameters = parser.parse(2);

        parameters->printParamConsole();

        string inputImage = parameters->getValueOfKey<string>("InputImage", "");
        string outputImage = parameters->getValueOfKey<string>("OutputImage", "");
        string outputTxtFile = parameters->getValueOfKey<string>("OutputTxt", "");

        if (inputImage == "" || outputImage == "" || outputTxtFile == ""){
            std::cout << "InputImage, OutputImage and OutputTxt are mandatory inputs";
            return 1;
        }

        std::cout << "inputImage = " << inputImage << std::endl;
        std::cout << "outputImage = " << outputImage << std::endl;
        std::cout << "outputTxtFile = " << outputTxtFile << std::endl;

        clock_t start = clock();
        blRandomGeneratorPM::srand(10);

        // load image
        blImage* image = new blImage(inputImage);
        std::cout << "image->imageType() = " << image->imageType() << std::endl;
        if (image->imageType() == blImage::TypeInt2D || image->imageType() == blImage::TypeInt3D){
            blCastImageToFloat castFilter;
            castFilter.setInput(image);
            castFilter.run();
            image = castFilter.output();
        }

        // get process
        blMppFactory factory(parameters);
        blMppGeneratorShape* generator = factory.generator(parameters->getValueOfKey<string>("blMppGenerator", ""));
        blMppDataTerm* dataTerm = factory.dataTerm(parameters->getValueOfKey<string>("blMppDataTerm", ""));
        dataTerm->setImage(image);
        blMppInteraction* interaction = factory.interaction(parameters->getValueOfKey<string>("blMppInteraction", ""));
        blMppAlgorithm* algorithm = factory.algorithm(generator, dataTerm, interaction, parameters->getValueOfKey<string>("blMppAlgorithm", ""));

        // Init
        /*
        if ( argc >= 6){
            generator->setBirthMap(new blImage(argv[5]));
        }
        else{
            generator->setBirthArea(image->imageSize());
        }
        */
        generator->setBirthArea(image->imageSize());

        blProcessObserverCommandeLine* obs = new blProcessObserverCommandeLine();
        algorithm->addObserver(obs);
        generator->addObserver(obs);

        // run
        dataTerm->initialize();
        generator->initialize();
        interaction->initialize();
        algorithm->compute();

        cout << "get shapes" << endl;
        std::vector<blMppShape*> shapes = algorithm->GetShapes();
        cout << "shapes size = " << shapes.size() << endl;
        cout << "save output " << endl;

        int useRandColor = parameters->getValueOfKey<int>("blMppResultRepresentation_randColor", 0);
        bool useRandColorb = false;
        if (useRandColor > 0){
            useRandColorb = true;
        }

        blImage* output = blMppResultRepresentation::imageRepresentation(shapes, "border", image, useRandColorb);
        output->save(outputImage, true);

        blMppResultRepresentation::saveShapeListToFile(shapes, outputTxtFile);
        cout << "save output finished" << endl;
        // free memory
        delete algorithm;
        delete interaction;
        delete dataTerm;
        delete generator;
        delete parameters;
        delete image;
        delete output;
        delete obs;
        for (int i = 0 ; i < shapes.size() ; ++i){
            delete shapes[i];
        }

        printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
    }
    catch (blException ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
