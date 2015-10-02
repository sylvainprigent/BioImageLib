/// \file blProcessParameters.h
/// \brief blProcessParameters class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#ifndef bl_PARAMMANAGER_H
#define bl_PARAMMANAGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <typeinfo>
#include <vector>

#include <blCore/blException.h>
#include <blCore/blCast.h>

#include "blTxtIOExport.h"

/// \class blConvert
/// \brief class that convert string param to variou types
/// This class declare static functions
class BLTXTIO_EXPORT blConvert{
public:
    /// \fn static std::string T_to_string(T const &val)
    /// \brief Convert T, which should be a primitive, to a std::string
    /// \param[in] T value to convert
    /// \return convertion to std::string
    template <typename T>
    static std::string T_to_string(T const &val){
        std::ostringstream ostr;
        ostr << val;

        return ostr.str();
    }

    /// \fn static T string_to_T(std::string const &val)
    /// \brief Convert a std::string to T.
    /// \param[in] std::string to convert
    /// \return Convertion to T
    template <typename T>
    static T string_to_T(std::string const &val){
        std::istringstream istr(val);
        T returnVal;
        if (!(istr >> returnVal))
            throw blException(("CFG: Not a valid " + ((std::string)typeid(T).name()) + " received for " + val+ "!\n").c_str()) ;

        return returnVal;
    }

    /// \fn static T string_to_T(std::string const &val)
    /// \brief Treat the particular case of a std::string.
    /// \param[in] std::string to convert
    /// \return Convertion to std::string
    static std::string string_to_T(std::string const &val);
};


/// \class blProcessParameters
/// \brief class BLCORE_EXPORT that allows to read/write param into a file
/// To use it for reading: call the constructor 'ParamFile(fName)',
/// then 'load()' to load parameters, and 'getValueOfKey' to read any parameter individually.

/// To use it for writing: call the constructor 'ParamFile(fName)',
/// then call 'load()' only if the file exist, then set param with 'setValueOfKey'
/// and finally call 'updateFile()' to save modification into the file.

class BLTXTIO_EXPORT blProcessParameters{

private:
    std::map<std::string, std::string> contents;
    std::string fName;
    bool m_alreadyLoaded;

    void removeComment(std::string &line) const;
    bool onlyWhitespace(const std::string &line) const ;
    bool validLine(const std::string &line) const;
    void extractKey(std::string &key, size_t const &sepPos, const std::string &line) const;
    void extractValue(std::string &value, size_t const &sepPos, const std::string &line) const;
    void extractContents(const std::string &line);
    void parseLine(const std::string &line, size_t const lineNo);
    void ExtractKeys();


public:
    /// \fn blProcessParameters();
    /// Default Constructor
    blProcessParameters();

    /// \fn blProcessParameters(const std::string &fName);
    /// Constructor using a param file
    /// \param[in] fName Path of the parameter file.
    blProcessParameters(const std::string &fName);

    /// \fn blProcessParameters(const std::string paramList, const std::string separator);
    /// \brief Constructor to use the  parameters in a string (with the same content than a file)
    /// \param[in] paramList String containing the parameters
    /// \param[in] separator char used to separate the parameters in paramList
    blProcessParameters(const std::string paramList, const std::string separator);
    /// \fn void UpdateParameters(const std::string paramList, const std::string separator);
    /// \brief Change all the parameters
    /// \param[in] paramList String containing the parameters
    /// \param[in] separator char used to separate the parameters in paramList
    void UpdateParameters(const std::string paramList, const std::string separator);

    /// \fn void load();
    /// \brief Method that read the file 'fName' to load the contained parameters
    void load();

    /// \fn void printParamConsole();
    /// \brief Method that print all the parameters in the console. It prints a
    /// line per parameter like: key = param
    void printParamConsole();

    /// \fn bool keyExists(const std::string &key) const;
    /// \param[in] key Key to test
    /// \brief Test if a key exists
    /// \return true if the key exist, false otherwise
    bool keyExists(const std::string &key) const;

    /// \fn ValueType getValueOfKey(const std::string &key, ValueType const &defaultValue = ValueType()) const
    /// \brief Method that get the value for a given key
    /// \param[in] key Key
    /// \param [in] defaultValue default value
    /// \return value for the key or default value if the key does not exists
    template <typename ValueType>
    ValueType getValueOfKey(const std::string &key, ValueType const &defaultValue = ValueType()) const{
        if (!keyExists(key))
            return defaultValue;

        return blConvert::string_to_T<ValueType>(contents.find(key)->second);
    }

    std::string getValueOfKeyString(const std::string& key, const std::string &defaultValue){
        if (!keyExists(key))
            return defaultValue;

        return contents.find(key)->second;
    }

    /// \fn std::vector<std::string> getVectorStringValuesOfKey(const std::string &key, std::vector<std::string> const &defaultValue) const;
    /// \brief Method that get a vector of string for a given key
    /// \param[in] key Key
    /// \param [in] defaultValue default value
    /// \return value for the key or default value if the key does not exists
    std::vector<std::string> getVectorStringValuesOfKey(const std::string &key, std::vector<std::string> const &defaultValue) const;

    /// \fn template <typename ValueType> std::vector<ValueType> getVectorValuesOfKey(const std::string &key, std::vector<ValueType> const &defaultValue) const;
    /// \brief Method that get a vector of templated values for a given key
    /// \param[in] key Key
    /// \param [in] defaultValue default value
    /// \return value for the key or default value if the key does not exists
    template <typename ValueType>
    std::vector<ValueType> getVectorValuesOfKey(const std::string &key, std::vector<ValueType> const &defaultValue) const{
        if (!keyExists(key))
            return defaultValue;

        // parse the vector
        std::string vectorData = contents.find(key)->second;

        std::vector<ValueType> vectorValueType;
        std::stringstream stream(vectorData);
        ValueType val;
        std::string readedword;
        while( getline(stream, readedword, ',')){
          blCast::from_string(readedword, val);
          vectorValueType.push_back(val);
        }
        return vectorValueType;
    }

    /// \fn void setValueOfKey(const std::string &key, const std::string &value);
    /// \brief Method that set a value to the key 'key'. If the key does not exists it is created
    /// \param[in] key Key
    /// \param[in] value Value converted to std::string
    void setValueOfKey(const std::string &key, const std::string &value);

    /// \fn template <typename ValueType> void setValueOfKey(const std::string &key, const ValueType &value)
    /// \brief set a key
    /// \param[in] key Key
    /// \param[in] value value
    template <typename ValueType>
    void setValueOfKey(const std::string &key, const ValueType &value){
        if (keyExists(key)) {
            contents.erase(contents.find(key));
        }
        std::string valueStr = blConvert::T_to_string<ValueType>(value);
        contents.insert(std::pair<std::string, std::string>(key, valueStr));
    }

    /// \fn void updateFile();
    /// \brief Method that save the parameters in the file.
    void updateFile();
};

/// \class blFileParser
/// \brief class that define function to parse file
class blFileParser{
public:
    /// \fn static vector<string> ParseFileLineByLine(string fileAdress);
    /// \brief Function that read a file line by line and that store all the lines in a vector of string
    /// \param[in] fileAdress Adress of the file to parse
    /// \return vector containing all the lines.
    static std::vector<std::string> ParseFileLineByLine(const std::string fileAdress);
};


#endif
