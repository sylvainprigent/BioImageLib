#include <string>
#include <vector>
#include <blTxtIO>
#include <blStatistics>
#include "blStatisticsTestConfig.h"

int main(int argc, char* argv[])
{

    // load inputs
    std::vector<double> xData = blCsvIO::loadArray(XDISTRIBUTION);
    std::vector<double> yData = blCsvIO::loadArray(YDISTRIBUTION);

    // Run
    double p_value;
    int error = blTtest::Ttest(xData, yData, p_value, "both");

    std::cout << "error = " << error << std::endl;
    std::cout << "p_value = " << p_value << std::endl;

    // real p-value = 0.00117777
    if (p_value > 0.00117778 || p_value < 0.00117776 ){
        return 1;
    }
    return 0;

}
