/// \file blMppDataTerm.h
/// \brief blMppDataTerm class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blMppShape.h"

/// \class blMppDataTerm
/// \brief Class that define the prototype of an Marked Point Process
/// data term
class blMppDataTerm : public blProcess{

public:
    /// \fn blMppDataTerm();
    /// \brief Constructor
    blMppDataTerm();
    /// \fn blMppDataTerm(blImage* image);
    /// \brief Constructor
    /// \param[in] image Pointer to the blImage in which
    /// the data term is calculated
    blMppDataTerm(blImage* image);
    /// \fn virtual ~blMppDataTerm();
    /// \brief Destructor
    virtual ~blMppDataTerm();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief Inirits from blProcess
    virtual std::string name();

public:
    // input
    /// \fn void setImage(blImage* image);
    /// \param[in] image Pointer to the blImage in which
    /// the data term is calculated
    void setImage(blImage* image);
    /// \fn blImage* image();
    /// \return the pointer to the blImage in which
    /// the data term is calculated
    blImage* image();

public:
    // virtuals
    /// \fn virtual void initialize() = 0;
    /// \brief This method is used to calculate all
    /// the data the generator needs
    virtual void initialize() = 0;
    /// \fn virtual float compute(blMppShape* shape) = 0;
    /// \brief Compute the data term for a given shape
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    virtual float compute(blMppShape* shape) = 0;
    /// \fn virtual void createCompatibleShapesList() = 0;
    /// \brief Create the list of the shape generators
    /// with which this data term is compatible.
    /// if the data term is compatible with all the
    /// shapes generator set "all" to the list
    virtual void createCompatibleShapesList() = 0;

    /// \fn virtual bool useInsidePixels() = 0;
    /// \return true if the data term needs the generator
    /// to pre-calculate the coordinates of the pixels
    /// inside each shapes. Return false otherwise
    virtual bool useInsidePixels() = 0;
    /// \fn virtual bool useBorderPixels() = 0;
    /// \return true if the data term needs the generator
    /// to pre-calculate the coordinates of the pixels
    /// in the border of each shapes. Return false otherwise
    virtual bool useBorderPixels() = 0;
    /// \fn virtual bool useNormals() = 0;
    /// \return true if the data term needs the generator
    /// to pre-calculate the normals of the pixels
    /// in the border of each shapes. Return false otherwise
    virtual bool useNormals() = 0;
    /// \fn void setGeneratorName(std::string name);
    /// \param[in] name Name of the generator. It can be use in the
    /// case where the dataterm is calculated differently depending
    /// on the generator
    void setGeneratorName(std::string name);

public:
    // compatibilities with shape generator
    /// \fn bool isCompatible(std::string shapeGenerator);
    /// \return true if the data term is compatible
    /// with the generator called shapeGenerator
    bool isCompatible(std::string shapeGenerator);
    /// \fn std::vector<std::string> compatibleShapes();
    /// \return the list of the shape generators that
    /// are compatible with this data term
    std::vector<std::string> compatibleShapes();

public:
    // parameters
    /// \fn void setThreshold(float threshold);
    /// \param[in] threshold Value of the threshold
    /// needed to decide if the data term of an shape
    /// is high enough to keep the shape
    void setThreshold(float threshold);
    /// \fn void setBorderWidth(int width);
    /// \param[in] width Width of the border of the shapes
    /// (default = 1)
    void setBorderWidth(int width);
    /// \fn int borderWidth();
    /// \return the border width of the shapes
    int borderWidth();

public:
    // common functions
    /// \fn void insideAndBorderValues(blMppShape* shape, std::vector<float> &insideValues, std::vector<float> &borderValues);
    /// \brief Calculate the inside pixels values and the border pixels values for a given shape
    /// \param[in] shape Shape
    /// \param[out] insideValues Values of the pixels inside the shape
    /// \param[out] borderValues Values of the pixels in the border of the shape
    void insideAndBorderValues(blMppShape* shape, std::vector<float> &insideValues, std::vector<float> &borderValues);

private:
    /// \fn void initAttributs();
    /// \brief initialize all the class attributs (method called in the constructor)
    void initAttributs();

protected:
    float m_threshold; ///< threshold to decide to keep a shape or not
    float m_borderWidth; ///< width of the border around the shape
    blImage* m_image; ///< input image
    std::vector<std::string> m_compatibleShapesList; ///< list of shapes compatible with this data term
    std::string m_generatorName; ///< name of the generator
};
