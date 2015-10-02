/// \file blMppContainerSilhouette3D.h
/// \brief blMppContainerSilhouette3D class
/// \author Emmanuelle Poulain (Emmanuelle.Poulain@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include <blCore>
#include <vector>
#include "blMppExport.h"

class BLMPP_EXPORT blMppContainerSilhouette3DLine;
// //////////////////////////////////////////////// //
//             blMppContainerSilhouette3D
// /////////////////////////////////////////////// //
/// \class blMppContainerSilhouette3D
/// \brief contains the silouette of a shape. The silhouette
/// is defined as set of points pairs defining the entrance and the exit
/// in the shape when browsing a square around the shape for each (x,y) layer of
/// of the shape
class blMppContainerSilhouette3D{

public:
    /// \fn blMppContainerSilhouette3D();
    /// \brief Constructor
    blMppContainerSilhouette3D();
    /// \fn virtual ~blMppContainerSilhouette3D();
    /// \brief Destructor
    virtual ~blMppContainerSilhouette3D();

public:
    // getters/setters
    /// \fn void setData(vector<vector<blMppContainerSilhouette3DLine> > *data);
    /// \param[in] data Pointer to the sihouette content
    void setData(std::vector<std::vector<blMppContainerSilhouette3DLine> > *data);
    /// \fn vector<vector<blMppContainerSilhouette3DLine> > *data();
    /// \return the silhouette content
    std::vector<std::vector<blMppContainerSilhouette3DLine> > *data();
    /// \fn void addLine(int layoutidx,const blMppContainerSilhouette3DLine &line);
    /// \param[in] layoutidx Index of the layout to add a line
    /// \param[in] line Line to be added to the silhouette
    void addLine(int layoutidx,const blMppContainerSilhouette3DLine &line);
    /// \fn void addLine(int layOutIdx,int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel);
    /// \param[in] layoutidx Index of the layout to add a line
    /// \param[in] lineIdx Index of the line to add
    /// \param[in] leftPixel Left pixel of the line to add
    /// \param[in] rightPixel Right pixel of the line to add
    void addLine(int layOutIdx,int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel);
    /// \fn void addLayout();
    /// \brief Add a layout to this silhouette
    void addLayout();
    /// \fn int nbLayout();
    /// \return the number of layouts in this silhouette
    int nbLayout();
    /// \fn int area();
    /// \return The number of pixels inside the silhouette
    int area();
    /// \fn void setArea(int area);
    /// \param[in] area The number of pixels inside the silhouette
    void setArea(int area);
    /// \fn std::vector<blMppContainerSilhouette3DLine> operator[](int i);
    /// \param[in] i index of the layout to get
    /// \return the layout data
    std::vector<blMppContainerSilhouette3DLine> operator[](int i);


public:
    //operations
    /// \fn bool isOverlaping(blMppContainerSilhouette3D *other, float allowedPurcentage);
    /// \brief Calculate if two silhouette overlap
    /// \param[in] other Silhouette to test with this silhouette
    /// \param[in] allowedPurcentage Allowed purcentage before considering the silhouette as overlaping
    bool isOverlaping(blMppContainerSilhouette3D *other, float allowedPurcentage);
    /// \fn bool isInside(blPixel pixel);
    /// \brief Test if a pixel is inside the silhouette
    /// \param[in] pixel Pixel to test
    bool isInside(blPixel pixel);
    /// \fn float purcentageOverlaping(blMppContainerSilhouette3D *other);
    /// \brief Calculate the purcentage of overlaping between two silhouettes
    /// \param[in] other Silhouette to test with this silhouette
    /// \return the puercentage of overlaping
    float purcentageOverlaping(blMppContainerSilhouette3D *other);
    /// \fn blMppContainerSilhouette* translate(blPixel);
    /// \brief Translate a silhouette
    /// \param[in] t Translation factor
    /// \return a pointer to the translated silhouette
    blMppContainerSilhouette3D* translate(blPixel t);
    /// \fn void printConsole();
    /// \brief Print the content of a silhouette in the console
    void printConsole();
    /// \fn int silhouetteMaxLength();
    /// \return the maximum size of the silhouette (in all the directions)
    int silhouetteMaxLength();
    /// \fn void calculateArea();
    /// \brief Compute the number of pixels inside the silhouette
    void calculateArea();

public:
    //operations
    /// \fn std::vector<blPixel> *insidePixels();
    /// \return the number of pixels inside the silhouette
    std::vector<blPixel> *insidePixels();
    /// \fn blMppContainerSilhouette* copy();
    /// \return A pointer to a copy of this shilouette
    blMppContainerSilhouette3D* copy();

protected:
    // internal
    /// \fn void findFirstCommonLine(blMppContainerSilhouette3D* other,int layoutIdx1, int layoutIdx2, int &m, int &n));
    /// \brief Search the first line in common between two silhouette
    /// \param[in] other Silhouette to test with this silhouette
    /// \param[in] layoutIdx1 Index of the layout of this shape to test
    /// \param[in] layoutIdx2 Index of the layout of "other" shape to test
    /// \param[out] m Position of the first common line in this silhouette
    /// \param[out] n Position of the first common line in "other" silhouette
    void findFirstCommonLine(blMppContainerSilhouette3D* other,int layoutIdx1, int layoutIdx2, int &m, int &n);
    /// \fn void findFirstCommonLayout(blMppContainerSilhouette3D* other,int &layoutIdx1, int &layoutIdx2);
    /// \param[in] other Silhouette to test with this silhouette
    /// \param[in] layoutIdx1 Index of the first layout of this shape in common with "other"
    /// \param[in] layoutIdx2 Index of the first layout of "other" in common with this shape
    void findFirstCommonLayout(blMppContainerSilhouette3D* other,int &layoutIdx1, int &layoutIdx2);

protected:
    std::vector<std::vector<blMppContainerSilhouette3DLine> > *m_data; ///< list of points pairs constituting the silhouette
    int m_area; ///< Number of pixels in the silhouette
    int m_layoutIdx; ///< Index of the layout
    int m_nbLayout; ///< Number of layouts in the silhouette
};


// //////////////////////////////////////////////// //
//             blMppContainerSilhouette3DLine
// /////////////////////////////////////////////// //
/// \class blMppContainerSilhouette3DLine
/// \brief Define a container of a single line of a silhouette

class blMppContainerSilhouette3DLine{

public:
    /// \fn blMppContainerSilhouette3DLine();
    /// \brief Constructor
    blMppContainerSilhouette3DLine();
    /// \fn blMppContainerSilhouette3DLine(int lineIdx);
    /// \brief Constructor
    /// \param[in] lineIdx
    blMppContainerSilhouette3DLine(int lineIdx);
    /// \fn blMppContainerSilhouette3DLine(int layoutIdx, int lineIdx, blPixel leftPixel, blPixel rightPixel);
    /// \brief Constructor
    /// \param[in] layoutIdx Index of the layout to which this line belongs to in the silhouette
    /// \param[in] lineIdx Index of this line in the silhouette
    /// \param[in] leftPixel Left pixel of the line
    /// \param[in] rightPixel Right pixel of the line
    blMppContainerSilhouette3DLine(int layoutIdx, int lineIdx, blPixel leftPixel, blPixel rightPixel);

public:
    // getters / setters
    /// \fn void addPointPair(const blPixel & leftPixel, const blPixel & rightPixel);
    /// \brief Add a new point pair to the line. This allow to menage the non-convex shapes
    /// \param[in] leftPixel Left pixel of the point pair
    /// \param[in] rightPixel Right pixel of the point pair
    void addPointPair(const blPixel & leftPixel, const blPixel & rightPixel);
    /// \fn int pairNumber();
    /// \return the number of points pairs in this line
    int pairNumber();
    /// \fn int layoutIdx();
    /// \return the index of the layout to which this line belongs to in the silhouette
    int layoutIdx();
    /// \fn int lineIdx();
    /// \return the index of this line
    int lineIdx();
    /// \fn blPixel getPixelLeftAt(int i);
    /// \return the pixel of the point pair "i"
    /// \param[in] i Index of the point pair to get left pixel
    blPixel getPixelLeftAt(int i);
    /// \fn blPixel getPixelRightAt(int i);
    /// \return the pixel of the point pair "i"
    /// \param[in] i Index of the point pair to get right pixel
    blPixel getPixelRightAt(int i);
    /// \fn blMppContainerSilhouette3DLine translate(blPixel t);
    /// \brief translate the line of a factor "t"
    /// \param[in] t Factor of the translation
    blMppContainerSilhouette3DLine translate(blPixel t, int layoutId = 0);
    /// \fn void setData(int layoutIdx, int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right);
    /// \brief Set the content of the line
    /// \param[in] layoutIdx Index of the layout to which this line belongs to in the silhouette
    /// \param[in] lineIdx Index of this line
    /// \param[in] left List of the left points
    /// \param[in] left List of the right points
    void setData(int layoutIdx, int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right);
    /// \fn void clear();
    /// \brief Remove all the content of this line
    void clear();

protected:
    int m_lineIdx; ///< line index
    int m_layoutIdx; ///< z layout idx

    std::vector<blPixel> m_pixelsLeft; ///< List of the left points
    std::vector<blPixel> m_pixelsRight; ///< List of the right points
};
