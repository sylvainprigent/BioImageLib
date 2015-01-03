/// \file blMathGeometry.h
/// \brief blMathGeometry class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>

/// \class blMathGeometry
/// \brief Define geometrical shapes
class blMathGeometry
{
public:
    static void AddPixel(int x, int y, std::vector<int> &xp, std::vector<int> &yp);
    static void AddPixel(int x, int y, std::vector<std::vector<int> > &coords);

public:
    /// \fn static void CalculateCircleCoordinates( int x_centre, int y_centre, int r, std::vector<int>& xp, std::vector<int>& yp );
    /// \brief Function that calculate the coordinates on a circle
    /// \param[in] x_center Circle center in the x-axis
    /// \param[in] y_center Circle center in the y-axis
    /// \param[in] r Circle radius
    /// \param[out] xp Coordinates of the circle points in the x-axis
    /// \param[out] yp Coordinates of the circle points in the y-axis
    static void CalculateCircleCoordinates( int x_centre, int y_centre, int r, std::vector<int>& xp, std::vector<int>& yp );

    /// \brief static void CalculateCircleCoordinates(int x_center, int y_center, int r, std::vector<std::vector<int> > &coords)
    /// \param[in] x_center Circle center in the x-axis
    /// \param[in] y_center Circle center in the y-axis
    /// \param[in] r Circle radius
    /// \param[out] coords Coordinates of the circle points
    static void CalculateCircleCoordinates(int x_center, int y_center, int r, std::vector<std::vector<int> > &coords);

    /// \fn static void Calculate2DLineCoordinates( int x1, int y1, int x2, int y2, std::vector<int> &px, std::vector<int> &py);
    /// \brief Function that compute the coordinates of points in a line
    /// \param[in] x1 X position of the starting point
    /// \param[in] y1 Y position of the starting point
    /// \param[in] x2 X position of the end point
    /// \param[in] y2 Y position of the end point
    /// \param[out] xp Coordinates of the line points in the x-axis
    /// \param[out] yp Coordinates of the line points in the y-axis
    static void Calculate2DLineCoordinates( int x1, int y1, int x2, int y2, std::vector<int> &px, std::vector<int> &py);

    /// \fn static void CalculateRectangleCorners(std::vector<float> &xp, std::vector<float> &yp, std::vector<float> A, std::vector<float> B, int width);
    /// \brief Function that calculates a rectangle corner coordinates in 2D from the centers of the small sides
    /// \param[out] xp x coordinates of the rectangle corners
    /// \param[out] yp y coordinates of the rectangle corners
    /// \param[in] A center of the first small side
    /// \param[in] B center of the second small side
    /// \param[in] width size of the corner small side
    static void CalculateRectangleCorners(std::vector<float> &xp, std::vector<float> &yp, std::vector<float> A, std::vector<float> B, int width);

    /// \fn static void Calculate3dLineCoordinates(std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz, std::vector<int> start_pts, std::vector<int> end_pts);
    /// \brief Function that calculate the coordinates of points that are in a line in 3D
    /// \param[out] px X coordinates of the founded points
    /// \param[out] py Y coordinates of the founded points
    /// \param[out] pz Z coordinates of the founded points
    /// \param[in] start_pts Coordinates of the starting point
    /// \param[in] end_point Coordinates of the ending point
    static void Calculate3dLineCoordinates(std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz, std::vector<int> start_pts, std::vector<int> end_pts);

    /// \fn static std::vector<float> LinSpace(float d1, float d2, int n);
    /// \brief internal method for calculate points in a line
    /// \param[in] d1 Point 1
    /// \param[in] d2 Point 2
    /// \param[in] n n
    static std::vector<float> LinSpace(float d1, float d2, int n);
    /// \fn static std::vector<int> LinSpaceRound(float d1, float d2, int n);
    /// \brief internal method for calculate points in a line
    /// \param[in] d1 Point 1
    /// \param[in] d2 Point 2
    /// \param[in] n n
    static std::vector<int> LinSpaceRound(float d1, float d2, int n);

    /// \fn static void CalculatePixelsCoordinateBorderSphere(std::vector<float> sphere, int borderWidth, std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz );
    /// \brief Function that calculates the coordinates of points defining a sphere
    /// \param[in] sphere std::vector containing the sphere parameters
    /// \param[in] borderWidth parameter controling the border size of the sphere
    /// \param[out] px X coordinates of the points in the sphere
    /// \param[out] py Y coordinates of the points in the sphere
    /// \param[out] pz Z coordinates of the points in the sphere
    static void CalculatePixelsCoordinateBorderSphere(std::vector<float> sphere, int borderWidth, std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz );

    /// \fn bool DoesEllipsesIntersect(std::vector<float> ellipse1, std::vector<float> ellipse2);
    /// \brief Calculate if two ellises intersect with a gradient descente on the equation
    /// \param[in] ellipse1 First ellipse
    /// \param[in] ellipse2 Second ellipse
    /// \return true if the ellipses overlap, zero otherwise
    static bool DoesEllipsesIntersect(std::vector<float> ellipse1, std::vector<float> ellipse2, double levelSet = 1.0);

    /// \fn bool DoesEllipsoidIntersect(std::vector<float> ellipsoid1, std::vector<float> ellipsoid2);
    /// \brief Calculate if two ellipsoid2 intersect with a gradient descente on the equation
    /// \param[in] ellipsoid1 First ellipse
    /// \param[in] ellipsoid2 Second ellipse
    /// \return true if the ellipsoid overlap, zero otherwise
    static bool DoesEllipsoidIntersect(std::vector<float> ellipsoid1, std::vector<float> ellipsoid2, double levelSet = 1.0);
};

/// \class blPoint2D
/// \brief class defining a 2D point
class blPoint2D
{
public:
    /// \fn blPoint2D(float x, float y);
    /// \brief Constructor
    /// \param[in] x Coordinate X
    /// \param[in] y Coordinate Y
    blPoint2D(float x, float y);
    /// \fn blPoint2D();
    /// \brief Constructor
    blPoint2D();

    /// \fn void setCoords(float x, float y);
    /// \brief Setter
    /// \param[in] x Coordinate X
    /// \param[in] y Coordinate Y
    void setCoords(float x, float y);
    /// \fn void setX(float x);
    /// \brief Setter
    /// \param[in] x Coordinate X
    void setX(float x);
    /// \fn void setY(float y);
    /// \brief Setter
    /// \param[in] y Coordinate Y
    void setY(float y);
    /// \fn float x();
    /// \brief Getter
    /// \return the X coordinate
    float x();
    /// \fn float y();
    /// \brief Getter
    /// \return the Y coordinate
    float y();

private:
    float m_x; ///< x coordinate
    float m_y; ///< y coordinate
};

/// \class blPolygon
/// \brief class defining a polygon
class blPolygon
{
public:
    /// \fn blPolygon();
    /// \brief Constructor
    blPolygon();

    /// \fn void addVertex(blPoint2D point);
    /// \brief add a vertex
    /// \param[in] point Coordinates of the vertex to add
    void addVertex(blPoint2D point);
    /// \fn int numSides();
    /// \brief getter
    /// \return the number of sides of the polygon
    int numSides();
    /// \fn blPoint2D center();
    /// \brief getter
    /// \return the center of the polygon
    blPoint2D center();
    /// \fn blPoint2D pointAt(int pos);
    /// \brief getter
    /// \param[in] pos position of the point in the polygon point list
    /// \return the coordinates of the points
    blPoint2D pointAt(int pos);

private:
    blPoint2D m_center; ///< center of the polygon
    int m_umSides; ///< number of sides of the polygon
    std::vector<blPoint2D> m_points; ///< vertex coordinates
};

/// \class blOverlapTests
/// \brief Test if two shapes overlap
class blOverlapTests
{
public:
    /// \fn static bool polypolyIntersect(Polygon *a, Polygon *b);
    /// \brief Tests for a collision between any two convex polygons.
    /// \return false if they do not intersect, true if they intersect
    static bool polypolyIntersect(blPolygon *a, blPolygon *b);

};
