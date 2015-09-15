/*******************************************************************************
 * @brief	Bloom Filter
 * @date	2015-9-15
 * @author	phenix
 * @mark    
			False Positive：假阳性，即误认为目标存在于缓存中；算法有一定概率出现。
			False Negative：假阴性，即误认为目标不存在于缓存中；算法0概率出现。
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_BLOOM_FILTER_H
#define PHENIX_COLLECTION_BLOOM_FILTER_H

#include <bitset>
#include <math.h>
#include <Phenix/Base/Hash.h>

namespace Phenix
{
namespace Collection
{
	template<std::size_t BYTES>
	class BloomFilter
	{
		enum
		{
			BITS = BYTES << 3;
		};
	public:
		BloomFilter(double expectdFalsePositiveRate)
		{
			_n = BITS / (1.44 * log(1/expectdFalsePositiveRate)/log(2));
			_k = 0.693 * BITS / _n;
			_falsePositiveRate = (1 - exp(-1 * _k * _n / BITS)) ** _k;
		}

		virtual ~BloomFilter(){}
		
		inline bool add(const Phenix::String& str)
		{
			if (_curSize >= _n)
			{
				return false;
			}
			static Phenix::String& salt = "phenix";
			for (std::size_t i=0; i<_k; ++i)
			{
				std::size_t bit = _hash(str+salt.substr(i % salt.size(), 1), BITS);
				if (_bits.test(bit) == 0)
				{
					_bits.set(bit);
					++_curSize;
					return true;
				}
			}
			return false;
		}

		inline bool add(const size_t val)
		{
			if (_curSize >= _n)
			{
				return false;
			}			
			for (std::size_t i=0; i<_k; ++i)
			{
				std::size_t bit = _hash(val+i, BITS);
				if (_bits.test(bit) == 0)
				{
					_bits.set(bit);
					++_curSize;
					return true;
				}
			}
			return false;
		}
		
		inline bool has(const Phenix::String& str)
		{
			static Phenix::String& salt = "phenix";
			for (std::size_t i=0; i<_k; ++i)
			{
				std::size_t bit = _hash(str+salt.substr(i % salt.size(), 1), BITS);
				if (_bits.test(bit) == 0)
				{
					return false;
				}
			}
			return true;
		}

		inline bool has(const size_t val)
		{			
			for (std::size_t i=0; i<_k; ++i)
			{
				std::size_t bit = _hash(val+i, BITS);
				if (_bits.test(bit) == 0)
				{					
					return false;
				}
			}
			return true;
		}

	private:
		std::size_t _curSize;		// 当前数据个数（含误报数）
		std::size_t _n;				// 保证目前_bits位数 和 False Positive 比率下支持的最大数据个数
		std::size_t	_k;				// 不同的hash函数个数 或者 同一hash函数要hash的次数
		float _falsePositiveRate;	// 实际的False Positive比率

		std::bitset<BITS>	_bits;
		Phenix::HashCode	_hash;
	};

}
}


#endif