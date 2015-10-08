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

	protected:
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

	template<typename T>
	class HashCodeTraits
	{
	public:
		HashCodeTraits(T& obj):_obj(obj){}		
		virtual ~HashCodeTraits(){}

		inline std::size_t hash(){return _obj.hashCode();}
		inline T& obj(){return _obj;}
	private:
		T _obj;
	};	

#define HASH_CODE_TRAITS_SPEC(X)								\
	template<>													\
	class HashCodeTraits<X>										\
		:private HashCode										\
	{															\
	public:														\
		inline std::size_t hash(){return hashCode(_obj);}		\
	private:													\
		X _obj;													\
	};

	HASH_CODE_TRAITS_SPEC(Phenix::Int8)
	HASH_CODE_TRAITS_SPEC(Phenix::UInt8)
	HASH_CODE_TRAITS_SPEC(Phenix::Int16)
	HASH_CODE_TRAITS_SPEC(Phenix::UInt16)
	HASH_CODE_TRAITS_SPEC(Phenix::Int32)
	HASH_CODE_TRAITS_SPEC(Phenix::UInt32)
	HASH_CODE_TRAITS_SPEC(Phenix::Int64)
	HASH_CODE_TRAITS_SPEC(Phenix::UInt64)
	HASH_CODE_TRAITS_SPEC(Phenix::String)

} // end namespace Phenix

#endif