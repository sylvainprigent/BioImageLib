/// \file blCmdParser.h
/// \brief blCmdParser class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#ifndef bl_CMDPARSER_H
#define bl_CMDPARSER_H



#include "blTxtIOExport.h"
#include "blProcessParameters.h"

/// \class blCmdParser
/// \brief class that define function to parse a shell commande
class BLTXTIO_EXPORT blCmdParser{

public:
    blCmdParser(int argc, char *argv[]);

public:
    // inputs
    void addArg(std::string arg);
    void setUsage(std::string usage);

    // outputs
    blProcessParameters* parse(int minArgc = 0);
    blProcessParameters* pairParse();

private:
    int parseArg(std::string arg, blProcessParameters* parameters, int i);

private:
    int m_argc;
    char **m_argv;

private:
    std::vector<std::string> m_args;
    std::string m_usage;
};


#endif
