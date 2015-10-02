/// \file blMppGeneratorShapeSilhouette2D.h
/// \brief blMppGeneratorShapeSilhouette2D class
/// \author Emmanuelle Poulain (Emmanuelle.Poulain@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include "blMppGeneratorShape.h"
#include "blMppShapeSilhouette.h"
#include "blMppExport.h"

/// \class blMppGeneratorShapeSilhouette2D
/// \brief Generator of 2D silhouette defined by blMppShapeSilhouette
class BLMPP_EXPORT blMppGeneratorShapeSilhouette2D : public blMppGeneratorShape {

public:
    /// \fn blMppGeneratorShapeSilhouette2D();
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSilhouette2D();
    /// \fn blMppGeneratorShapeSilhouette2D(blImage* birthMap);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSilhouette2D(blImage* birthMap);
    /// \fn blMppGeneratorShapeSilhouette2D(blImageSize size);
    /// \brief reimplemented from blMppGeneratorShape
    blMppGeneratorShapeSilhouette2D(blImageSize size);
    /// \fn virtual ~blMppGeneratorShapeSilhouette2D();
    /// \brief reimplemented from blMppGeneratorShape
    virtual ~blMppGeneratorShapeSilhouette2D();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blMppGeneratorShape
    virtual std::string name();

public:
    // setters
    /// \fn void setSilhouetteLibraryFile(std::string file);
    /// \param[in] file File containing the silhouette library
    void setSilhouetteLibraryFile(std::string file);

    /// \fn void setLibraryDirectory(string libraryDir);
    /// \param[in] libraryDir Directory containing the library
    void setLibraryDirectory(std::string libraryDir);

    /// \fn void setNumberOfFile(int nbFiles);
    /// \param[in] nbFiles Number of files constituting the library
    void setNumberOfFile(int nbFiles);

public:
    /// \fn void initialize();
    /// \brief reimplemented from blMppGeneratorShape
    void initialize();
    /// \fn virtual blMppShape* generateRandomUsingMask();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingMask();
    /// \fn virtual blMppShape* generateRandomUsingArea();
    /// \brief reimplemented from blMppGeneratorShape
    virtual blMppShape* generateRandomUsingArea();
    /// \fn virtual int shapesMaxLength();
    /// \brief reimplemented from blMppGeneratorShape
    virtual int shapesMaxLength();

    /// \fn blMppShape* translate(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* translate(blMppShape* shape);
    /// \fn blMppShape* rotate(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* rotate(blMppShape* shape);
    /// \fn blMppShape* resize(blMppShape* shape);
    /// \brief reimplemented from blMppGeneratorShape
    blMppShape* resize(blMppShape* shape);

private:
    /// \fn void initAttributs();
    /// \brief initialize all the class attributs (method called in the constructor)
    void initAttributs();

protected:
    // parameters
    std::string m_libraryDir;
    int m_nbFiles;

    // internal data
    std::vector<blMppShapeSilhouette*> m_shapeLibrary;
    int m_maxLength;


protected:
    void calculateShapeData(blPixel centerPixel, blMppShapeSilhouette* templateShape, blMppShapeSilhouette *newShape);
    void copyNormals(blMppShapeSilhouette* templateShape, blMppShapeSilhouette *newShape);
    void translatePixels(std::vector<blPixel>* pixels, blPixel t, std::vector<blPixel>* translated);
    void addSilhouette(blMppContainerSilhouette *silhouette);

private:
    void loadSilhouettes(int indFichier);
    void loadGlobalPropertiesFiles();

};
