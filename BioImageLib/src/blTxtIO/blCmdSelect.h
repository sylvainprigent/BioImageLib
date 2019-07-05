/// \file blCmdSelect.h
/// \brief blCmdSelect class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#pragma once

#include <string>
#include <vector>
#include "blTxtIOExport.h"

/// \class blCmdSelect
/// \brief class that define a container for unix shell single input
class BLTXTIO_EXPORT blCmdSelect{

public:
    blCmdSelect();

public:
    int count();
    void add(std::string name, std::string value);
    std::string name(int i);
    std::string value(int i);

protected:
    std::vector<std::string> m_names;
    std::vector<std::string> m_values;
};
