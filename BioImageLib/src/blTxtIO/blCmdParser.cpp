/// \file blCmdParser.cpp
/// \brief blCmdParser class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blCmdParser.h"

blCmdParser::blCmdParser(int argc, char *argv[]){
    m_argc = argc;
    m_argv = argv;
}


void blCmdParser::addArg(std::string arg){
    m_args.push_back(arg);
}

void blCmdParser::setUsage(std::string usage){
    m_usage = usage;
}

// outputs
blProcessParameters* blCmdParser::parse(int minArgc){

    blProcessParameters* parameters = new blProcessParameters;
    for (int i = 1; i < m_argc; ++i) {
        std::string arg = m_argv[i];
        // help
        if ((arg == "-h") || m_argc <= minArgc) {
            throw blException(m_usage.c_str());
        }

        // arg
        for (int j = 0 ; j < m_args.size() ; ++j){
            if ((arg == m_args[j])) {
                i = this->parseArg(m_args[j], parameters, i);
            }
        }
    }
    return parameters;
}

int blCmdParser::parseArg(std::string arg, blProcessParameters* parameters, int i){
    if (i + 1 < m_argc) {
        i++;
        parameters->setValueOfKey(arg, m_argv[i]);
    }
    else {
        std::cerr << arg + " option requires one argument." << std::endl;
    }
    return i;
}
