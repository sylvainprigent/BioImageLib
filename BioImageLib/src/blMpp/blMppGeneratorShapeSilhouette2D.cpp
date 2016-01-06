/// \file blMppGeneratorShapeSilhouette2D.cpp
/// \brief blMppGeneratorShapeSilhouette2D class
/// \author Emmanuelle Poulain (Emmanuelle.Poulain@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSilhouette2D.h"
#include <blMath>
#include "blRand/blRandomGeneratorPM.h"

using namespace std;

blMppGeneratorShapeSilhouette2D::blMppGeneratorShapeSilhouette2D() : blMppGeneratorShape(){
    initAttributs();
}

blMppGeneratorShapeSilhouette2D::blMppGeneratorShapeSilhouette2D(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
    initAttributs();
}

blMppGeneratorShapeSilhouette2D::blMppGeneratorShapeSilhouette2D(blImageSize size)
    : blMppGeneratorShape(size){
    initAttributs();
}

void blMppGeneratorShapeSilhouette2D::initAttributs(){
    m_libraryDir = "";
    m_nbFiles = 0;
    m_maxLength = 0;
}

blMppGeneratorShapeSilhouette2D::~blMppGeneratorShapeSilhouette2D(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeSilhouette2D::name(){
    return "blMppGeneratorShapeSilhouette2D";
}

// setters parameters
void blMppGeneratorShapeSilhouette2D::setSilhouetteLibraryFile(std::string file){
    m_libraryDir = file;
}

void blMppGeneratorShapeSilhouette2D::setNumberOfFile(int nbFiles){
    m_nbFiles = nbFiles;
}

void blMppGeneratorShapeSilhouette2D::setLibraryDirectory(string libraryDir){
    m_libraryDir = libraryDir;
}


// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeSilhouette2D::initialize(){

    loadGlobalPropertiesFiles();
    for (int indFile = 0 ; indFile < m_nbFiles ; ++indFile){//initialize the configuration
        loadSilhouettes(indFile+1);
    }
    cout << "taille librairie"<<m_shapeLibrary.size() << endl;
}

void blMppGeneratorShapeSilhouette2D::loadGlobalPropertiesFiles(){
    string line;
    string way;
    way = m_libraryDir+"/listObj0";
    std::cout <<way.c_str()<< endl;
    ifstream file( way.c_str());
    int distance_maximale;
    string distance = "distance";
    string maximale;
    if ( file )
    {
        while(getline(file,line)){
            std::size_t found_distance = line.find(distance);

            if(found_distance!=std::string::npos){
                stringstream streamD(line);
                streamD >> distance >> maximale >> distance_maximale;
            }
        }
    }
    else{
        throw( blException("blMppGeneratorShapeSilhouette2D: attempt to access a file which doesn't exist"));
    }
    m_maxLength=distance_maximale;
}

void blMppGeneratorShapeSilhouette2D::loadSilhouettes(int indFichier){

    //std::cout << "initialize" << std::endl;
    stringstream indFichierS ;

    indFichierS << indFichier;
    string s(indFichierS.str());
    string libraryFinder2("listObj"+s);
    string way;
    way = m_libraryDir+"/"+libraryFinder2;
    std::cout << way << std::endl;
    ifstream file( way.c_str());
    if ( file )
    {
        string line;
        string silhouette = "silhouette";
        string new_line = "new line";
        string rayon = "rayon";
        string readedword; int val;
        bool already_found_a_silhouette = false;
        blMppContainerSilhouette *curentSilhouette;
        blMppContainerSilhouetteLine curentLine;
        bool already_fill_line = false;

        while ( std::getline( file, line ) )
        {
            //indline = indline + 1;
            //cout << indline<<endl;
            std::size_t found_silhouette = line.find(silhouette);
            std::size_t found_new_line = line.find(new_line);
            std::size_t found_rayon = line.find(rayon);
            if (found_silhouette!=std::string::npos){
                if (already_found_a_silhouette){
                    curentSilhouette->addLine(curentLine);
                    curentLine.clear();
                    already_fill_line = false;
                    addSilhouette(curentSilhouette);
                }
                curentSilhouette = new blMppContainerSilhouette();
                already_found_a_silhouette = true;
            }
            else if(found_new_line!=std::string::npos){
                if(already_fill_line){
                    curentSilhouette->addLine(curentLine);
                }
                curentLine.clear();
                already_fill_line = true;
            }
            else if (found_rayon!=std::string::npos){
            //    remove this test
            }
            else{
                // get left pixel
                vector<int> interL;
                stringstream streamL(line);
                while( getline(streamL, readedword, '\t')){
                    //val = std::stoi(readedword);
                    blCast::from_string(readedword, val);
                    interL.push_back(val);
                }

                // get right pixel
                std::getline( file, line );
                vector<int> interR;
                stringstream streamR(line);
                while( getline(streamR, readedword, '\t')){
                    //val = std::stoi(readedword);
                    blCast::from_string(readedword, val);
                    interR.push_back(val);
                }

                curentLine.addPointPair(blPixel(interL[0], interL[1], interL[2]), blPixel(interR[0], interR[1], interR[2]));
            }

        }
        curentSilhouette->addLine(curentLine);
        addSilhouette(curentSilhouette);
        file.close();
    }
    else{
        throw( blException("blMppGeneratorShapeSilhouette2D: attempt to access a silhouette file that doesn't exist"));
    }
}

void blMppGeneratorShapeSilhouette2D::addSilhouette(blMppContainerSilhouette *silhouette){

    //std::cout << "add silhouette to library" << std::endl;
    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    newShape->setSilhouette(silhouette);
    newShape->silhouette()->calculateArea();
    newShape->computeInsidePixels();
    newShape->computeBorderPixels(m_borderWidth);
    m_shapeLibrary.push_back(newShape);

}

blMppShape* blMppGeneratorShapeSilhouette2D::generateRandomUsingMask(){

    throw blException("blMppGeneratorShapeSilhouette2D cannot use birth mask");
    return NULL;
}

blMppShape* blMppGeneratorShapeSilhouette2D::generateRandomUsingArea(){

    // select a shape in the library
    float randVal = blRandomGeneratorPM::rand();
    int pos = int(randVal*float(m_shapeLibrary.size()));

    //std::cout << "pos = " << pos << std::endl;
    blMppShape *copiedShape = m_shapeLibrary[pos]->copy();

    //std::cout << "copied shape" << std::endl;

    return copiedShape;
}

int blMppGeneratorShapeSilhouette2D::shapesMaxLength(){
    return m_maxLength;
}

// virtual
blMppShape* blMppGeneratorShapeSilhouette2D::translate(blMppShape*){

    throw( blException("blMppGeneratorShapeSilhouette2D: cannot generate translation"));
    return NULL;
}

blMppShape* blMppGeneratorShapeSilhouette2D::rotate(blMppShape*){

    throw( blException("blMppGeneratorShapeSilhouette2D: cannot generate rotation"));
    return NULL;
}

blMppShape* blMppGeneratorShapeSilhouette2D::resize(blMppShape*){

    throw( blException("blMppGeneratorShapeSilhouette2D: cannot generate resize"));
    return NULL;
}
