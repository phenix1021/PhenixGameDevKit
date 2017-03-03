/*******************************************************************************
 * @brief	对象转数组下标
 * @date	2017-2-26
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_INDEX_H
#define PHENIX_BASE_INDEX_H

namespace Phenix
{

template<typename T>
class Index
{
public:
	Index(T& t, std::map<T, Phenix::UInt32>& objs):_t(t), _objs(objs){}	
	virtual ~Index(){}
	
	operator Phenix::UInt32()
	{
   		std::map<T, Phenix::UInt32>::iterator iter = _objs.find(_t);
		if (iter != _objs.end())
		{
			return iter->second;
		}
		throw;		
	}

private:
	T&								_t;
	std::map<T, Phenix::UInt32>&	_objs;
};



} // end namespace Phenix

#endif // PHENIX_BASE_INDEX_H