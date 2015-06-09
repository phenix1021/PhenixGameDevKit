#include "stdafx.h"
#include <Phenix/Concurrent/ScopedLock.h>

namespace Phenix
{
namespace Concurrent
{

	ScopedLock::ScopedLock(FastMutex& lock)
		:_lock(lock)
	{
		_lock.lock();
	}

	ScopedLock::~ScopedLock()
	{
		_lock.unlock();
	}

}
}