/*******************************************************************************
 * @brief	几何计算
 * @date	2015-4-7
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MATH_GEOMETRY_H
#define PHENIX_MATH_GEOMETRY_H

#include "Utils.h"
#include "Vector2D.h"

namespace Phenix
{
namespace Math
{

	class Geometry
	{
	public:
		// C点到AB的垂线距离
		static double DistToLineSegment(const Vector2D& A,	const Vector2D& B, const Vector2D& C);
		// AB,CD是否相交
		static bool LineIntersection2D(const Vector2D A, const Vector2D B, const Vector2D C, const Vector2D D);
		//Given 2 lines in 2D space AB, CD this returns true if an 
		//intersection occurs and sets dist to the distance the intersection
		//occurs along AB
		static bool LineIntersection2D(const Vector2D A, const Vector2D B,	const Vector2D C, const Vector2D D,
										double &dist);
		//Given 2 lines in 2D space AB, CD this returns true if an 
		//intersection occurs and sets dist to the distance the intersection
		//occurs along AB. Also sets the 2d vector point to the point of
		//intersection
		static bool LineIntersection2D(const Vector2D A, const Vector2D B,	const Vector2D C, const Vector2D D,
										double &dist, Vector2D &point);
		//  tests two polygons for intersection. *Does not check for enclosure*
		static bool ObjectIntersection2D(const std::vector<Vector2D>& object1, const std::vector<Vector2D>& object2);
		//give the line segment AB and an object, this function returns true if they intersect， Does not check for enclosure		
		static bool SegmentObjectIntersection2D(const Vector2D& A,	const Vector2D& B, const std::vector<Vector2D>& object2);
		//  Returns true if the two circles overlap
		static bool TwoCirclesOverlapped(const double x1, const double y1, const double r1, const double x2, 
										const double y2, const double r2);
		//  returns true if one circle encloses the other
		static bool TwoCirclesEnclosed(double x1, double y1, double r1, double x2, double y2, double r2);
		//  Given two circles this function calculates the intersection points of any overlap.		
		//  returns false if no overlap found
		static bool TwoCirclesIntersectionPoints(double x1, double y1, double r1, double x2, double y2, double r2,
										double &p3X, double &p3Y, double &p4X, double &p4Y);
		//  Tests to see if two circles overlap and if so calculates the area defined by the union
		static double TwoCirclesIntersectionArea(double x1, double y1, double r1, double x2, double y2, double r2);
		static double CircleArea(double radius);
		//  returns true if the point p is within the radius of the given circle
		static bool   PointInCircle(const Vector2D Pos, const double radius, const Vector2D p); 
		//  returns true if the line segemnt AB intersects with a circle at position P with radius radius
		static bool   LineCircleIntersection(const Vector2D A,	const Vector2D B, const Vector2D P,	const double radius);
	};

} // end namespace Math
} // end namespace Phenix


#endif

              




















