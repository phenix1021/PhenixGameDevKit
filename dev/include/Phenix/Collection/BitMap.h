/*******************************************************************************
 * @brief	bit map
 * @date	2015-9-14
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_BIT_MAP_H
#define PHENIX_COLLECTION_BIT_MAP_H

#include <bitset>

namespace Phenix
{
namespace Collection
{
	template<std::size_t BYTES>
	class BitMap
	{
	public:
		BitMap(){}
		virtual ~BitMap(){}

		inline bool add(std::size_t val)
		{
			if (val > BYTES<<3)
			{
				return false;
			}
			_bits.set(val);
			return true;
		}

		inline bool remove(std::size_t val)
		{
			if (val > BYTES<<3)
			{
				return false;
			}
			_bits.reset(val);
			return true;
		}

		inline bool has(std::size_t val)
		{
			if (val > BYTES<<3)
			{
				return false;
			}			
			return _bits.test(val);
		}

	private:
		std::bitset<BYTES<<3> _bits;
	};
}
}


#endif