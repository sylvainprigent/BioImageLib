/// \file blMppAlgorithm.h
/// \brief blMppAlgorithm class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppGeneratorShape.h"
#include "blMppDataTerm.h"
#include "blMppInteraction.h"
#include <blCore/blTableNumber.h>
#include "blMppExport.h"

/// \class blMppAlgorithm
/// \brief Class defing the basic functions needed to implement a Marked Point Process Algorithm.
/// This class need a Shape generator derivated from blMppGeneratorShape to generate
/// shapes, blMppdataTerm to compute the dataTerm term, and blMppInteraction to evaluate
/// interactions between pairs of objects
class BLMPP_EXPORT blMppAlgorithm : public blProcess{

public:
    /// \fn blMppAlgorithm();
    /// \brief Constructor
    blMppAlgorithm();
    /// \fn blMppAlgorithm(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \brief Constructor
    /// \param[in] shapeGenerator Random generator of shapes
    /// \param[in] dataTerm Pointer to a class that calculates the data term of a single shape
    /// \param[in] interaction Pointer to a class that calculates the interaction between two shapes
    blMppAlgorithm(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \fn virtual ~blMppAlgorithm();
    virtual ~blMppAlgorithm();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    virtual std::string name();

public:
    // setters
    /// \fn void setShapeGenerator(blMppGeneratorShape *shapeGenerator);
    /// \brief set the shape generator
    /// \param[in] shapeGenerator Random generator of shapes
    void setShapeGenerator(blMppGeneratorShape *shapeGenerator);
    /// \fn void setdataTerm(blMppDataTerm *dataTerm);
    /// \brief set the data term
    /// \param[in] dataTerm Pointer to a class that calculates the data term of a single shape
    void setdataTerm(blMppDataTerm *dataTerm);
    /// \fn void setInteraction(blMppInteraction* interaction);
    /// \brief set the shape pair interaction
    /// \param[in] interaction Pointer to a class that calculates the interaction between two shapes
    void setInteraction(blMppInteraction* interaction);
    /// \fn void setShapeMaxLength(int shapeMaxLength);
    /// \brief set the maximum length that the generator can generate.
    /// this parameter is used to build a hash table to speed up the
    /// interaction evaluation process
    /// \param[in] shapeMaxLength maximum length that the generator can generate
    void setShapeMaxLength(int shapeMaxLength);

public:
    // virtuals
    /// \fn virtual void compute() = 0;
    /// \brief start/run the algorithm
    virtual void compute() = 0;
    /// \fn virtual std::vector<blMppShape*> GetShapes() = 0;
    /// \return the list of shapes that the algorithm found in the image
    virtual std::vector<blMppShape*> GetShapes() = 0;

    /// \fn virtual void connectDataTermGenerator();
    /// \brief implement a connection that allow the data term class and the
    /// generator class to exchange information
    virtual void connectDataTermGenerator();

public:
    // outputs
    /// \fn blTableNumber* getShapesTable();
    /// \brief return the output shapes list as a table
    blTableNumber* getShapesTable();

protected:
    blMppGeneratorShape* m_shapeGenerator; ///< pointer to the shape generator object
    blMppDataTerm* m_dataTerm; ///< Pointer to the data term object
    blMppInteraction* m_interaction; ///< Pointer to the interaction object
    int m_shapeMaxLength; ///< maximum possible length of an object
};
