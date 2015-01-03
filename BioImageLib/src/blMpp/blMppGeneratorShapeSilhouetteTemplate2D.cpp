/// \file blMppGeneratorShapeSilhouetteTemplate2D.cpp
/// \brief blMppGeneratorShapeSilhouetteTemplate2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppGeneratorShapeSilhouetteTemplate2D.h"
#include "blRand/blRandomGeneratorPM.h"
#include <blMath>

using namespace std;

blMppGeneratorShapeSilhouetteTemplate2D::blMppGeneratorShapeSilhouetteTemplate2D() : blMppGeneratorShape(){

}

blMppGeneratorShapeSilhouetteTemplate2D::blMppGeneratorShapeSilhouetteTemplate2D(blImage* birthMap)
    : blMppGeneratorShape(birthMap){
}

blMppGeneratorShapeSilhouetteTemplate2D::blMppGeneratorShapeSilhouetteTemplate2D(blImageSize size)
    : blMppGeneratorShape(size){
}

void blMppGeneratorShapeSilhouetteTemplate2D::initAttributs(){

    m_libraryFile = "";
    m_maxLength = 0;
}

blMppGeneratorShapeSilhouetteTemplate2D::~blMppGeneratorShapeSilhouetteTemplate2D(){

    for (int i = 0 ; i < m_shapeLibrary.size() ; ++i){
        delete m_shapeLibrary[i];
    }
}

// blProcess methods
std::string blMppGeneratorShapeSilhouetteTemplate2D::name(){
    return "blMppGeneratorShapeSilhouetteTemplate2D";
}


// setters parameters
void blMppGeneratorShapeSilhouetteTemplate2D::setSilhouetteLibraryFile(std::string file){
    m_libraryFile = file;
}


// ----------------------- Virtual methods for Shape management ------------
void blMppGeneratorShapeSilhouetteTemplate2D::initialize(){

    std::cout << "initialize" << std::endl;
    // load the library
    // 1- Open file
    m_maxLength = 0;
    ifstream file( m_libraryFile.c_str() );

    if ( file )
    {
        string line;
        string silhouette = "silhouette";
        string new_line = "new line";
        string readedword; int val;
        bool already_found_a_silhouette = false;
        blMppContainerSilhouette *curentSilhouette;
        blMppContainerSilhouetteLine curentLine;
        bool already_fill_line = false;
        while ( std::getline( file, line ) )
        {
            std::size_t found_silhouette = line.find(silhouette);
            std::size_t found_new_line = line.find(new_line);
            if (found_silhouette!=std::string::npos){
                if (already_found_a_silhouette){
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
            else{
                // get left pixel
                vector<int> interL;
                stringstream streamL(line);
                while( getline(streamL, readedword, '\t')){
                    blCast::from_string(readedword, val);
                    interL.push_back(val);
                }

                // get right pixel
                std::getline( file, line );
                vector<int> interR;
                stringstream streamR(line);
                while( getline(streamR, readedword, '\t')){
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


    std::cout << "shape library size = " <<  m_shapeLibrary.size() << std::endl;
    this->notify("shape library size = " + m_shapeLibrary.size());
}

void blMppGeneratorShapeSilhouetteTemplate2D::addSilhouette(blMppContainerSilhouette *silhouette){

    std::cout << "add silhouette to library" << std::endl;
    silhouette->calculateArea();
    int maxLength = silhouette->silhouetteMaxLength();
    silhouette->printConsole();
    if (maxLength > m_maxLength){
        m_maxLength = maxLength;
    }

    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    newShape->setSilhouette(silhouette);
    m_shapeLibrary.push_back(newShape);

    newShape->setCenter(blPixel(0,0));
    newShape->setParameters(m_shapeLibrary.size()-1 );
    newShape->setSilhouette(silhouette);
    newShape->setMaxDistanceToBarycenter(maxLength);
    if (m_calculateBorderPixels){
        std::cout << "conpute border pixels " << std::endl;
        newShape->computeBorderPixels(m_borderWidth);
        std::cout << "conpute border pixels done" << std::endl;
    }
    if (m_calculateNormals){
        newShape->computeNormalsBorder();
    }
    std::cout << "add silhouette to library end" << std::endl;
}

blMppShape* blMppGeneratorShapeSilhouetteTemplate2D::generateRandomUsingMask(){

    // randomly get a shape parameter set
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSilhouette* templateShape = m_shapeLibrary[pos];

    // randomly get center
    int x, y;
    int posMin = templateShape->maxDistanceToBarycenter() + m_borderWidth+1;
    int posMaxNl = m_birthArea.xSize()-1 - 2*posMin;
    int posMaxNc = m_birthArea.ySize()-1 - 2*posMin;
    bool stop = false;
    while (!stop){
        blPixel pixelIndex = getRandomPixelFromMask();
        if (pixelIndex.x() >= posMin &&  pixelIndex.x() <= posMaxNl && pixelIndex.y() >= posMin &&  pixelIndex.y() <= posMaxNc){
            x = pixelIndex.x();
            y = pixelIndex.y();
            stop = true;
        }
    }

    // generate the shape data
    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    calculateShapeData(blPixel(x,y), templateShape,newShape);
    return newShape;
}

blMppShape* blMppGeneratorShapeSilhouetteTemplate2D::generateRandomUsingArea(){

    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSilhouette* templateShape = m_shapeLibrary[pos];

    int posMin = templateShape->maxDistanceToBarycenter() + m_borderWidth+1;
    int rangeMaxNl = m_birthArea.xSize()-1 - 2*posMin;
    int rangeMaxNc = m_birthArea.ySize()-1 - 2*posMin;
    int x = int(blRandomGeneratorPM::rand()*float(rangeMaxNl) + float(posMin));
    int y = int(blRandomGeneratorPM::rand()*float(rangeMaxNc) + float(posMin));

    //std::cout << "normal before = " << templateShape->normals()->operator [](0)[0] << ", " << templateShape->normals()->operator [](0)[1] << std::endl;
    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    calculateShapeData(blPixel(x,y), templateShape, newShape);
    return newShape;
}

void blMppGeneratorShapeSilhouetteTemplate2D::calculateShapeData(blPixel centerPixel, blMppShapeSilhouette* templateShape, blMppShapeSilhouette *newShape){

    // parameters
    newShape->setCenter(centerPixel);
    newShape->setParameters(templateShape->position());
    newShape->setMaxDistanceToBarycenter(templateShape->maxDistanceToBarycenter());

    // silhouette
    newShape->setSilhouette(templateShape->silhouette()->translate(centerPixel));

    // border
    if (m_calculateBorderPixels){
        std::vector<blPixel>* translated = new std::vector<blPixel>();
        translatePixels(templateShape->borderPixels(), centerPixel,  translated);
        newShape->setBorderPixels(translated);
    }

    // normals
    if (m_calculateNormals){
        copyNormals(templateShape, newShape);
    }
}

void blMppGeneratorShapeSilhouetteTemplate2D::copyNormals(blMppShapeSilhouette* templateShape, blMppShapeSilhouette *newShape){
    // copy normals
    std::vector<std::vector<float> >* template_normals = templateShape->normalsBorder();
    std::vector<std::vector<float> >* new_normals = new std::vector<std::vector<float> >();
    new_normals->resize(template_normals->size());
    for (int i = 0 ; i < template_normals->size() ; ++i){
        std::vector<float> inter; inter.resize(2);
        inter[0] = template_normals->operator [](i)[0];
        inter[1] = template_normals->operator [](i)[1];
        new_normals->operator [](i) = inter;
    }
    newShape->setNormalsBorder(new_normals);
}

void blMppGeneratorShapeSilhouetteTemplate2D::translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated){

    translated->resize(pixels->size());
    std::vector<blPixel>::iterator itPixels;
    std::vector<blPixel>::iterator ittranslated;
    for (itPixels = pixels->begin(), ittranslated = translated->begin(); (itPixels != pixels->end()) && (ittranslated != translated->end()); ++itPixels, ++ittranslated) {
        *ittranslated = blPixel((*itPixels).x() + t.x(), (*itPixels).y() + t.y());
    }
}

int blMppGeneratorShapeSilhouetteTemplate2D::shapesMaxLength(){
    return m_maxLength;
}

// virtual
blMppShape* blMppGeneratorShapeSilhouetteTemplate2D::translate(blMppShape* shape){

    blMppShapeSilhouette* shapes = dynamic_cast<blMppShapeSilhouette*>(shape);

    int deltaX = int(blMath::randSign())*m_deltaTranslation;
    int deltaY = int(blMath::randSign())*m_deltaTranslation;
    int x = shapes->getCenter().x() + deltaX;
    int y = shapes->getCenter().y() + deltaY;


    // control if still inside image
    int posMin = shapes->maxDistanceToBarycenter() + m_borderWidth+1;
    int posMaxNl = m_birthArea.xSize()-1 - 2*posMin;
    int posMaxNc = m_birthArea.ySize()-1 - 2*posMin;

    if (x < posMin){
        x = posMin;
    }
    if (x > posMaxNl){
        x = posMaxNl;
    }
    if (y < posMin){
        y = posMin;
    }
    if (y > posMaxNc){
        y = posMaxNc;
    }

    // fill shape
    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    // copy parameters
    newShape->setCenter(blPixel(x,y));
    newShape->setParameters(shapes->position() );
    newShape->setMaxDistanceToBarycenter(shapes->maxDistanceToBarycenter());

    // translate silhouette
    newShape->setSilhouette(shapes->silhouette()->translate(blPixel(deltaX, deltaY)));
    // translate border
    if (m_calculateBorderPixels){
        vector<blPixel>* translated = new vector<blPixel>();
        translatePixels(shapes->borderPixels(), blPixel(deltaX, deltaY), translated);
        newShape->setBorderPixels(translated);
    }
    // copy normals
    if (m_calculateNormals){
        copyNormals(shapes, newShape);
    }

    return newShape;
}

blMppShape* blMppGeneratorShapeSilhouetteTemplate2D::rotate(blMppShape* shape){

    blMppShapeSilhouette* shapes = dynamic_cast<blMppShapeSilhouette*>(shape);

    // get a new random shape
    int pos = int(blRandomGeneratorPM::rand()*float(m_shapeLibrary.size()));
    blMppShapeSilhouette* templateShape = m_shapeLibrary[pos];

    int x = shapes->getCenter().x();
    int y = shapes->getCenter().y();

    // control if still inside image
    int posMin = templateShape->maxDistanceToBarycenter() + m_borderWidth+1;
    int posMaxNl = m_birthArea.xSize()-1 - 2*posMin;
    int posMaxNc = m_birthArea.ySize()-1 - 2*posMin;

    if (x < posMin){
        x = posMin;
    }
    if (x > posMaxNl){
        x = posMaxNl;
    }
    if (y < posMin){
        y = posMin;
    }
    if (y > posMaxNc){
        y = posMaxNc;
    }

    blMppShapeSilhouette *newShape = new blMppShapeSilhouette();
    calculateShapeData(blPixel(x,y), templateShape, newShape);
    return newShape;

}

blMppShape* blMppGeneratorShapeSilhouetteTemplate2D::resize(blMppShape* shape){

    return rotate(shape);
}
