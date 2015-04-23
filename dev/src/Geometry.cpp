#include "stdafx.h"
#include <Phenix/Math/Geometry.h>

namespace Phenix
{
namespace Math
{

	double Geometry::DistToLineSegment( const Vector2D& A, const Vector2D& B, const Vector2D& C )
	{
		//if the angle is obtuse between CA and AB is obtuse then the closest
		//vertex must be A
		double dotA = (C.x - A.x)*(B.x - A.x) + (C.y - A.y)*(B.y - A.y);
		if (dotA <= 0) 
			return A.Distance(C);
		//if the angle is obtuse between CB and AB is obtuse then the closest
		//vertex must be B
		double dotB = (C.x - B.x)*(A.x - B.x) + (C.y - B.y)*(A.y - B.y);
		if (dotB <= 0) 
			return B.Distance(C);
		//calculate the point along AB that is the closest to C
		Vector2D P = A + ((B - A) * dotA)/(dotA + dotB);
		//calculate the distance CP
		return C.Distance(P);
	}

	bool Geometry::LineIntersection2D(const Vector2D A, const Vector2D B, const Vector2D C, const Vector2D D)
	{
		double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
		double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
		double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
		double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

		if ( (rBot == 0) || (sBot == 0))
		{
			//lines are parallel
			return false;
		}

		double r = rTop/rBot;
		double s = sTop/sBot;
		if((r > 0) && (r < 1) && (s > 0) && (s < 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Geometry::LineIntersection2D(const Vector2D A, const Vector2D B, const Vector2D C, const Vector2D D,
								double &dist)
	{
		double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
		double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
		double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
		double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
		if ( (rBot == 0) || (sBot == 0))
		{
			//lines are parallel
			return false;
		}
		double r = rTop/rBot;
		double s = sTop/sBot;
		if((r > 0) && (r < 1) && (s > 0) && (s < 1))
		{
  			dist = r;
			return true;
		}
		else
		{
			dist = 0;
			return false;
		}
	}

	bool Geometry::LineIntersection2D(const Vector2D A, const Vector2D B, const Vector2D C, const Vector2D D,
									double &dist, Vector2D &point)
	{
		double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
		double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
		double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
		double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
		if ( (rBot == 0) || (sBot == 0))
		{
			//lines are parallel
			return false;
		}
		double r = rTop/rBot;
		double s = sTop/sBot;
		if((r > 0) && (r < 1) && (s > 0) && (s < 1))
		{
  			dist = r;
			point = A + r * (B - A);
			return true;
		}
		else
		{
			dist = 0;
			return false;
		}
	}

	bool Geometry::ObjectIntersection2D(const std::vector<Vector2D>& object1, const std::vector<Vector2D>& object2)
	{
		//test each line segment of object1 against each segment of object2
		Phenix::Int32 num1 = object1.size();
		for (Phenix::Int32 r=0; r<num1-1; ++r)
		{
			Phenix::Int32 num2 = object2.size();
			for (Phenix::Int32 t=0; t<num2-1; ++t)
			{
				if (LineIntersection2D(object2[t], object2[t+1], object1[r], object1[r+1]))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Geometry::SegmentObjectIntersection2D(const Vector2D& A, const Vector2D& B, const std::vector<Vector2D>& object)
	{
		//test AB against each segment of object
		Phenix::Int32 num = object.size();
		for (Phenix::Int32 r=0; r<num-1; ++r)
		{
			if (LineIntersection2D(A, B, object[r], object[r+1]))
			{
				return true;
			}
		}
		return false;
	}

	bool Geometry::TwoCirclesOverlapped(const double x1, const double y1, const double r1,
							  const double x2, const double y2, const double r2)
	{
		double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
		if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
		{
			return true;
		}
		return false;
	}

	bool Geometry::TwoCirclesEnclosed(double x1, double y1, double r1, double x2, double y2, double r2)
	{
		double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
		if (DistBetweenCenters < fabs(r1-r2))
		{
			return true;
		}
		return false;
	}

	bool Geometry::TwoCirclesIntersectionPoints(double x1, double y1, double r1, double x2, double y2, double r2,
									  double &p3X, double &p3Y, double &p4X, double &p4Y)
	{
		//first check to see if they overlap
		if (!TwoCirclesOverlapped(x1,y1,r1,x2,y2,r2))
		{
			return false;
		}

		//calculate the distance between the circle centers
		double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
	  
		//Now calculate the distance from the center of each circle to the center
		//of the line which connects the intersection points.
		double a = (r1 - r2 + (d * d)) / (2 * d);
		//double b = (r2 - r1 + (d * d)) / (2 * d); 

		//MAYBE A TEST FOR EXACT OVERLAP? 

		//calculate the point P2 which is the center of the line which 
		//connects the intersection points
		double p2X, p2Y;
		p2X = x1 + a * (x2 - x1) / d;
		p2Y = y1 + a * (y2 - y1) / d;

		//calculate first point
		double h1 = sqrt((r1 * r1) - (a * a));
		p3X = p2X - h1 * (y2 - y1) / d;
		p3Y = p2Y + h1 * (x2 - x1) / d;
		
		//calculate second point
		double h2 = sqrt((r2 * r2) - (a * a));
		p4X = p2X + h2 * (y2 - y1) / d;
		p4Y = p2Y - h2 * (x2 - x1) / d;

		return true;
	}

	double Geometry::TwoCirclesIntersectionArea(double x1, double y1, double r1, double x2, double y2, double r2)
	{
		//first calculate the intersection points
		double iX1, iY1, iX2, iY2;
		if(!TwoCirclesIntersectionPoints(x1,y1,r1,x2,y2,r2,iX1,iY1,iX2,iY2))
		{
			return 0; //no overlap
		}

		//calculate the distance between the circle centers
		double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));

		//find the angles given that A and B are the two circle centers
		//and C and D are the intersection points
		double CBD = 2 * acos((r2*r2 + d*d - r1*r1) / (r2 * d * 2)); 
		double CAD = 2 * acos((r1*r1 + d*d - r2*r2) / (r1 * d * 2));

		//Then we find the segment of each of the circles cut off by the 
		//chord CD, by taking the area of the sector of the circle BCD and
		//subtracting the area of triangle BCD. Similarly we find the area
		//of the sector ACD and subtract the area of triangle ACD.

		double area = 0.5*CBD*r2*r2 - 0.5*r2*r2*sin(CBD) + 0.5*CAD*r1*r1 - 0.5*r1*r1*sin(CAD);
		
		return area;
	}

	double Geometry::CircleArea(double radius)
	{
		return Pi * radius * radius;
	}

	bool Geometry::PointInCircle(const Vector2D Pos, const double radius, const Vector2D p)
	{
		double DistFromCenterSquared = (p-Pos).LengthSq();
		if (DistFromCenterSquared < (radius*radius))
		{
			return true;
		}
		return false;
	}

	bool Geometry::LineCircleIntersection(const Vector2D A, const Vector2D B, const Vector2D P, const double radius)
	{
		//first determine the distance from the center of the circle to
		//the line segment
		double DistToLine = DistToLineSegment(A, B, P);
		if (DistToLine < radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


} // end namespace Math
} // end namespace Phenix