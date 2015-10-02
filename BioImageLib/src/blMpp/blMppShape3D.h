/// \file blMppShape3D.h
/// \brief blMppShape3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>
#include <blCore>
#include "blMppShape.h"
#include "blMppContainerSilhouette3D.h"
#include "blMppExport.h"

/// \class blMppShape3D
/// \brief Class defining a 3D Shape for a Marked Point Process
class BLMPP_EXPORT blMppShape3D : public blMppShape{

public:
    /// \fn blMppShape3D();
    /// \brief Constructor
    blMppShape3D();
    /// \fn virtual ~blMppShape3D();
    /// \brief Desctructor
    virtual ~blMppShape3D();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    // setters/ setters
    /// \fn virtual std::vector<std::string>& parametersDescription();
    /// \return the name of each parameters of the shape
    virtual std::vector<std::string> parametersDescription() = 0;

    /// \fn std::vector<blPixel> * representation(std::string type);
    /// \return the list of pixels that represent the shape
    /// \param[in] type Representation type
    ///                 - border: the border of the shape
    ///                 - centerPoint: the point in the center of the shape
    ///                 - centercross: a cross centerd in the center of the shape
    ///                 - full: all the pixels inside the shape
    ///                 - silhouette: the silhouette of the shape
    std::vector<blPixel> * representation(std::string type);

public:
    // setters/getters pixels
    /// \fn void setSilhouette(blMppContainerSilhouette3D* silhouette);
    /// \param[in] silhouette Silhouette of the shape
    void setSilhouette(blMppContainerSilhouette3D* silhouette);
    /// \fn blMppContainerSilhouette3D* silhouette();
    /// \return the silhouette of the shape
    blMppContainerSilhouette3D* silhouette();

public:
    // silhouette computation
    /// \fn virtual void computeSilhouette() = 0;
    /// \brief compute the silhouette
    virtual void computeSilhouette() = 0;
    /// \fn virtual void computeInsidePixels() = 0;
    /// \brief compute the pixels coordinates inside the shape
    virtual void computeInsidePixels();
    /// \fn virtual void computeBorderPixels(int borderWidth) = 0;
    /// \brief compute the pixels coordinates in the border of the shape
    virtual void computeBorderPixels(int borderWidth);
    /// \fn virtual void computeNormalsBorder();
    /// \brief compute the normals to the pixel in the border of the shape
    virtual void computeNormalsBorder();
    /// \fn virtual void computeMaxDistanceToBarycenter() = 0;
    /// \brief compute the max distance between the shape center and any pixel
    /// inside the shape
    virtual void computeMaxDistanceToBarycenter() = 0;
    /// \fn virtual blMppShape3D* copy()=0;
    /// \return a copy of the shape
    virtual blMppShape3D* copy()=0;
    /// \fn void copyData(blMppShape* copied) = 0;
    /// \brief copy this shape into a new shape copied
    /// \param[in] copied Pointer to the copy
    void copyData(blMppShape3D *copied);

public:
    std::vector<std::vector<int> > generateInsideShapeImage(int &minX, int &minY, int &minZ, int &sizeX, int &sizeY, int &sizeZ);

protected:
    blMppContainerSilhouette3D* m_silhouette;

    // internal methods
    void findClosestPixels(int pos, std::vector<blPixel> &neighboors);
    bool isPointInBorder(int x, int y);
};
