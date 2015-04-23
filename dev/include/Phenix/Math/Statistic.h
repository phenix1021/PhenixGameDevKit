/*******************************************************************************
 * @brief	统计学
 * @date	2015-4-4
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MATH_STATISTIC_H
#define PHENIX_MATH_STATISTIC_H

#include <math.h>

namespace Phenix
{
namespace Math
{
	class Stat
	{
	public:
		inline static double Average(const std::vector<double>& v)
		{
			double average = 0.0;

			for (int i=0; i<v.size(); ++i)
			{    
				average += v[i];
			}

			return average / v.size();
		}

		// 方差
		inline static double Deviation(const std::vector<double>& v)
		{
			double sd      = 0.0;
			double average = Average(v);

			for (int i=0; i<v.size(); ++i)
			{     
				sd += (v[i] - average) * (v[i] - average);
			}

			return sd / v.size();
		}

		// 标准差
		inline static double StandardDeviation(const std::vector<double>& v)
		{			
			return sqrt(Deviation(v));
		}
	};

} // end namespace Math
} // end namespace Phenix

#endif