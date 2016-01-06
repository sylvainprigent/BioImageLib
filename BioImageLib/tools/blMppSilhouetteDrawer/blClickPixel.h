#ifndef blClickPixel_H
#define blClickPixel_H

#include <QLabel>
#include <QMouseEvent>

class blClickPixel : public QLabel
{
    Q_OBJECT
public:
    explicit blClickPixel(int posX, int posY, bool isActivated = false, QWidget *parent = 0);
    
signals:
    
public slots:
    void setActivatedBlack(bool activated);
    bool isActivatedBlack();
    void setActivatedGray(bool activated);
    bool isActivatedGray();
    void setEditable(bool editable);
    int getPosX();
    int getPosY();

private:
    //attributs
    int m_posX, m_posY;
    bool m_isActivatedBlack;
    bool m_isActivatedGray;
    bool m_isEditable;

private:
    // methods
    void setBackgroundColor(bool isRight = false);

protected:
    void mousePressEvent ( QMouseEvent * e );
};

#endif // blClickPixel_H
