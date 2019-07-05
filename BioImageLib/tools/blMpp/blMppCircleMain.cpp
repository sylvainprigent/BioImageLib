#include <iostream>
#include <string>
#include <vector>
#include "blMppFactories.h"
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
        parser.setUsage(usage("blMppCircle"));
        blProcessParameters* parameters = parser.pairParse();

        string p_inputImage = parameters->getValueOfKey<string>("-input");
        string p_outputImage = parameters->getValueOfKey<string>("-outputI");
        string p_outputTxtFile = parameters->getValueOfKey<string>("-outputO");


        if (p_inputImage == "" || p_outputImage == "" || p_outputTxtFile == ""){
            std::cout << "InputImage, OutputImage and OutputTxt are mandatory arguments";
            return 1;
        }

        std::cout << "inputImage = " << p_inputImage << std::endl;
        std::cout << "outputImage = " << p_outputImage << std::endl;
        std::cout << "outputTxtFile = " << p_outputTxtFile << std::endl;

        std::cout << "parameters list:" << std::endl;
        std::cout << "----------------" << std::endl;
        parameters->printParamConsole();
        std::cout << "----------------" << std::endl;

        clock_t start = clock();
        blRandomGeneratorPM::srand(10);

        // load image
        blImage* image = new blImage(p_inputImage);
        std::cout << "image->imageType() = " << image->imageType() << std::endl;
        if (image->imageType() == blImage::TypeInt2D || image->imageType() == blImage::TypeInt3D){
            blCastImageToFloat castFilter;
            castFilter.setInput(image);
            castFilter.run();
            image = castFilter.output();
        }

        // get process

        blMppFactory factory(parameters);
        blMppGeneratorShape* generator = factory.generator(parameters->getValueOfKey<string>("blMppGenerator", "blMppGeneratorShapeCircle"));
        blMppDataTerm* dataTerm = factory.dataTerm(parameters->getValueOfKey<string>("blMppDataTerm", "blMppDataTermBhattacharyya"));
        dataTerm->setImage(image);
        blMppInteraction* interaction = factory.interaction(parameters->getValueOfKey<string>("blMppInteraction", "blMppInteractionNoOverlap"));
        blMppAlgorithm* algorithm = factory.algorithm(generator, dataTerm, interaction, parameters->getValueOfKey<string>("blMppAlgorithm", "blMppAlgorithmMBCR"));

        // Init
        string birthMap = parameters->getValueOfKey<string>("birthMap", "");
        if ( birthMap != ""){
            generator->setBirthMap(new blImage(argv[5]));
        }
        else{
            generator->setBirthArea(image->imageSize());
        }

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
        output->save(p_outputImage, true);

        blMppResultRepresentation::saveShapeListToFile(shapes, p_outputTxtFile);
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
