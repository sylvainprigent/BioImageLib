/// \file blCmdParser2.h
/// \brief blCmdParser2 class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#pragma once

#include "blCmdParameterList.h"
#include "blTxtIOExport.h"

#include "blProcessParameters.h"

/// \class blCmdParser2
/// \brief class that define function to parse a unix shell commande
class BLTXTIO_EXPORT blCmdParser2{

public:
    blCmdParser2(int argc, char *argv[]);

public:
    // inputs
    void addParameter(std::string name, std::string description, std::string defaultValue, std::string type = blCmdParameter::TypeString , bool isAdvanced=false);
    void addInputImage(std::string name, std::string description);
    void addOutputImage(std::string name, std::string Description);
    void setMan(std::string man);

    // parse
    void parse(int minArgc = 0);

    // outputs
    bool getParameterBool(std::string name);
    int getParameterInt(std::string name);
    float getParameterFloat(std::string name);
    double getParameterDouble(std::string name);
    std::string getParameterString(std::string name);
    const char* getParameterChar(std::string name);
    std::string getImageUrl(std::string name);

    blProcessParameters* toProcessParameters();

public:
    void print();

private:
    void setArgValue(std::string argName, std::string value);

private:
    int m_argc;
    char **m_argv;

private:
    blCmdParameterList m_args;
    std::string m_man;
};
