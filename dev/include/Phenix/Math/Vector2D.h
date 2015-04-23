/*******************************************************************************
 * @brief	ÏòÁ¿²Ù×÷
 * @date	2015-4-7
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MATH_VECTOR2D_H
#define PHENIX_MATH_VECTOR2D_H

#include <math.h>

namespace Phenix
{
namespace Math
{
	struct Vector2D
	{	  
		double x, y;	  

		Vector2D(double a = 0.0, double b = 0.0):x(a),y(b){}
		Vector2D(const Vector2D &v)
		{
			x = v.x;
			y = v.y;
		}

		//  returns the length of a 2D vector
		inline double    Length();
		//  returns the squared length of a 2D vector
		inline double    LengthSq();
		//  normalizes a 2D Vector
		inline void      Normalize();
		//  calculates the dot product
		inline double    Dot(const Vector2D &v2);
		//  returns positive if v2 is clockwise of this vector, minus if anticlockwise
		inline Phenix::Int32  Sign(const Vector2D &v2);
		//  Returns a vector perpendicular to the parameter
		inline Vector2D  Perp();	  
		//  calculates the euclidean distance between two vectors (treated as points)
		inline double    Distance(const Vector2D &v2) const;
		//  calculates the euclidean distance squared between two vectors (treated as points)
		inline double    DistanceSq(const Vector2D &v2);
		//  truncates a vector so that its length does not exceed max
		inline void      Truncate(const double max);

		//we need some overloaded operators
		Vector2D &operator+=(const Vector2D &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2D &operator-=(const Vector2D &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2D &operator*=(const double &rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		Vector2D &operator/=(const double &rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

		bool operator!()
		{
			if ( (x==0) && (y==0) )
			{
				return true;
			}
			return false;
		}
	};

	inline double Vector2D::Length()
	{
		return sqrt(x * x + y * y);
	}

	inline double Vector2D::LengthSq()
	{
		return (x * x + y * y);
	}

	inline void Vector2D::Normalize()
	{	 
		double vector_length = this->Length();
		if (vector_length < 0.00000001) 
			return;
		this->x = this->x / vector_length;
		this->y = this->y / vector_length;
	}

	inline double Vector2D::Dot(const Vector2D &v2)
	{
		return x*v2.x + y*v2.y;
	}

	inline Phenix::Int32 Vector2D::Sign(const Vector2D &v2)
	{
		if (y*v2.x > x*v2.y)
		{ 
			return 1;
		}
		else 
		{
			return -1;
		}
	}

	inline Vector2D Vector2D::Perp()
	{
		Vector2D perp;
		perp.x = -y;
		perp.y = x;
		return perp;
	}

	inline double Vector2D::Distance(const Vector2D &v2) const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;
		return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
	}

	inline double Vector2D::DistanceSq(const Vector2D &v2)
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;	
		return ySeparation*ySeparation + xSeparation*xSeparation;
	}
	
	inline void Vector2D::Truncate(const double max)
	{
		if (this->Length() > max)
		{
			this->Normalize();
			*this *= max;
		} 
	}


	//overload the * operator
	inline Vector2D operator*(const Vector2D &lhs, double rhs)
	{
		Vector2D result(lhs);
		result *= rhs;
		return result;
	}

	inline Vector2D operator*(double lhs, const Vector2D &rhs)
	{
		Vector2D result(rhs);
		result *= lhs;
		return result;
	}

	//overload the - operator
	inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
	{
		Vector2D result(lhs);
		result.x -= rhs.x;
		result.y -= rhs.y;
	  
	  return result;
	}

	//overload the + operator
	inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
	{
		Vector2D result(lhs);
		result.x += rhs.x;
		result.y += rhs.y;	  
		return result;
	}

	//overload the / operator
	inline Vector2D operator/(const Vector2D &lhs, const double val)
	{
		Vector2D result(lhs);
		result.x /= val;
		result.y /= val;
		return result;
	}
}	// end namespace Phenix
}	// end namespace Math

#endif