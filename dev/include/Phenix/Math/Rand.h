/*******************************************************************************
 * @brief	随机数发生器
 * @date	2015-4-4
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MATH_RAND_H
#define PHENIX_MATH_RAND_H

#include <math.h>

namespace Phenix
{
namespace Math
{
	class Random
	{
	public:
		// return a random int in the range [x, y]
		inline static Phenix::Int32	RandInt(Phenix::Int32 x, Phenix::Int32 y)
		{
			return rand()%(y-x+1)+x;
		}

		// return a random double in the range [0, 1)
		inline static double RandFloat()
		{
			return (rand())/(RAND_MAX+1.0);		
		}

		// return a random double in the range [0, 1]
		inline static double RandFloatEx()
		{
			return (rand())/double(RAND_MAX);
		}

		// returns a random float in the range (-1, 1)
		inline static double RandClamped()
		{
			return RandFloat() - RandFloat();
		}

		// returns a random float in the range [-1, 1]
		inline static double RandClampedEx()
		{
			return RandFloatEx() - RandFloatEx();
		}

		// return a random bool
		inline static bool RandBool()
		{
			return RandInt(0, 1) > 0;
		}		

		// 产生一个符合正态分布规律的随机数
		inline static double RandGaussian(double mean/*平均值*/ = 0.0, double standard_deviation/*标准差*/ = 1.0)
		{				        
			double x1, x2, w, y1;
			static double y2;
			static int use_last = 0;

			if (use_last)		        /* use value from previous call */
			{
				y1 = y2;
				use_last = 0;
			}
			else
			{
				do 
				{
					x1 = 2.0 * RandFloat() - 1.0;
					x2 = 2.0 * RandFloat() - 1.0;
					w = x1 * x1 + x2 * x2;
				}
				while ( w >= 1.0 );

				w = sqrt( (-2.0 * log( w ) ) / w );
				y1 = x1 * w;
				y2 = x2 * w;
				use_last = 1;
			}

			return( mean + y1 * standard_deviation );
		}
	};

} // end namespace Math
} // end namespace Phenix

#endif