#include <iostream>
#include <string>
#include <vector>
#include <blTxtIO>
#include <blCastStacks>
#include <blFiltering>
#include "blRand/blRandomGeneratorPM.h"


#include <time.h>
#include <cstdlib>
#include <ctime>

std::string usage(std::string name)
{
    return     "" + name + " <option(s)> "
            + "Options:\n"
            + "\t-h\t\tShow this help message\n"
            + "\t-inputImage INPUT\tInput image\n"
            + "\t-domainsigma PARAM\tDomain sigma\n"
            + "\t-rangeSigma PARAM\tRange sigma\n"
            + "\t-outputImage OUTPUT\tOutput filtered image\n";
}

int main(int argc, char* argv[])
{
    try{

        // 1- Parse I/O
        blCmdParser parser(argc, argv);
        parser.addArg("-inputImage");
        parser.addArg("-domainsigma");
        parser.addArg("-rangeSigma");
        parser.addArg("-outputImage");

        parser.setUsage(usage(argv[0]));
        blProcessParameters* parameters = parser.parse(4);

        parameters->printParamConsole();
        string inputImage = parameters->getValueOfKeyString("-inputImage", "");
        string outputImage = parameters->getValueOfKeyString("-outputImage", "");
        float p_domainSigma = parameters->getValueOfKey<float>("-domainsigma", 2);
        float p_rangeSigma = parameters->getValueOfKey<float>("-rangesigma", 2);

        std::cout << "inputImage = " << inputImage << std::endl;
        std::cout << "outputImage = " << outputImage << std::endl;
        std::cout << "domainsigma = " << p_domainSigma << std::endl;
        std::cout << "rangesigma = " << p_rangeSigma << std::endl;

        if (inputImage == "" || outputImage == ""){
            std::cout << "-inputImage and -outputImage are mandatory arguments";
            return 1;
        }

        // 2- Run Filter
        blImage* image = new blImage(inputImage);
        blBilateralFilter filter;
        filter.setInput(image);
        filter.setDomainSigma(p_domainSigma);
        filter.setRangeSigma(p_rangeSigma);
        filter.run();
        blImage* filteredImage = filter.output();
        filteredImage->save(outputImage);

        delete image;
        delete filteredImage;
    }
    catch (blException ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
