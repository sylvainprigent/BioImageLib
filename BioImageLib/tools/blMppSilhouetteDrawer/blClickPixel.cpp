#include "blClickPixel.h"

blClickPixel::blClickPixel(int posX, int posY, bool isActivated, QWidget *parent) :
    QLabel(parent)
{
    m_posX = posX;
    m_posY = posY;
    m_isActivatedBlack = isActivated;
    m_isActivatedGray = false;
    m_isEditable = true;

    setBackgroundColor(false);
    this->setFixedSize(20,20);
}

void blClickPixel::setBackgroundColor(bool isRight){
    if (!isRight){
        if (m_isActivatedBlack){
            this->setStyleSheet("QLabel { background-color : black; }");
        }
        else{
            this->setStyleSheet("QLabel { background-color : white; }");
        }
    }
    else{
        if (m_isActivatedGray){
            this->setStyleSheet("QLabel { background-color : Gray; }");
        }
        else{
            this->setStyleSheet("QLabel { background-color : white; }");
        }
    }
}


void blClickPixel::setActivatedBlack(bool activated){
    m_isActivatedBlack = activated;
    setBackgroundColor(false);
}

bool blClickPixel::isActivatedBlack(){
    return m_isActivatedBlack;
}

void blClickPixel::setActivatedGray(bool activated){
    m_isActivatedGray = activated;
    setBackgroundColor(true);
}

bool blClickPixel::isActivatedGray(){
    return m_isActivatedGray;
}

void blClickPixel::setEditable(bool editable){
    m_isEditable = editable;
}

int blClickPixel::getPosX(){
    return m_posX;
}

int blClickPixel::getPosY(){
    return m_posY;
}

void blClickPixel::mousePressEvent ( QMouseEvent * e )
{
    if (e->button() == Qt::LeftButton){
        if (m_isEditable){
            if (m_isActivatedBlack){
                m_isActivatedBlack = false;
            }
            else{
                m_isActivatedBlack = true;
                m_isActivatedGray = false;
            }
            setBackgroundColor(false);
        }
    }
    /*
    else if(e->button() == Qt::RightButton){
        if (m_isEditable){
            if (m_isActivatedGray){
                m_isActivatedGray = false;
            }
            else{
                m_isActivatedGray = true;
                m_isActivatedBlack = false;
            }
            setBackgroundColor(true);
        }
    }
    */
}
