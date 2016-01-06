#include "blClosableWidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

blClosableWidget::blClosableWidget(QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    m_inputWidget = widget;
    QVBoxLayout *totalLayout = new QVBoxLayout;
    totalLayout->setContentsMargins(0,0,0,0);
    QWidget *totalWidget = new QWidget;
    totalLayout->addWidget(totalWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    //layout->setContentsMargins(0,0,0,0);

    closeBar = new QWidget;
    QHBoxLayout* closeBarLayout = new QHBoxLayout;
    closeBarLayout->setContentsMargins(0,0,0,0);
    closeBarLayout->setSpacing(0);
    closeBar->setLayout(closeBarLayout);
    closeBar->setVisible(false);

    QPushButton *closeButton = new QPushButton(tr("x"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(EmitClose()));
    closeButton->setObjectName("blClosableWidgetCloseButton");
    closeButton->setMaximumSize(20,20);

    closeBarLayout->addWidget(closeButton, 1, Qt::AlignRight);

    layout->addWidget(closeBar);
    layout->addWidget(widget);

    totalWidget->setLayout(layout);
    this->setLayout(totalLayout);

    this->setStyleSheet(".QWidget{background-color: lightgray;}");
}

void blClosableWidget::EmitClose(){

    emit CloseMe();
    emit CloseMe(m_id);
    emit CloseMe(m_id, m_idString);
    emit CloseMe(m_idString);
}

void blClosableWidget::enterEvent(QEvent *){
   closeBar->setVisible(true);
   this->setStyleSheet(".QWidget{background-color: #5A8E8E;}");
}

void blClosableWidget::leaveEvent(QEvent *){
   closeBar->setVisible(false);
   this->setStyleSheet(".QWidget{background-color: #e1e1e1;}");
}


void blClosableWidget::setId(int id){
    m_id = id;
}

void blClosableWidget::setIdString(QString id){
    m_idString = id;
}

int blClosableWidget::getId(){
    return m_id;
}

QString blClosableWidget::getidString(){
    return m_idString;
}

QWidget* blClosableWidget::GetWidget(){
    return m_inputWidget;
}
