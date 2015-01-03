/// \file blProcessParameters.cpp
/// \brief blProcessParameters class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013


#include "blProcessParameters.h"


/******************************************************************/
//                      class ConfigFile
/*****************************************************************/

std::string blConvert::string_to_T(std::string const &val)
{
    return val;
}

/******************************************************************/
//                      class blProcessParameters
/*****************************************************************/

void blProcessParameters::removeComment(std::string &line) const
{
    if (line.find(';') != line.npos)
        line.erase(line.find(';'));
}

bool blProcessParameters::onlyWhitespace(const std::string &line) const
{
    return (line.find_first_not_of(' ') == line.npos);
}

bool blProcessParameters::validLine(const std::string &line) const
{
    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    if (temp[0] == '=')
        return false;

    for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
        if (temp[i] != ' ')
            return true;

    return false;
}

void blProcessParameters::extractKey(std::string &key, size_t const &sepPos, const std::string &line) const
{
    key = line.substr(0, sepPos);
    if (key.find('\t') != line.npos || key.find(' ') != line.npos)
        key.erase(key.find_first_of("\t "));
}

void blProcessParameters::extractValue(std::string &value, size_t const &sepPos, const std::string &line) const
{
    value = line.substr(sepPos + 1);
    value.erase(0, value.find_first_not_of("\t "));
    value.erase(value.find_last_not_of("\t ") + 1);
}

void blProcessParameters::extractContents(const std::string &line)
{
    std::string temp = line;
    // Erase leading whitespace from the line.
    temp.erase(0, temp.find_first_not_of("\t "));
    size_t sepPos = temp.find('=');

    std::string key, value;
    extractKey(key, sepPos, temp);
    extractValue(value, sepPos, temp);

    if (!keyExists(key)){
		//std::cout << "curent key = "<< key << std::endl;
        contents.insert(std::pair<std::string, std::string>(key, value));
	}
    else
        throw blException(("CFG: Can only have unique key names for key " + key + " !\n").c_str());
}

// lineNo = the current line number in the file.
// line = the current line, with comments removed.
void blProcessParameters::parseLine(const std::string &line, size_t const lineNo)
{
    if (line.find('=') == line.npos)
        throw blException(("CFG: Couldn't find separator on line: " + blConvert::T_to_string(lineNo) + "\n").c_str());

    if (!validLine(line))
        throw blException(("CFG: Bad format for line: " + blConvert::T_to_string(lineNo) + "\n" + line + "\n").c_str());

    extractContents(line);
}

void blProcessParameters::ExtractKeys()
{
    std::ifstream file;
    file.open(fName.c_str());
    if (!file)
        throw blException(("CFG: File " + fName + " couldn't be found!\n").c_str());

    std::string line;
    size_t lineNo = 0;
    while (std::getline(file, line))
    {
        lineNo++;
        std::string temp = line;

        if (temp.empty())
            continue;

        removeComment(temp);
        if (onlyWhitespace(temp))
            continue;

        this->parseLine(temp, lineNo);
    }

    file.close();
}

blProcessParameters::blProcessParameters()
{
  this->fName = "";
  m_alreadyLoaded = true;
}


blProcessParameters::blProcessParameters(const std::string &fName)
{
    this->fName = fName;
    m_alreadyLoaded = false;
}

blProcessParameters::blProcessParameters(const std::string paramList, const std::string separator){
    UpdateParameters(paramList, separator);
}

void blProcessParameters::UpdateParameters(const std::string paramList, const std::string separator){

    std::string line;
    size_t lineNo = 0;
    std::stringstream stream(paramList);
    while( getline(stream, line, '\n') )
    {
        lineNo++;
        std::string temp = line;

        if (temp.empty())
            continue;

        removeComment(temp);
        if (onlyWhitespace(temp))
            continue;

        this->parseLine(temp, lineNo);
    }

    //for( std::map<std::string,std::string>::iterator ii=contents.begin(); ii!=contents.end(); ++ii)
    //{
    //       std::cout << (*ii).first << ": " << (*ii).second << std::endl;
    //}
    m_alreadyLoaded = true;
}

void blProcessParameters::load(){
    if (!m_alreadyLoaded){
        ExtractKeys();
        //for( std::map<std::string,std::string>::iterator ii=contents.begin(); ii!=contents.end(); ++ii)
        //{
        //       std::cout << (*ii).first << ": " << (*ii).second << std::endl;
        //}
        m_alreadyLoaded = true;
	}
}

bool blProcessParameters::keyExists(const std::string &key) const
{
    return contents.find(key) != contents.end();
}

void blProcessParameters::setValueOfKey(const std::string &key, const std::string &value){

    if (keyExists(key)) {
        contents.erase(contents.find(key));
    }
    contents.insert(std::pair<std::string, std::string>(key, value));
}

void blProcessParameters::updateFile(){
    std::ofstream file;
    file.open(fName.c_str());
    if (!file)
        throw blException(("CFG: File " + fName + " couldn't be found for writing!\n").c_str());

    std::map<std::string,std::string>::iterator iter;
    for( iter = contents.begin(); iter != contents.end(); iter++ ) {
      file <<  iter->first << " = " << iter->second << std::endl;
    }

    file.close();
}

void blProcessParameters::printParamConsole(){
    std::map<std::string,std::string>::iterator iter;
    for( iter = contents.begin(); iter != contents.end(); iter++ ) {
      std::cout <<  iter->first << " = " << iter->second << std::endl;
    }
}

std::vector<std::string> blProcessParameters::getVectorStringValuesOfKey(const std::string &key, std::vector<std::string> const &defaultValue) const{
    if (!keyExists(key))
        return defaultValue;

    // parse the vector
    std::string vectorData = contents.find(key)->second;
    std::vector<std::string> vectorString;
    std::stringstream stream(vectorData);
    std::string readedword;
    while( getline(stream, readedword, ',')){
      vectorString.push_back(readedword);
    }
    return vectorString;
}


// //////////////////////////////////////////////////////////// //
//                      File Parser
// //////////////////////////////////////////////////////////// //
std::vector<std::string> blFileParser::ParseFileLineByLine(const std::string fileAdress){

    std::vector<std::string> lines;

    std::ifstream file;
    file.open(fileAdress.c_str());
    if (!file)
        throw blException(("CFG: File " + fileAdress + " couldn't be found!\n").c_str());

    std::string line;
    while (std::getline(file, line))
    {
        std::string temp = line;
        if (!temp.empty()){
            lines.push_back(temp);
        }
    }
    file.close();

    return lines;
}
