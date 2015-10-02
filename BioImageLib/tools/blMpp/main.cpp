#include <blMpp>
#include <blCore>

#include <time.h>
#include <cstdlib>
#include <ctime>

#include "itkBinaryThresholdImageFilter.h"

#include <blMpp>
#include "blMppFactories.h"
#include <blCastStacks>
#include "blRand/blRandomGeneratorPM.h"
using namespace std;

int main(int argc, char *argv[]){

    // --------------------- Get inputs --------------------------//
    if (argc < 5){
        cout << "Usage: need 4 input parameters" << endl;
        cout << "\t 1- Input Image (2D images)" << endl;
        cout << "\t 2- Parameter file (.txt)" << endl;
        cout << "\t 3- Output Image" << endl;
        cout << "\t 4- Output Txt File" << endl;
        cout << "\t 5- Birth mask (optional)" << endl;
        cout << "\t 6- Representation Image (optional)" << endl;
        return 1;
    }
    string inputImage = argv[1];
    string parameterFile = argv[2];
    string outputImage = argv[3];
    string outputTxtFile = argv[4];

    // -------------------- Get the input frame list ---------------------------

    clock_t start = clock();

    //time_t timeVal= time( NULL );
    //srand( timeVal );
    //srand( 10 ); // initialize the random
    blRandomGeneratorPM::srand(10);

    try{
        // load parameters
        blProcessParameters* parameters = new blProcessParameters(parameterFile);
        parameters->load();
        parameters->printParamConsole();

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
        if ( argc >= 6){
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

        blImage* output = NULL;
        if ( argc == 7){
            std::cout << "Save the representation image: main" << std::endl;
            blImage* imageRepresentation = new blImage(argv[6]);
            output = blMppResultRepresentation::imageRepresentation(shapes, "border", imageRepresentation, useRandColorb);
            output->save(outputImage, true);
        }
        else{
            output = blMppResultRepresentation::imageRepresentation(shapes, "border", image, useRandColorb);
            output->save(outputImage, true);
        }

        blMppResultRepresentation::saveShapeListToFile(shapes, outputTxtFile);
        //blMppResultRepresentation::saveShapeListToFileInsidePixels(shapes);
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
    }
    catch (blException ex){
        cout << ex.what() << endl;
        return 1;
    }
    printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}
