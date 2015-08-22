/*******************************************************************************
 * @brief	原子锁
 * @date	2015-6-8
 * @author	phenix
 * @mark    注意原子锁不可重入，即非递归锁，否认会引起死锁
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_ATOM_LOCK_H
#define PHENIX_CONCURRENT_ATOM_LOCK_H

#include <Windows.h>
#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/Event.h>

namespace Phenix
{
namespace Concurrent
{

using Phenix::Noncopyable;

class AtomLock
	:private Noncopyable
{
	enum
	{
		UNLOCKED	= 0,
		LOCKED		= 1,
		SPIN_DURATION	= 5	/*单位秒*/
	};

public:
	AtomLock();
	AtomLock(bool locked);
	virtual ~AtomLock();

	void lock();	
	void unlock();

private:
	volatile long	_lock;
	time_t			_spinStartTime;
	Event			_lock2;
};

}
}


#endif