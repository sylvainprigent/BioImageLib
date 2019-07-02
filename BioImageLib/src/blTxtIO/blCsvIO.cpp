/// \file blTxtIO.cpp
/// \brief blTxtIO class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#include "blCsvIO.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <blCore/blCast.h>

using namespace std;

// //////////////////////////////////////////// //
//                   blCsvIO
// //////////////////////////////////////////// //
std::vector<double> blCsvIO::loadArray(std::string filePath, std::string separator){

    ifstream file(filePath.c_str(), ios::in);
    vector<double> data;

    std::string line;
    std::string buf;
    if (std::getline(file, line))
    {
        int pos = 0;
        std::string token;
        line += ",";
        while ((pos = line.find(separator)) != std::string::npos) {
            token = line.substr(0, pos);
            //std::cout << token << std::endl;
            data.push_back(atof(token.c_str()));
            line.erase(0, pos + separator.length());
        }
    }

    return data;
}

void blCsvIO::saveNumber(std::string filePath, double number){

    ofstream file(filePath.c_str(), ios::out | ios::trunc);
    if(file)
    {
        file << number;
        file.close();
    }
}
