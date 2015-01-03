/// \file blMppContainerSilhouette.cpp
/// \brief blMppContainerSilhouette class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppContainerSilhouette.h"
#include <iostream>

using namespace std;

// //////////////////////////////////////////////// //
//             blMppContainerSilhouette
// /////////////////////////////////////////////// //
blMppContainerSilhouette::~blMppContainerSilhouette(){
    delete m_data;
}

blMppContainerSilhouette::blMppContainerSilhouette(){
    m_data = new std::vector<blMppContainerSilhouetteLine>;
    m_area = 0;
}

void blMppContainerSilhouette::setData(std::vector<blMppContainerSilhouetteLine> *data){
    delete m_data;
    m_data = data;
}

void blMppContainerSilhouette::addLine(int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel){
    m_data->push_back(blMppContainerSilhouetteLine(lineIdx, leftPixel, rightPixel));
}

void blMppContainerSilhouette::addLine(const blMppContainerSilhouetteLine &line){
    m_data->push_back(line);
}

int blMppContainerSilhouette::area(){
    return m_area;
}

void blMppContainerSilhouette::setArea(int area){
    m_area = area;
}

bool blMppContainerSilhouette::isOverlaping(blMppContainerSilhouette *other, float allowedPurcentage){

    if (allowedPurcentage == 0){
        // find the first common line x
        int m = -1; int n = -1;
        findFirstCommonLine(other, m, n);

        if (m == -1){
            return false;
        }

        // compare objects line by line
        while(1){
            for (int i = 0 ; i < m_data->operator [](m).pairNumber() ; ++i){
                for (int j = 0 ;j < other->m_data->operator [](n).pairNumber() ; ++j){
                    float y1 = m_data->operator [](m).getPixelLeftAt(i).y();
                    float y2 = m_data->operator [](m).getPixelRightAt(i).y();

                    float yp1 = other->m_data->operator [](n).getPixelLeftAt(j).y();
                    float yp2 = other->m_data->operator [](n).getPixelRightAt(j).y();

                    if (y2 < yp1){
                        continue;
                    }
                    if (yp2 < y1){
                        continue;
                    }
                    if ( y2 >= yp1 && y2 <= yp2){
                        return true;
                    }
                    if (yp2 >= y1 && yp2 <= y2){
                        return true;
                    }
                }
            }
            m++; if (m >= m_data->size()){return false;}
            n++; if (n >= other->m_data->size()){return false;}
            if (m_data->operator [](m).lineIdx() != other->m_data->operator [](n).lineIdx()){
                return false;
            }
        }
    }
    else
    {
        if (purcentageOverlaping(other) > allowedPurcentage){
            return true;
        }
        return false;
    }
}

float blMppContainerSilhouette::purcentageOverlaping(blMppContainerSilhouette *other){
    // find the first common line x
    int m = -1; int n = -1;
    //=>
    findFirstCommonLine(other, m, n);

    if (m == -1){
        return 0;   //EDIT
    }
    // compare objects line by line
    int overlapArea = 0;
    float area = other->area();
    if (m_area < area){
        area = m_area;
    }
    while(1){
        for (int i = 0 ; i < m_data->operator [](m).pairNumber() ; ++i){
            for (int j = 0 ;j < other->m_data->operator [](n).pairNumber() ; ++j){
                float y1 = m_data->operator [](m).getPixelLeftAt(i).y();
                float y2 = m_data->operator [](m).getPixelRightAt(i).y();

                float yp1 = other->m_data->operator [](n).getPixelLeftAt(j).y();
                float yp2 = other->m_data->operator [](n).getPixelRightAt(j).y();

                if (y2 < yp1){
                    continue;
                }
                if (yp2 < y1){
                    continue;
                }
                if ( y2 >= yp1 && y2 <= yp2){
                    if (y1 <= yp1)
                        overlapArea += y2 - yp1 + 1;
                    else
                        overlapArea += yp2 - yp1 + 1;
                }
                if (yp2 >= y1 && yp2 <= y2){
                    if (yp1 <= y1)
                        overlapArea += yp2 - y1 + 1;
                    else
                        overlapArea += y2 - y1 + 1;
                }
            }
        }
        m++; if (m >= m_data->size()){return float(overlapArea)/area;}
        n++; if (n >= other->m_data->size()){return float(overlapArea)/area;}
        if (m_data->operator [](m).lineIdx() != other->m_data->operator [](n).lineIdx()){
            return float(overlapArea)/area;
        }
    }
}

bool blMppContainerSilhouette::isInside(blPixel pixel){

    // find common x
    int x = pixel.x();
    int x_min = m_data->operator [](0).lineIdx();
    int x_max = m_data->operator [](m_data->size()-1).lineIdx();

    if ( x >= x_min && x <= x_max){
        int pos = x - x_min;

        // test all the couples
        int y = pixel.y();
        for (int j = 0 ;j < m_data->operator [](pos).pairNumber() ; ++j){
            float y1 = m_data->operator [](pos).getPixelLeftAt(j).y();
            float y2 = m_data->operator [](pos).getPixelRightAt(j).y();

            if ( y1 <= y && y <= y2 ){
                return true;
            }
        }
        return false;
    }
    else{
        return false;
    }

}

void blMppContainerSilhouette::findFirstCommonLine(blMppContainerSilhouette* other, int &m, int &n){

    int x_min = m_data->operator [](0).lineIdx();
    int x_max = m_data->operator [](m_data->size()-1).lineIdx();

    int xp_min = other->m_data->operator [](0).lineIdx();
    int xp_max = other->m_data->operator [](other->m_data->size()-1).lineIdx();

    if (x_min >= xp_min && x_min <=xp_max){
        m = 0;
        n = x_min - xp_min;
    }
    else if (xp_min >= x_min && xp_min <=x_max){
        m = xp_min - x_min;
        n = 0;
    }
}

void blMppContainerSilhouette::calculateArea(){

    m_area = 0;
    for (int i = 0 ; i < m_data->size() ; ++i){
        for (int j = 0 ; j < m_data->operator [](i).pairNumber() ; ++j){
            int y1 = m_data->operator [](i).getPixelLeftAt(j).y();
            int y2 = m_data->operator [](i).getPixelRightAt(j).y();
            m_area += abs(y2-y1)+1;
        }
    }
}

std::vector<blPixel> *blMppContainerSilhouette::insidePixels(){

    std::vector<blPixel> *insidePixels = new std::vector<blPixel>;
    insidePixels->resize(m_area);
    int iter = -1;
    for (int i = 0 ; i < m_data->size() ; ++i){
        for (int j = 0 ; j < m_data->operator [](i).pairNumber() ; ++j){
            int x = m_data->operator [](i).lineIdx();
            int y1 = m_data->operator [](i).getPixelLeftAt(j).y();
            int y2 = m_data->operator [](i).getPixelRightAt(j).y();
            int z =  m_data->operator [](i).getPixelLeftAt(j).z();

            if (y1 == y2){
                iter++;
                insidePixels->operator [](iter) = blPixel(x,y1);
            }
            else{
                for (int y = int(y1) ; y <= int(y2) ; ++y){
                    iter++;
                    insidePixels->operator [](iter) = blPixel(x,y,z);
                }
            }
        }
    }
    return insidePixels;
}

void blMppContainerSilhouette::printConsole(){
    for (int i = 0 ; i < m_data->size() ; ++i){
        for (int j = 0 ; j < m_data->operator [](i).pairNumber() ; ++j){
            int x = m_data->operator [](i).lineIdx();
            int y1 = m_data->operator [](i).getPixelLeftAt(j).y();
            int y2 = m_data->operator [](i).getPixelRightAt(j).y();

            cout << "line: " << x << " ( " << y1 << ", " << y2 << ")" << endl;
        }
    }
}

std::vector<blMppContainerSilhouetteLine>* blMppContainerSilhouette::data(){
    return m_data;
}

blMppContainerSilhouette* blMppContainerSilhouette::translate(blPixel pixel){

    std::vector<blMppContainerSilhouetteLine>* dataNew = new std::vector<blMppContainerSilhouetteLine>;
    dataNew->resize(m_data->size());
    std::vector<blMppContainerSilhouetteLine>::iterator itNew;
    std::vector<blMppContainerSilhouetteLine>::iterator itOrig;
    for (itNew = dataNew->begin(), itOrig = m_data->begin(); (itNew != dataNew->end()) && (itOrig != m_data->end()); ++itNew, ++itOrig) {
        *itNew = (*itOrig).translate(pixel);
    }

    blMppContainerSilhouette *silhouette = new blMppContainerSilhouette;
    silhouette->setArea(m_area);
    silhouette->setData(dataNew);
    return silhouette;
}

blMppContainerSilhouette* blMppContainerSilhouette::copy(){

    blMppContainerSilhouette* silhouette = new blMppContainerSilhouette();

    silhouette->setArea(m_area);
    std::vector<blMppContainerSilhouetteLine> *data = new std::vector<blMppContainerSilhouetteLine>();
    data->resize(m_data->size());
    for (int i = 0 ; i < m_data->size() ; ++i){
        data->operator [](i) = m_data->operator [](i);
    }
    silhouette->setData(data);
    return silhouette;
}

int blMppContainerSilhouette::silhouetteMaxLength(){
    int lengthX = m_data->size();

    int lengthY = 0;
    int val, y1, y2;
    for (int i = 0 ; i < m_data->size() ; ++i){
        y1 = m_data->operator [](i).getPixelLeftAt(0).y();
        y2 = m_data->operator [](i).getPixelRightAt(m_data->operator [](i).pairNumber()-1).y();
        val = abs(y1-y2);
        if (val > lengthY){
            lengthY = val;
        }
    }
    return max(lengthX, lengthY);
}

// //////////////////////////////////////////////// //
//             blMppContainerSilhouetteLine
// /////////////////////////////////////////////// //
blMppContainerSilhouetteLine::blMppContainerSilhouetteLine(){
    m_lineIdx = -1;
}

blMppContainerSilhouetteLine::blMppContainerSilhouetteLine(int lineIdx){
    m_lineIdx = lineIdx;
}

blMppContainerSilhouetteLine::blMppContainerSilhouetteLine(int lineIdx, blPixel leftPixel, blPixel rightPixel){

    m_lineIdx = lineIdx;
    m_pixelsLeft.resize(1);
    m_pixelsRight.resize(1);
    m_pixelsLeft[0] = leftPixel;
    m_pixelsRight[0] = rightPixel;
}

void blMppContainerSilhouetteLine::addPointPair(const blPixel &leftPixel, const blPixel &rightPixel){
    m_pixelsLeft.push_back(leftPixel);
    m_pixelsRight.push_back(rightPixel);
    m_lineIdx = m_pixelsLeft[m_pixelsLeft.size()-1].x();
}

// getters / setters
int blMppContainerSilhouetteLine::lineIdx(){
    return m_lineIdx;
}

int blMppContainerSilhouetteLine::pairNumber(){
    return m_pixelsLeft.size();
}

blPixel blMppContainerSilhouetteLine::getPixelLeftAt(int i){
    return m_pixelsLeft[i];
}

blPixel blMppContainerSilhouetteLine::getPixelRightAt(int i){
    return m_pixelsRight[i];
}

void blMppContainerSilhouetteLine::setData(int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right){
    m_lineIdx = lineIdx;
    m_pixelsLeft = left;
    m_pixelsRight = right;
}

blMppContainerSilhouetteLine blMppContainerSilhouetteLine::translate(blPixel t){

    std::vector<blPixel> pixelsLeft; pixelsLeft.resize(m_pixelsLeft.size());
    std::vector<blPixel> pixelsRight; pixelsRight.resize(m_pixelsRight.size());

    int newX = m_lineIdx+t.x();
    for (int i = 0 ; i < pixelsLeft.size() ; ++i){
        pixelsLeft[i] = blPixel(newX, m_pixelsLeft[i].y()+t.y(), m_pixelsLeft[i].z() + t.z());
        pixelsRight[i] = blPixel(newX, m_pixelsRight[i].y()+t.y(), m_pixelsRight[i].z() + t.z());
    }

    blMppContainerSilhouetteLine line;
    line.setData(newX, pixelsLeft, pixelsRight);
    return line;
}

void blMppContainerSilhouetteLine::clear(){
    m_lineIdx = 0;
    m_pixelsLeft.clear();
    m_pixelsRight.clear();
}
