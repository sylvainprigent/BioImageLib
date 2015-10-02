/// \file blMppShape.h
/// \brief blMppShape class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014


#pragma once

#include <vector>
#include <string>
#include <blCore>
#include "blMppExport.h"

/// \class blMppShape
/// \brief Class defining an Shape for a Marked Point Process
class BLMPP_EXPORT blMppShape{

public:
    /// \fn blMppShape();
    /// \brief Constructor
    blMppShape();
    /// \fn virtual ~blMppShape();
    /// \brief Desctructor
    virtual ~blMppShape();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    // setters/ setters
    /// \fn void setCenter(blPixel center);
    /// \param[in] center Central pixel of the shape
    void setCenter(blPixel center);
    /// \fn blPixel getCenter();
    /// \return the central pixels of the shape
    blPixel getCenter();

    /// \fn void setDataTerm(float dataTerm);
    /// \param[in] dataTerm Data term of this shape
    void setDataTerm(float dataTerm);
    /// \fn float dataTerm();
    /// \return the data term of this shape
    float dataTerm();

    /// \fn float maxDistanceToBarycenter();
    /// \return the maximum distance between any point inside the shape
    /// and the shape center
    float maxDistanceToBarycenter();
    /// \fn void setMaxDistanceToBarycenter(float value);
    /// \param[in] value The maximum distance between any point inside the shape
    /// and the shape center
    void setMaxDistanceToBarycenter(float value);
    /// \fn virtual std::vector<float> getParameters();
    /// \return the parameters of the shape. If the shape
    /// is defined by an equation, is return the parameters of the equation
    /// If the shape is defined by a silhouette, it returns the id of the
    /// shape inside a silhouette library
    virtual std::vector<float> getParameters();

    /// \fn virtual std::vector<std::string>& parametersDescription();
    /// \return the name of each parameters of the shape
    virtual std::vector<std::string> parametersDescription();

    /// \fn virtual std::vector<blPixel> * representation(std::string type) = 0;
    /// \return the list of pixels that represent the shape
    /// \param[in] type Representation type
    ///                 - border: the border of the shape
    ///                 - centerPoint: the point in the center of the shape
    ///                 - centercross: a cross centerd in the center of the shape
    ///                 - full: all the pixels inside the shape
    ///                 - silhouette: the silhouette of the shape
    virtual std::vector<blPixel> * representation(std::string type) = 0;

public:
    // setters/getters pixels
    /// \fn void setInsidePixels(std::vector<blPixel> *insidePixels);
    /// \param[in] insidePixels Pixels inside the shape
    void setInsidePixels(std::vector<blPixel> *insidePixels);
    /// \fn std::vector<blPixel> * insidePixels();
    /// \return the pixels inside the shape
    std::vector<blPixel> * insidePixels();

    /// \fn void setBorderPixels(std::vector<blPixel> *borderPixels);
    /// \param[in] borderPixels Pixels in the border of the shape
    void setBorderPixels(std::vector<blPixel> *borderPixels);
    /// \fn std::vector<blPixel> * borderPixels();
    /// \return the pixels in the border of the shape
    std::vector<blPixel> * borderPixels();

    /// \fn void setNormalsBorder(std::vector<std::vector<float> > *normalsBorder);
    /// \param[in] normalsBorder Normals of the shape
    void setNormalsBorder(std::vector<std::vector<float> > *normalsBorder);
    /// \fn std::vector<std::vector<float> > *normalsBorder();
    /// \return the normals of the shape
    std::vector<std::vector<float> > *normalsBorder();

public:
    // silhouette computation
    /// \fn virtual void computeSilhouette() = 0;
    /// \brief compute the silhouette
    virtual void computeSilhouette() = 0;
    /// \fn virtual void computeInsidePixels() = 0;
    /// \brief compute the pixels coordinates inside the shape
    virtual void computeInsidePixels() = 0;
    /// \fn virtual void computeBorderPixels(int borderWidth) = 0;
    /// \brief compute the pixels coordinates in the border of the shape
    virtual void computeBorderPixels(int borderWidth) = 0;
    /// \fn virtual void computeNormalsBorder();
    /// \brief compute the normals to the pixel in the border of the shape
    virtual void computeNormalsBorder() = 0;
    /// \fn virtual void computeMaxDistanceToBarycenter() = 0;
    /// \brief compute the max distance between the shape center and any pixel
    /// inside the shape
    virtual void computeMaxDistanceToBarycenter() = 0;
    /// \fn virtual blMppShape* copy()=0;
    /// \return a copy of the shape
    virtual blMppShape* copy()=0;

protected:
    float m_maxDistanceToBarycenter;
    float m_dataTerm;
    blPixel m_center;
    std::vector<blPixel>* m_insidePixels;
    std::vector<blPixel>* m_borderPixels;
    std::vector<std::vector<float> >* m_normalsBorder;

    bool m_insidePixelsCalculated;
    bool m_borderPixelsCalculated;
    bool m_normalsCalculated;

    bool m_centerSetted;

};
