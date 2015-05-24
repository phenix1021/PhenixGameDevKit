#include "stdafx.h"
#include <Phenix/Thread/ScopedLock.h>

namespace Phenix
{
namespace Thread
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