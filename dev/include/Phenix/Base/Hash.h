/*******************************************************************************
* @brief	¹þÏ£º¯Êý
 * @date	2015-9-15
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_HASH_H
#define PHENIX_BASE_HASH_H

namespace Phenix
{

class HashCode
{
public:
	template<typename T>
	std::size_t operator () (T key, std::size_t maxValue = 0)
	{
		if (maxValue > 0)
		{
			return static_cast<std::size_t>(hashCode(key)) % maxValue;
		}
		else
		{
			return static_cast<std::size_t>(hashCode(key));
		}
	}

private:
	inline std::size_t hashCode(const Phenix::Int8 n);
	inline std::size_t hashCode(const Phenix::UInt8 n);
	inline std::size_t hashCode(const Phenix::Int16 n);
	inline std::size_t hashCode(const Phenix::UInt16 n);
	inline std::size_t hashCode(const Phenix::Int32 n);
	inline std::size_t hashCode(const Phenix::UInt32 n);
	inline std::size_t hashCode(const Phenix::Int64 n);
	inline std::size_t hashCode(const Phenix::UInt64 n);
	inline std::size_t hashCode(const Phenix::String& str);
};

} // end namespace Phenix

#endif