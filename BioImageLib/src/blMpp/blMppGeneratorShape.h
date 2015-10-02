/// \file blMppGeneratorShape.h
/// \brief blMppGeneratorShape class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <blCore>
#include "blMppShape.h"
#include "blMppExport.h"

// //////////////////////////////////////////////////////// //
//                     blMppGeneratorShape
// //////////////////////////////////////////////////////// //
/// \class blMppGeneratorShape
/// \brief Class defining the prototype of a Marked Point Process shape generator
/// This class aims at generating random shapes, and apply perturbations to existing shapes.
/// A perturbation is defined as a translation, a rotation or a resizing
class BLMPP_EXPORT blMppGeneratorShape : public blProcess {

public:
    /// \fn blMppGeneratorShape();
    /// \brief Constructor
    blMppGeneratorShape();
    /// \fn blMppGeneratorShape(blImage* birthMap);
    /// \brief Constructor
    /// \param[in] birthMap Binary image defining a region
    /// of interest in the image. Shapes are generated
    /// only in pixel where the birthmap equals 255
    blMppGeneratorShape(blImage* birthMap);
    /// \fn blMppGeneratorShape(blImageSize size);
    /// \brief Constructor
    /// \param[in] size Size of the image where shapes
    /// are generated
    blMppGeneratorShape(blImageSize size);
    /// \fn virtual ~blMppGeneratorShape();
    /// \brief Destructor
    virtual ~blMppGeneratorShape();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplement from blProcess
    virtual std::string name();

public:
    /// \fn blMppShape* generateRandom();
    /// \return a pointer to a randomly generated shape
    blMppShape* generateRandom();

public:
    // Setters / getters
    /// \fn void setBirthMap(blImage* birthMap);
    /// \param[in] birthMap Binary image defining a region
    /// of interest in the image. Shapes are generated
    /// only in pixel where the birthmap equals 255
    void setBirthMap(blImage* birthMap);
    /// \fn void setBirthArea(blImageSize area);
    /// \param[in] area Size of the image where shapes
    /// are generated
    void setBirthArea(blImageSize area);
    /// \fn bool useBirthMap();
    /// \return true if the generator use a birthmap
    /// retrun false if the generator use the image area
    bool useBirthMap();

    /// \fn void setCalculateBorderPixels(bool value);
    /// \param[in] value set true is the generator have
    /// to calculate the coordinates of the border pixels
    /// of each generated shape
    void setCalculateBorderPixels(bool value);
    /// \fn void setCalculateInsidePixels(bool value);
    /// \param[in] value set true is the generator have
    /// to calculate the coordinates of the pixels inside
    /// of each generated shape
    void setCalculateInsidePixels(bool value);
    /// \fn void setCalculateNormals(bool value);
    /// \param[in] value set true is the generator have
    /// to calculate the normal of the border pixels
    /// of each generated shape
    void setCalculateNormals(bool value);

public:
    // parameters
    /// \fn void setBorderWidth(int width);
    /// \brief set the border width needed to
    /// calculate the border pixels
    void setBorderWidth(int width);

    // parameters perturbations
    /// \fn void setDeltaTranslation(int value);
    /// \param[in] value maximum possible translation (in pixels)
    /// during a translation perturbation
    void setDeltaTranslation(int value);
    /// \fn void setDeltaRotation(float value);
    /// \param[in] value maximum possible rotation (in radian)
    /// during a translation perturbation
    void setDeltaRotation(float value);
    /// \fn void setDeltaResize(int value);
    /// \param[in] value maximum possible dilatation or erosion (in pixels)
    /// during a resize perturbation
    void setDeltaResize(int value);
    /// \fn void setPerturbationsProba(float translation, float resize, float rotate);
    /// \brief set the probabilities to generate one of the 3 possible perturbations
    /// the sum of the 3 probabilities must equal to 1
    /// \param[in] translation Probability to generate a translation perturbation
    /// \param[in] resize Probability to generate a resize perturbation
    /// \param[in] rotate Probability to generate a rotation perturbation
    void setPerturbationsProba(float translation, float resize, float rotate);

public:
    /// \fn blPixel getRandomPixelFromMask();
    /// \return a pixel randomly taken in the birth map
    blPixel getRandomPixelFromMask();

    /// \fn bool is3DShape();
    ///  \return true if the object is represented by
    /// a 3D silhouette
    bool is3DShape();

public:
    /// \fn virtual void initialize() = 0;
    /// \brief this methods aims at calculating all the
    /// data the generator needs
    virtual void initialize() = 0;
    /// \fn virtual blMppShape* generateRandomUsingMask() = 0;
    /// \return a shape randomly generated using bith map
    virtual blMppShape* generateRandomUsingMask() = 0;
    /// \fn virtual blMppShape* generateRandomUsingArea() = 0;
    /// \return a shape randomly generated inside the image area
    virtual blMppShape* generateRandomUsingArea() = 0;
    /// \fn virtual int shapesMaxLength() = 0;
    /// \return the maximum possible length of a shape in x,y
    virtual int shapesMaxLength() = 0;
    /// \fn virtual int shapesMaxLengthZ();
    /// \return the maximum possible length of a shape in z
    virtual int shapesMaxLengthZ();

    /// \fn virtual blMppShape* generatePerturbation(blMppShape* shape);
    /// \return a new shape generated by perturbating the input shape
    /// \param[in] shape Shape to perturbate
    virtual blMppShape* generatePerturbation(blMppShape* shape);
    /// \fn virtual blMppShape* translate(blMppShape* shape) = 0;
    /// \return a new shape generated by translating the input shape
    /// \param[in] shape Shape to perturbate
    virtual blMppShape* translate(blMppShape* shape) = 0;
    /// \fn virtual blMppShape* rotate(blMppShape* shape) = 0;
    /// \return a new shape generated by rotation of the input shape
    /// \param[in] shape Shape to perturbate
    virtual blMppShape* rotate(blMppShape* shape) = 0;
    /// \fn virtual blMppShape* resize(blMppShape* shape) = 0;
    /// \return a new shape generated by resizing the input shape
    /// \param[in] shape Shape to perturbate
    virtual blMppShape* resize(blMppShape* shape) = 0;

private:
    /// \fn void initAttributs();
    /// \brief initialize all the class attributs (method called in the constructor)
    void initAttributs();

protected:
    // attributs
    blImageSize m_birthArea;
    std::vector<blPixel > m_birthMap;
    int m_borderWidth;
    bool m_useBirthMap;

protected:
    // parameters perturbation
    float m_deltaTranslation;
    float m_deltaRotation;
    float m_deltaResize;
    float m_ProbaPerutbation1;
    float m_ProbaPerutbation2;

protected:
    bool m_calculateBorderPixels;
    bool m_calculateInsidePixels;
    bool m_calculateNormals;

    bool m_is3DShapes;

};
