#include <iostream>
#include <string>
#include <vector>
#include <blTxtIO>
#include <blStatistics>


#include <time.h>
#include <cstdlib>
#include <ctime>

std::string usage(std::string name)
{
    return     "" + name + " <option(s)> "
            + "Options:\n"
            + "\t-h\t\tShow this help message\n"
            + "\t-x INPUT\tFirst distribution in a csv file\n"
            + "\t-y INPUT\tSecond distribution in a csv file\n"
            + "\t-tail INPUT\tTail (both, left, right) \n"
            + "\t-p OUTPUT\tStudent p-value\n";
}

int main(int argc, char* argv[])
{
    try{

        // 1- Parse I/O
        blCmdParser parser(argc, argv);
        parser.addArg("-x");
        parser.addArg("-y");
        parser.addArg("-tail");
        parser.addArg("-p");

        parser.setUsage(usage(argv[0]));
        blProcessParameters* parameters = parser.parse(5);

        parameters->printParamConsole();
        string xFile = parameters->getValueOfKeyString("-x", "");
        string yFile = parameters->getValueOfKeyString("-y", "");
        string tail = parameters->getValueOfKeyString("-tail", "");
        string pFile = parameters->getValueOfKeyString("-p", "");

        std::cout << "x = " << xFile << std::endl;
        std::cout << "y = " << yFile << std::endl;

        if (xFile == "" || yFile == ""){
            std::cout << "-x and -y are mandatory arguments";
            return 1;
        }

        // 2- Load data to vectors
        std::vector<double> xData = blCsvIO::loadArray(xFile);
        std::vector<double> yData = blCsvIO::loadArray(yFile);

        // 3- Run
        double p_value;
        int error = blTtest::Ttest(xData, yData, p_value, tail);

        std::cout << "error code = " << error << std::endl;
        std::cout << "p_value = " << p_value << std::endl;

        blCsvIO::saveNumber(pFile, p_value);

    }
    catch (blException ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
