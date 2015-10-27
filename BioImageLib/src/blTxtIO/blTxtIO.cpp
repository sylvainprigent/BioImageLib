/// \file blTxtIO.cpp
/// \brief blTxtIO class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#include "blTxtIO.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <blCore/blCast.h>

using namespace std;

// //////////////////////////////////////////// //
//                   blTxtIO
// //////////////////////////////////////////// //
void blTxtIO::SaveShapesIntoTxtFile(vector< vector<float> > Shapes, string filePath){

    ofstream file(filePath.c_str(), ios::out | ios::trunc);
    if(file)
    {
        if (Shapes.size() > 1){
            unsigned int ShapeSize = int(Shapes.at(0).size());
            for (unsigned int i = 0 ; i < Shapes.size() ; ++i){
                for (unsigned int j = 0 ; j < ShapeSize ; ++j){
                    file << Shapes[i][j] << "\t";
                }
                file << endl;
            }
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file!" << endl;
    }
}

void blTxtIO::SaveShapesIntoTxtFile(vector<vector< vector<float> > > Shapes, vector<string> ShapesTypeName, string filePath, bool useSingleFile){

    if (useSingleFile){
        ofstream file(filePath.c_str(), ios::out | ios::trunc);
        if(file)
        {
            for (unsigned int t = 0 ; t < Shapes.size() ; ++t){
                file << ShapesTypeName[t] << endl;
                if (Shapes[t].size() > 1){
                    unsigned int ShapeSize = int(Shapes[t][0].size());
                    for (unsigned int i = 0 ; i < Shapes[t].size() ; ++i){
                        for (unsigned int j = 0 ; j < ShapeSize ; ++j){
                            file << Shapes[t][i][j] << "\t";
                        }
                        file << endl;
                    }
                }
            }
            file.close();
        }
        else{
            cerr << "Impossible to open the file!" << endl;
        }
    }
    else{
        for (unsigned int t = 0 ; t < Shapes.size() ; ++t){
            SaveShapesIntoTxtFile(Shapes[t], filePath+ "_" + ShapesTypeName[t] + ".txt");
        }
    }
}

vector<vector<float> > blTxtIO::LoadShapeFromTxtFile(string filePath, int ShapeSize){

    ifstream file(filePath.c_str(), ios::in);
    int line = -1;
    vector<vector<float> > Shapes;
    vector<float> Shape; Shape.resize(ShapeSize);
    if(file)
    {
        while (!file.eof()){
            line++;
            for (int j = 0 ; j < ShapeSize ; ++j){
                file >> Shape[j];
            }
            Shapes.push_back(Shape);
        }
        file.close();
    }
    else{
        cerr << "IO::LoadShapeFromTxtFile -> Impossible to open the file " << filePath << "!" << endl;
    }
    return Shapes;
}

vector<vector<float> > blTxtIO::LoadShapeFromTxtFile(string filePath){
    ifstream file(filePath.c_str(), ios::in);
    //int line = -1;
    vector<vector<float> > Shapes;
    vector<float> Shape; //Shape.resize(ShapeSize);
    if(file)
    {
        string curentLine, readedword;
        double val;
        while(!file.eof()){
            // Read the line
            getline(file, curentLine);
            if (!curentLine.empty()){
                Shape.clear();
                // split the line
                stringstream stream(curentLine);
                while( getline(stream, readedword, '\t')){
                    //val = std::stod(readedword);
                    blCast::from_string(readedword, val);
                    Shape.push_back(val);
                }
                Shapes.push_back(Shape);
            }
        }
        file.close();
    }
    else{
        cerr << "IO::LoadShapeFromTxtFile -> Impossible to open the file " << filePath << "!" << endl;
    }
    return Shapes;
}
