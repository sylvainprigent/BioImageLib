/// \file blMppContainerConfigShape2D.h
/// \brief blMppContainerConfig2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include "blMppShape.h"
#include "blMppInteraction.h"
#include "blMppShapeGraph.h"

/// \class blMppContainerConfig2D
/// \brief Virtual class to store the 2D shapes in a 2D grid to accelerate the interaction calculation
class blMppContainerConfig2D{

public:
    /// \fn blMppContainerConfig2D();
    /// \brief Constructor
    blMppContainerConfig2D();
    /// \fn blMppContainerConfig2D();
    /// \brief Constructor
    /// \param[in] imageSize Size of the image
    /// \param[in] shapeMaxLength Maximum length of a shape
    blMppContainerConfig2D(blImageSize imageSize, int shapeMaxLength);
    /// \fn ~blMppContainerConfig2D();
    /// \brief Destructor
    ~blMppContainerConfig2D();

public:
    // getters/setters
    /// \fn int getsize();
    /// \return The number of shapes in the grid
    int getsize();

public:
    // operations
    /// \brief return true if there is at least one interaction with "new_Shape"
    /// \param[in] new_Shape Shape to test
    /// \param[in] interaction Interaction class
    virtual bool isInteraction(blMppShape *new_Shape, blMppInteraction *interaction) = 0;

public:
    /// \fn int findClosestBucket(blMppShape *new_Shape);
    /// \return the index of the bucket in which the "new_Shape" should be
    int findClosestBucket(blMppShape *new_Shape);
    /// \fn std::vector<int> getNeighboorBucket(int bucket);
    /// \param[in] bucket Index of the curent bucket
    /// \return the list of buckets in interaction with "bucket"
    std::vector<int> getNeighboorBucket(int bucket);

private:
    /// \fn void initAttributs();
    /// \brief initialize all the class attributs (method called in the constructor)
    void initAttributs();

protected:
    // internal data
    int m_numBucketsX; ///< Number of buckets in the X direction
    int m_numBucketsY; ///< Number of buckets in the Y direction
    int m_sizeBucketX; ///< Size of the buckets in the X direction
    int m_sizeBucketY; ///< Size of the buckets in the Y direction
    int m_shapeMaxLength; ///< maximum possible length of a shape
    int m_size; ///< Number of shapes in the grid

    // internal methods
    /// \fn void initBuckets(blImageSize imagesize, int shapeMaxLength);
    /// \brief Create the grid
    /// \param[in] imageSize Size of the image
    /// \param[in] shapeMaxLength Maximum length of a shape
    void initBuckets(blImageSize imagesize, int shapeMaxLength);
};


/// \class blMppContainerConfigShape2D
/// \brief store blMppShape s  in a 2D grid to accelerate the interaction calculation
class blMppContainerConfigShape2D : public blMppContainerConfig2D{

public:  
    /// \fn blMppContainerConfigShape2D();
    /// \brief Constructor
    blMppContainerConfigShape2D();
    /// \fn blMppContainerConfigShape2D();
    /// \brief Constructor
    /// \param[in] imageSize Size of the image
    /// \param[in] shapeMaxLength Maximum length of a shape
    blMppContainerConfigShape2D(blImageSize imageSize, int shapeMaxLength);
    /// \fn ~blMppContainerConfigShape2D();
    /// \brief Destructor
    ~blMppContainerConfigShape2D();

public:
    // getters/setters
    /// \fn void addShape(blMppShape *new_Shape);
    /// \brief add a shape to the grid
    /// \param[in] new_Shape Shape to add
    void addShape(blMppShape *new_Shape);
    /// \fn void setData(std::vector<std::vector<blMppShape*> >* data);
    /// \param[in] data Grid content
    void setData(std::vector<std::vector<blMppShape*> >* data);
    /// \fn std::vector<std::vector<blMppShape*> >* configData();
    /// \return the grid content
    std::vector<std::vector<blMppShape*> >* configData();
    /// \fn blMppShape* operator[](int ind);
    /// \return return the shape at ind
    /// \param[in] ind Index of the shape to get
    blMppShape* operator[](int ind);

public:
    // operations
    /// \brief return true if there is at least one interaction with "new_Shape"
    /// \param[in] new_Shape Shape to test
    /// \param[in] interaction Interaction class
    bool isInteraction(blMppShape *new_Shape, blMppInteraction *interaction);
    /// \fn void clear();
    /// \brief Clear the grid (remove all the shapes). This does not free the shape pointers
    void clear();

protected:
    // internal data
    std::vector<std::vector<blMppShape*> >* m_config; ///< grid content
};

/// \class blMppContainerConfigShapeGraph2D
/// \brief store blMppShapeGraph s  in a 2D grid to accelerate the interaction calculation
class blMppContainerConfigShapeGraph2D : public blMppContainerConfig2D{

public:
    /// \fn blMppContainerConfigShapeGraph2D();
    /// \brief Constructor
    blMppContainerConfigShapeGraph2D();
    /// \fn blMppContainerConfigShapeGraph2D();
    /// \brief Constructor
    /// \param[in] imageSize Size of the image
    /// \param[in] shapeMaxLength Maximum length of a shape
    blMppContainerConfigShapeGraph2D(blImageSize imageSize, int shapeMaxLength);
    /// \fn ~blMppContainerConfigShapeGraph2D();
    /// \brief Destructor
    ~blMppContainerConfigShapeGraph2D();

public:
    // getters/setters
    /// \fn void addShape(blMppShape *new_Shape);
    /// \brief add a shape to the grid
    /// \param[in] new_Shape Shape to add
    void addShape(blMppShapeGraph *new_Shape);
    /// \fn void setData(std::vector<std::vector<blMppShape*> >* data);
    /// \param[in] data Grid content
    void setData(std::vector<std::vector<blMppShapeGraph*> >* data);
    std::vector<std::vector<blMppShapeGraph*> >* configData();

public:
    // operations
    /// \brief return true if there is at least one interaction with "new_Shape"
    /// \param[in] new_Shape Shape to test
    /// \param[in] interaction Interaction class
    bool isInteraction(blMppShape *new_Shape, blMppInteraction *interaction);
    /// \fn void clear();
    /// \brief Clear the grid (remove all the shapes). This does not free the shape pointers
    void clear();

protected:
    // internal data
    std::vector<std::vector<blMppShapeGraph*> >* m_config; ///< grid content
};
