#include "blMppShapeDrawer.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QDebug>

blMppShapeDrawer::blMppShapeDrawer(QWidget *parent) :
    QWidget(parent)
{

    m_sizeX = 12; m_sizeY = 12;
    QSplitter *splitterRight = new QSplitter;

    QWidget* editor = createEditor();
    QWidget* libList = createLibraryList();

    splitterRight->addWidget(editor);
    splitterRight->addWidget(libList);
    splitterRight->setStretchFactor(0, 0);
    splitterRight->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(splitterRight);
    this->setLayout(layout);

}

void blMppShapeDrawer::setFile(std::string fileURL){
    m_silhouettelist->setFile(fileURL);
}

QWidget* blMppShapeDrawer::createEditor(){

    QWidget *editor = new QWidget;
    QVBoxLayout *editorLayout = new QVBoxLayout;
    editorLayout->setContentsMargins(0,0,0,0);
    editor->setLayout(editorLayout);
    //editor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    // 1- View
    QWidget *editorView = new QWidget;
    m_layoutView = new QHBoxLayout;
    m_layoutView->setContentsMargins(0,0,0,0);
    editorView->setLayout(m_layoutView);
    // grid editor area
    blGridClick* gridClickWidget = new blGridClick(m_sizeX, m_sizeY);
    setCurentWidget(gridClickWidget);

    editorLayout->addWidget(editorView,1);

    // 2- Settings
    QWidget *editorSettings = new QWidget;
    QVBoxLayout *layoutSettings = new QVBoxLayout;
    layoutSettings->setContentsMargins(0,0,0,0);
    editorSettings->setLayout(layoutSettings);
    // size
    QWidget *sizeWidget = new QWidget;
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->setContentsMargins(0,0,0,0);
    sizeWidget->setLayout(sizeLayout);

    QLabel *sizeXLabel = new QLabel(tr("Size X"));
    sizeLayout->addWidget(sizeXLabel, 0, Qt::AlignLeft);
    QDoubleSpinBox *sizeXBox = new QDoubleSpinBox;
    sizeXBox->setValue(m_sizeX);
    sizeLayout->addWidget(sizeXBox, 0, Qt::AlignLeft);
    connect(sizeXBox, SIGNAL(valueChanged(double)), this, SLOT(updateSizeX(double)));

    QLabel *sizeYLabel = new QLabel(tr("Size Y"));
    sizeLayout->addWidget(sizeYLabel, 0, Qt::AlignLeft);
    QDoubleSpinBox *sizeYBox = new QDoubleSpinBox;
    sizeYBox->setValue(m_sizeY);
    connect(sizeYBox, SIGNAL(valueChanged(double)), this, SLOT(updateSizeY(double)));
    sizeLayout->addWidget(sizeYBox, 1, Qt::AlignLeft);

    layoutSettings->addWidget(sizeWidget, 0, Qt::AlignBottom);

    // Reset/add
    QWidget *ResetAddWidget = new QWidget;
    QHBoxLayout *ResetAddLayout = new QHBoxLayout;
    ResetAddWidget->setLayout(ResetAddLayout);

    QPushButton *ResetButton = new QPushButton(tr("Reset"));
    ResetButton->setObjectName("btnDefault");
    ResetButton->setMaximumWidth(50);
    connect(ResetButton, SIGNAL(clicked()), this, SLOT(resetGrid()));
    ResetAddLayout->addWidget(ResetButton, 0, Qt::AlignBottom);

    QPushButton* rotationButton = new QPushButton(tr("Generate all rotations"));
    rotationButton->setObjectName("btnDefault");
    ResetAddLayout->addWidget(rotationButton, 0, Qt::AlignRight);
    connect(rotationButton, SIGNAL(clicked()), this, SLOT(generateAllRotations()));

    QPushButton* addButton = new QPushButton(tr("Add >>"));
    addButton->setObjectName("btnDefault");
    ResetAddLayout->addWidget(addButton, 0, Qt::AlignRight);
    connect(addButton, SIGNAL(clicked()), this, SLOT(addSilhouetteToList()));

    layoutSettings->addWidget(ResetAddWidget, 0, Qt::AlignBottom);

    editorLayout->addWidget(editorSettings, 0, Qt::AlignBottom);

    return editor;
}

void blMppShapeDrawer::setCurentWidget(blGridClick* gridClickWidget){
    m_curentGridClickWidget = gridClickWidget;
    QLayoutItem* item;
    while ( ( item = m_layoutView->takeAt( 0 ) ) != NULL )
    {
        m_layoutView->removeItem(item);
    }
    m_layoutView->addWidget(gridClickWidget, 1);
}

QWidget* blMppShapeDrawer::createLibraryList(){
    m_silhouettelist = new blMppShapeGridList;
    return m_silhouettelist;
}

void blMppShapeDrawer::addSilhouetteToList(){
    m_silhouettelist->addSilhouette(m_curentGridClickWidget);
    m_curentGridClickWidget = new blGridClick(m_sizeX, m_sizeY);
    setCurentWidget(m_curentGridClickWidget);
}

void blMppShapeDrawer::updateSizeX(double size){
    m_sizeX = int(size);
    m_curentGridClickWidget->setRegionSize(m_sizeX, m_sizeY);
    m_curentGridClickWidget->drawGrid();
}

void blMppShapeDrawer::updateSizeY(double size){
    m_sizeY = int(size);
    m_curentGridClickWidget->setRegionSize(m_sizeX, m_sizeY);
    m_curentGridClickWidget->drawGrid();
}

void blMppShapeDrawer::resetGrid(){
    m_curentGridClickWidget->drawGrid();
}

void blMppShapeDrawer::generateAllRotations(){

    //qDebug() << "blMppShapeDrawer::generateAllRotations() -> begin";
    vector<blGridClick*> rotations = m_curentGridClickWidget->generateAllRotations();

    //qDebug() << "blMppShapeDrawer::generateAllRotations() -> addSilhouette";
    for (unsigned int i = 0 ; i < rotations.size() ; ++i){
        m_silhouettelist->addSilhouette(rotations[i]);
    }

    delete m_curentGridClickWidget;
    //m_silhouettelist->addSilhouette(m_curentGridClickWidget);
    m_curentGridClickWidget = new blGridClick(m_sizeX, m_sizeY);
    setCurentWidget(m_curentGridClickWidget);
    //qDebug() << "blMppShapeDrawer::generateAllRotations() -> end";
}
