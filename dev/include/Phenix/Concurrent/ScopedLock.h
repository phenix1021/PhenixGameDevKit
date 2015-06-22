/*******************************************************************************
 * @brief	æ÷”ÚÀ¯
 * @date	2015-5-10
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_SCOPED_LOCK_H
#define PHENIX_CONCURRENT_SCOPED_LOCK_H

#include <Phenix/Concurrent/FastMutex.h>

namespace Phenix
{
namespace Concurrent
{

template<typename M>
class ScopedLock
	:private Noncopyable
{
public:
	ScopedLock(M& lock):_lock(lock){_lock.lock();}
	virtual ~ScopedLock(){_lock.unlock();}

private:
	M& _lock;
};


}
}


#endif