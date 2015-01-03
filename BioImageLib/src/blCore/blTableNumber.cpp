/// \file blTableNumber.cpp
/// \brief blTableNumber class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blTableNumber.h"
#include "blCast.h"
#include "blException.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

blTableNumber::blTableNumber(){

}

blTableNumber::~blTableNumber(){

}

std::string blTableNumber::name(){
    return "blTableNumber";
}

void blTableNumber::setData(const std::vector<std::vector<float> > &data){
    m_data = data;
}

const std::vector<std::vector< float > >& blTableNumber::data(){
    return m_data;
}


void blTableNumber::setTitle(std::string title){
    m_title = title;
}

std::string blTableNumber::title(){
    return m_title;
}

void blTableNumber::setHeaders(const std::vector<std::string >& headers){
    m_headers = headers;
}

const std::vector<std::string >& blTableNumber::headers(){
    return m_headers;
}

void blTableNumber::load(std::string filePath){
    ifstream file(filePath.c_str(), ios::in);

    m_data.clear();
    m_headers.clear();


    vector<float> Shape;
    std::string curentLine, readedword;
    double val;
    if(file)
    {
        // title
        getline(file, curentLine);
        m_title = curentLine;

        // headers
        getline(file, curentLine);
        stringstream stream(curentLine);
        while( getline(stream, readedword, '\t')){
            m_headers.push_back(readedword);
        }

        // data
        while(!file.eof()){
            // Read the line
            getline(file, curentLine);
            if (!curentLine.empty()){
                Shape.clear();
                // split the line
                stringstream stream(curentLine);
                while( getline(stream, readedword, '\t')){
                    blCast::from_string(readedword, val);
                    Shape.push_back(val);
                }
                m_data.push_back(Shape);
            }
        }
        file.close();
    }
    else{
        cerr << "blTableNumber -> Impossible to open the file " << filePath << "!" << endl;
    }
}

void blTableNumber::save(std::string filePath){
    ofstream file(filePath.c_str(), ios::out | ios::trunc);
    if(file)
    {
        std::cout << "title = " << m_title << std::endl;
        file << m_title << endl;
        std::cout << "m_headers = " << m_headers.size()  << std::endl;
        for (int i = 0 ; i < m_headers.size() ; ++i){
            std::cout << ", " << m_headers[i] << std::endl;
            file << m_headers[i] << "\t";
        }
        file << endl;


        if (m_data.size() >= 1){
            std::cout << "m_headers = " << m_data.size()  << std::endl;
            for (unsigned int i = 0 ; i < m_data.size() ; ++i){
                for (unsigned int j = 0 ; j < m_data[i].size() ; ++j){
                    file << m_data[i][j] << "\t";
                }
                file << endl;
            }
        }
        file.close();
    }
    else{
        throw blException("blTableNumber -> Impossible to open the file");
    }
}

void blTableNumber::printConsole(){
    std::cout << "blTableNumber: " << m_title << std::endl;
    for (int i = 0 ; i < m_headers.size() ; ++i){
        std::cout << m_headers[i] << "\t";
    }
    std::cout << std::endl;
    for (int i = 0 ; i < m_data.size() ; ++i){
        for (int j = 0 ; j < m_data[i].size() ; ++j){
            std::cout << m_data[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
