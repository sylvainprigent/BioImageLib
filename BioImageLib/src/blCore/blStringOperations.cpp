/// \file blStringOperations.cpp
/// \brief blStringOperations class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blStringOperations.h"
#include <sstream>

using namespace std;

string blStringOperations::getFileNameFromPath(string file){

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char slash='\\';
#else
    char slash='/';
#endif

    stringstream stream(file);
    string readedword;
    while( getline(stream, readedword, slash)){
    }
    return readedword;
}
