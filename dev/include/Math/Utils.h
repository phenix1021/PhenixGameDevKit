/*******************************************************************************
 * @brief	数学工具
 * @date	2015-4-4
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MATH_UTILS_H
#define PHENIX_MATH_UTILS_H

#include <math.h>

namespace Phenix
{
namespace Math
{
	class Utils
	{
	public:		
		// 两个浮点数是否相等
		inline static bool Equal(double a, double b)
		{
			return (fabs(a-b) < 1E-12);			
		}		
		
		// Carmack在QUAKE3中使用的计算平方根的函数		
		inline static float FastSqrt(float x)
		{
			union
			{
				int intPart;
				float floatPart;
			} convertor;
			union
			{
				int intPart;
				float floatPart;
			} convertor2;
			convertor.floatPart = x;
			convertor2.floatPart = x;
			convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
			convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
			return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
		}
		
		// 来自Carmack在QUAKE3中使用的计算参数x的平方根的倒数的代码
		inline static float FastInvSqrt (float x)
		{
			float xhalf = 0.5f*x;
			int i = *(int*)&x;
			i = 0x5f3759df - (i >> 1); // 计算第一个近似根
			x = *(float*)&i;
			x = x*(1.5f - xhalf*x*x); // 牛顿迭代法
			return x;
		}

		//clamps the first argument between the second two
		inline static void Clamp(double &arg, const double min, const double max)
		{
			if (arg < min)
			{
				arg = min;
			}
			if (arg > max)
			{
				arg = max;
			}
		}

	};

} // end namespace Math
} // end namespace Phenix

#endif