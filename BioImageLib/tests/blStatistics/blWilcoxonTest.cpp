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
    blWilcoxonTest wtest;
    wtest.setDistributions(xData, yData);
    wtest.run();

    //std::cout << "rank = " << wtest.getRank() << std::endl;
    //std::cout << "p_value = " << wtest.getPvalue() << std::endl;

    // real p-value = 0.00299796
    if (wtest.getPvalue() > 0.00299797 || wtest.getPvalue() < 0.00299795 ){
        return 1;
    }
    return 0;

}
