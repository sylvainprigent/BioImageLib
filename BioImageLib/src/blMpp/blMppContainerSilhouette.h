/// \file blMppContainerSilhouette.h
/// \brief blMppContainerSilhouette class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once
#include <blCore>
#include <vector>

class blMppContainerSilhouetteLine;
// //////////////////////////////////////////////// //
//             blMppContainerSilhouette
// /////////////////////////////////////////////// //
/// \class blMppContainerSilhouette
/// \brief contains the silouette of a shape. The silhouette
/// is defined as set of points pairs defining the entrance and the exit
/// in the shape when browsing a square around the shape
class blMppContainerSilhouette{

public:
    /// \fn blMppContainerSilhouette();
    /// \brief Constructor
    blMppContainerSilhouette();
    /// \fn virtual ~blMppContainerSilhouette();
    /// \brief Destructor
    virtual ~blMppContainerSilhouette();

public:
    // getters/setters
    /// \fn void setData(std::vector<blMppContainerSilhouetteLine> *data);
    /// \param[in] data Pointer to the sihouette content
    void setData(std::vector<blMppContainerSilhouetteLine> *data);
    /// \fn std::vector<blMppContainerSilhouetteLine> *data();
    /// \return the silhouette content
    std::vector<blMppContainerSilhouetteLine> *data();
    /// \fn void addLine(const blMppContainerSilhouetteLine &line);
    /// \param[in] line Line to be added to the silhouette
    void addLine(const blMppContainerSilhouetteLine &line);
    /// \fn void addLine(int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel);
    /// \param[in] lineIdx Index of the line to add
    /// \param[in] leftPixel Left pixel of the line to add
    /// \param[in] rightPixel Right pixel of the line to add
    void addLine(int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel);
    /// \fn int area();
    /// \return The number of pixels inside the silhouette
    int area();
    /// \fn void setArea(int area);
    /// \param[in] area The number of pixels inside the silhouette
    void setArea(int area);

public:
    //operations
    /// \fn bool isOverlaping(blMppContainerSilhouette *other, float allowedPurcentage);
    /// \brief Calculate if two silhouette overlap
    /// \param[in] other Silhouette to test with this silhouette
    /// \param[in] allowedPurcentage Allowed purcentage before considering the silhouette as overlaping
    bool isOverlaping(blMppContainerSilhouette *other, float allowedPurcentage);
    /// \fn bool isInside(blPixel pixel);
    /// \brief Test if a pixel is inside the silhouette
    /// \param[in] pixel Pixel to test
    bool isInside(blPixel pixel);
    /// \fn float purcentageOverlaping(blMppContainerSilhouette *other);
    /// \brief Calculate the purcentage of overlaping between two silhouettes
    /// \param[in] other Silhouette to test with this silhouette
    /// \return the puercentage of overlaping
    float purcentageOverlaping(blMppContainerSilhouette *other);
    /// \fn blMppContainerSilhouette* translate(blPixel);
    /// \brief Translate a silhouette
    /// \param[in] t Translation factor
    /// \return a pointer to the translated silhouette
    blMppContainerSilhouette* translate(blPixel t);
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
    blMppContainerSilhouette* copy();

protected:
    // internal
    /// \fn void findFirstCommonLine(blMppContainerSilhouette* other, int &m, int &n);
    /// \brief Search the first line in common between two silhouette
    /// \param[in] other Silhouette to test with this silhouette
    /// \param[out] m Position of the first common line in this silhouette
    /// \param[out] n Position of the first common line in "other" silhouette
    void findFirstCommonLine(blMppContainerSilhouette* other, int &m, int &n);

protected:
    std::vector<blMppContainerSilhouetteLine> *m_data; ///< list of points pairs constituting the silhouette
    int m_area; ///< Number of pixels in the silhouette
};


// //////////////////////////////////////////////// //
//             blMppContainerSilhouetteLine
// /////////////////////////////////////////////// //
/// \class blMppContainerSilhouetteLine
/// \brief Define a container of a single line of a silhouette
class blMppContainerSilhouetteLine{

public:
    /// \fn blMppContainerSilhouetteLine();
    /// \brief Constructor
    blMppContainerSilhouetteLine();
    /// \fn blMppContainerSilhouetteLine(int lineIdx);
    /// \brief Constructor
    /// \param[in] lineIdx Index of this line in the silhouette
    blMppContainerSilhouetteLine(int lineIdx);
    /// \fn blMppContainerSilhouetteLine(int lineIdx, blPixel leftPixel, blPixel rightPixel);
    /// \brief Constructor
    /// \param[in] lineIdx Index of this line in the silhouette
    /// \param[in] leftPixel Left pixel of the line
    /// \param[in] rightPixel Right pixel of the line
    blMppContainerSilhouetteLine(int lineIdx, blPixel leftPixel, blPixel rightPixel);

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
    /// \fn blMppContainerSilhouetteLine translate(blPixel t);
    /// \brief translate the line of a factor "t"
    /// \param[in] t Factor of the translation
    blMppContainerSilhouetteLine translate(blPixel t);
    /// \fn void setData(int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right);
    /// \brief Set the content of the line
    /// \param[in] lineIdx Index of this line
    /// \param[in] left List of the left points
    /// \param[in] left List of the right points
    void setData(int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right);
    /// \fn void clear();
    /// \brief Remove all the content of this line
    void clear();

protected:
    int m_lineIdx; ///< line index
    std::vector<blPixel> m_pixelsLeft; ///< List of the left points
    std::vector<blPixel> m_pixelsRight; ///< List of the right points
};
