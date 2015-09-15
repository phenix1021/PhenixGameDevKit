/*******************************************************************************
 * @brief	Bloom Filter
 * @date	2015-9-15
 * @author	phenix
 * @mark    
			False Positive�������ԣ�������ΪĿ������ڻ����У��㷨��һ�����ʳ��֡�
			False Negative�������ԣ�������ΪĿ�겻�����ڻ����У��㷨0���ʳ��֡�
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
		std::size_t _curSize;		// ��ǰ���ݸ�������������
		std::size_t _n;				// ��֤Ŀǰ_bitsλ�� �� False Positive ������֧�ֵ�������ݸ���
		std::size_t	_k;				// ��ͬ��hash�������� ���� ͬһhash����Ҫhash�Ĵ���
		float _falsePositiveRate;	// ʵ�ʵ�False Positive����

		std::bitset<BITS>	_bits;
		Phenix::HashCode	_hash;
	};

}
}


#endif