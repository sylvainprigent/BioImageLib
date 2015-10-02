/// \file blTxtIO.h
/// \brief blTxtIO class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#ifndef blTxtIO_H
#define blTxtIO_H

#include <vector>
#include <string>

#include "blTxtIOExport.h"

/// \class blTxtIO
/// \brief class BLCORE_EXPORT containing static function to load/write various type of data into txt files
class BLTXTIO_EXPORT blTxtIO{
public:
    /// \fn static void SaveShapesIntoTxtFile(std::vector< std::vector<float> > Shapes, std::string filePath);
    /// \brief Function that save mpp Shapes into a txt file
    /// \param[in] Shapes List of Shapes
    /// \param[in] filePath Adress of the saving file
    static void SaveShapesIntoTxtFile(std::vector< std::vector<float> > Shapes, std::string filePath);

    /// \fn static void SaveShapesIntoTxtFile(std::vector<std::vector< std::vector<float> > > Shapes, std::vector<std::string> ShapesTypeName, std::string filePath, bool useSingleFile = true);
    /// \brief Function that save mpp Shapes into a txt file
    /// \param[in] Shapes List of Shapes
    /// \param[in] ShapesTypeName List of Shapes names
    /// \param[in] filePath Adress of the saving file
    /// \param[in] useSingleFile True if all the shapes are in a single file
    static void SaveShapesIntoTxtFile(std::vector<std::vector< std::vector<float> > > Shapes, std::vector<std::string> ShapesTypeName, std::string filePath, bool useSingleFile = true);

    /// \fn static std::vector<std::vector<float> > LoadShapeFromTxtFile(std::string filePath, int ShapeSize);
    /// \brief Function that loads mpp Shapes from a txt file
    /// \param[in] filePath Adress of the txt file
    /// \param[in] ShapeSize size (ie number of parameters) of the Shape
    /// \return std::vector containing the Shapes
    static std::vector<std::vector<float> > LoadShapeFromTxtFile(std::string filePath, int ShapeSize);

    /// \fn static std::vector<std::vector<float> > LoadShapeFromTxtFile(std::string filePath);
    /// \brief Function that loads mpp Shapes from a txt file
    /// \param[in] filePath Adress of the txt file
    /// \return std::vector containing the Shapes
    static std::vector<std::vector<float> > LoadShapeFromTxtFile(std::string filePath);
};


#endif
