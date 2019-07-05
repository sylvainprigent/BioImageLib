/// \file blCmdParameter.h
/// \brief blCmdParameter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2018

#pragma once

#include <string>
#include <vector>
#include "blTxtIOExport.h"

#include "blCmdSelect.h"

/// \class blCmdParameter
/// \brief class that define a container for unix shell single input
class BLTXTIO_EXPORT blCmdParameter{

public:
    static const std::string TypeImage; 
    static const std::string TypeNumber;
    static const std::string TypeString;
    static const std::string TypeSelect;
    static const std::string TypeBoolean;
    static const std::string TypeHidden;

    static const std::string IOInput;
    static const std::string IOOutput;

public:
    blCmdParameter(std::string name, std::string description="");

public:
    std::string getName();
    std::string getValueSuffix();
    std::string getDescription();
    std::string getValue();
    std::string getType();
    std::string getIO();
    std::string getDefault();
    bool getIsAdvanced();
    blCmdSelect getSelectInfo();

public:
    void setDescription(std::string description);
    void setType(std::string type);
    void setIO(std::string io);
    void setValue(std::string value);
    void setDefault(std::string value);
    void setIsAdvanced(bool value);
    void setValueSuffix(std::string suffix);
    void setSelectInfo(blCmdSelect select);

protected:
    std::string m_name;
    std::string m_description;
    std::string m_value;
    std::string m_type; // image, number, string, choice
    std::string m_io; // input | output
    std::string m_defaultValue;
    blCmdSelect m_selectInfo; // for select
    std::string m_valueSuffix;
    bool m_isAdvanced;
};
