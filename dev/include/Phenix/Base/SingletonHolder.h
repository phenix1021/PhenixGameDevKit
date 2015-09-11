/*******************************************************************************
* @brief	单件模式
 * @date	2015-9-10
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_SINGLETON_HOLDER_H
#define PHENIX_BASE_SINGLETON_HOLDER_H

#include <Phenix/Concurrent/ScopeLock.h>

using namespace Phenix::Concurrent;

namespace Phenix
{

template<typename T>
class SingletonHolder	
{
public:
	SingletonHolder(){}
	virtual ~SingletonHolder(){delete _obj;}

	T* get()
	{
		if (!_obj)
		{
			ScopeLock(_lock);
			try
			{
				_obj = new T;
			}
			catch (...)
			{
				throw;
			}
		}
		return _obj;
	}

private:
	T* _obj;
	FastMutex	_lock;
};

#define SUPPORT_SINGLETON(class_name)							/
		friend class Phenix::SingletonHolder<class_name>;		/
	public:														/
		static class_name& GetInstance()						/
		{														/
			static Phenix::SingletonHolder<class_name> inst;	/
			return *inst.get();									/	
		}

} // end namespace Phenix

#endif