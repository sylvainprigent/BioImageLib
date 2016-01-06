/// \file blMppShape2D.cpp
/// \brief blMppShape2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShape2D.h"
#include <iostream>

using namespace std;

blMppShape2D::blMppShape2D(){
    m_silhouette = NULL;
}

blMppShape2D::~blMppShape2D(){
    delete m_silhouette;
}

std::string blMppShape2D::name(){
    return "blMppShape2D";
}

void blMppShape2D::setSilhouette(blMppContainerSilhouette* silhouette){
    m_silhouette = silhouette;
}

std::vector<blPixel> *blMppShape2D::representation(std::string type){

    std::vector<blPixel> *pixels;
    if (type == "border")
        pixels = borderPixels();
    else if (type == "centerpoint"){
        pixels = new std::vector<blPixel>;
        pixels->push_back(m_center);
    }
    else if (type == "centercross"){
        pixels = new std::vector<blPixel>;
        pixels->push_back(m_center);

        blPixel pix;
        pix.setX(m_center.x()+1);
        pix.setY(m_center.y());
        pixels->push_back(pix);

        blPixel pix2;
        pix2.setX(m_center.x()-1);
        pix2.setY(m_center.y());
        pixels->push_back(pix2);

        blPixel pix3;
        pix3.setX(m_center.x());
        pix3.setY(m_center.y()+1);
        pixels->push_back(pix3);

        blPixel pix4;
        pix4.setX(m_center.x());
        pix4.setY(m_center.y()-1);
        pixels->push_back(pix4);
    }
    else if (type == "full"){
        pixels = this->insidePixels();
    }
    else if (type == "silhouette"){
        pixels = new std::vector<blPixel>;
        vector<blMppContainerSilhouetteLine>* silhouette =  this->silhouette()->data();
        for (int i = 0 ; i < silhouette->size() ; ++i){
            blPixel left = silhouette->operator [](i).getPixelLeftAt(0);
            blPixel right = silhouette->operator [](i).getPixelRightAt(0);
            pixels->push_back(blPixel(left.x(), left.y()));
            pixels->push_back(blPixel(right.x(), right.y()));
        }
    }
    else{
        pixels = this->borderPixels();
    }
    return pixels;
}

blMppContainerSilhouette* blMppShape2D::silhouette(){
    return m_silhouette;
}

void blMppShape2D::computeInsidePixels(){
    m_insidePixelsCalculated = true;
    m_insidePixels = m_silhouette->insidePixels();
}

void blMppShape2D::computeBorderPixels(int borderWidth){
    if (borderWidth > 1){
        throw blException("the default shape border implementation can handle only borderwidth of 1");
    }
    //std::cout << "generateInsideShapeImage" << std::endl;
    m_borderPixels = new vector<blPixel>();
    int minX, minY;
    vector<vector<int> > pixelsImage = generateInsideShapeImage(minX, minY);
    //std::cout << "generateInsideShapeImage end" << std::endl;
    for (int i=1 ; i < pixelsImage.size()-1 ; i++){
        for (int j=1 ; j < pixelsImage[i].size()-1 ; j++){
            if (pixelsImage[i][j] == 0){
                if ( pixelsImage[i-1][j] > 0 || pixelsImage[i+1][j] > 0 || pixelsImage[i][j-1] > 0 || pixelsImage[i][j+1] > 0){
                    m_borderPixels->push_back(blPixel(i + (minX - 3), j + (minY - 3) ) ) ;
                }
            }
        }
    }
    m_borderPixelsCalculated = true;
}

void blMppShape2D::computeNormalsBorder(){


    //std::cout << "border size = " << m_borderPixels->size() << std::endl;
    //for(int i = 0 ; i <m_borderPixels->size() ; ++i ){
    //    std::cout << "border : (" << m_borderPixels->at(i).x() <<  ", " << m_borderPixels->at(i).y() << ")" << std::endl;
    //}

    m_normalsBorder = new std::vector<std::vector<float> >();
    m_normalsBorder->resize(m_borderPixels->size());
    for (int i = 0 ; i < m_borderPixels->size() ; ++i){

        //std::cout << "border process pixel at " << i << std::endl;
        // get the 2 neigbooring points
        vector<blPixel> neighboors;
        findClosestPixels(i, neighboors);

        /*
        std::cout << "calculate the normal at " << i << std::endl;
        std::cout << "neighboors size = " << neighboors.size() << std::endl;
        for(int b =0 ; b < neighboors.size() ; ++b){
            std::cout << "neighboors : (" << neighboors.at(b).x() <<  ", " << neighboors.at(b).y() << ")" << std::endl;
        }
        */
        // calculate the normal
        float x = neighboors[1].x() - neighboors[0].x();
        float y = neighboors[1].y() - neighboors[0].y();
        float norm = sqrt(x*x + y*y);
        x /= norm;
        y /= norm;

        //std::cout << "is inside at " << i << std::endl;
        if (this->silhouette()->isInside(blPixel(m_borderPixels->operator [](i).x() -int(y+0.5),m_borderPixels->operator [](i).y() + int(x+0.5)))){
            vector<float> normal; normal.resize(2);
            normal[0] = -y;
            normal[1] = x;
            m_normalsBorder->operator [](i) = normal;
        }
        else{
            vector<float> normal; normal.resize(2);
            normal[0] = y;
            normal[1] = -x;
            m_normalsBorder->operator [](i) = normal;
        }
    }
    m_normalsCalculated = true;
}

void blMppShape2D::findClosestPixels(int pos, vector<blPixel> &neighboors){

    blPixel ref = m_borderPixels->operator [](pos);

    //std::cout << "find closest to : " << ref.x() << ", " << ref.y() << std::endl;

    for (int x = ref.x()-1 ; x <= ref.x() +1 ; ++x){
        for (int y = ref.y()-1 ; y <= ref.y() +1 ; ++y)
        {
            //std::cout << "x : " << x << ",y : " << y << std::endl;
            if ( x == ref.x() && y == ref.y() ){
            }
            else{
                if (isPointInBorder(x, y)){
                    neighboors.push_back(blPixel(x,y));
                }
            }
        }
    }
    //std::cout << "find closest to : " << ref.x() << ", " << ref.y() << " done " << std::endl;
}

bool blMppShape2D::isPointInBorder(int x, int y){

    //std::cout << "is point in border" << std::endl;

    for (int i = 0 ; i < m_borderPixels->size() ; ++i){
        blPixel curent = m_borderPixels->operator [](i);
        if (curent.x() == x && curent.y() == y){
            //std::cout << "is point in border yes" << std::endl;
            return true;
        }
    }
    //std::cout << "is point in border done" << std::endl;
    return false;
}

vector<vector<int> > blMppShape2D::generateInsideShapeImage(int& minX, int& minY ){

    if (!m_insidePixelsCalculated){
        this->computeInsidePixels();
    }

    // get the silhouette area
    int maxX = -999999;
    minX = 999999;
    int maxY = -999999;
    minY = 999999;
    int x, y;
    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = m_insidePixels->operator [](i).x();
        y = m_insidePixels->operator [](i).y();
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
        if (y > maxY)
            maxY = y;
        if (y < minY)
            minY = y;
    }

    if (!m_centerSetted){
        m_center = blPixel((minX+maxX)/2, (minY+maxY)/2);
    }

    int sizeX = maxX - minX + 6; // +6 for border width
    int sizeY = maxY - minY + 6; // +6 for border width
    m_maxDistanceToBarycenter = sqrt( (maxX - minX)*(maxX - minX) + (maxY - minY)*(maxY - minY))/2;


    vector<vector<int> > silhouette; silhouette.resize(sizeX);
    for (int i=0 ; i < sizeX ; ++i){
        silhouette[i].resize(sizeY);
        for (int j=0 ; j < sizeY ; ++j){
            silhouette[i][j] = 0;
        }
    }
    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = m_insidePixels->operator [](i).x();
        y = m_insidePixels->operator [](i).y();
        silhouette[x-(minX-3)][y-(minY-3)] = 1; // +3 for border width
    }
    return silhouette;
}

void blMppShape2D::copyData(blMppShape2D* copied){

    //std::cout << "copy silhouette data" << std::endl;
    copied->setCenter(m_center);
    copied->setSilhouette(m_silhouette->copy());
    copied->setDataTerm(m_dataTerm);

    if (m_insidePixelsCalculated){
        std::vector<blPixel>* insidePixels = new std::vector<blPixel>();
        insidePixels->resize(m_insidePixels->size());
        for (int i = 0 ; i < m_insidePixels->size() ; ++i){
            insidePixels->operator [](i) = m_insidePixels->operator [](i);
        }
        copied->setInsidePixels(insidePixels);
    }
    else{
        copied->setInsidePixels(m_silhouette->insidePixels());
    }
    if (m_borderPixelsCalculated){
        std::vector<blPixel>* borderPixels = new std::vector<blPixel>();
        borderPixels->resize(m_borderPixels->size());
        for (int i = 0 ; i < m_borderPixels->size() ; ++i){
            borderPixels->operator [](i) = m_borderPixels->operator [](i);
        }
        copied->setBorderPixels(borderPixels);
    }
}
