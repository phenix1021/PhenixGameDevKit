#include "stdafx.h"
#include <Phenix/Concurrent/AtomLock.h>
#include <Windows.h>
#include <Phenix/Concurrent/Thread.h>

namespace Phenix
{
namespace Concurrent
{

	AtomLock::AtomLock()
	{
		_lock = (long)UNLOCKED;
	}

	AtomLock::~AtomLock()
	{
		
	}

	void AtomLock::lock()
	{
		while (InterlockedExchange(&_lock, LOCKED) == LOCKED)
		{
			Thread::sleep(0);
		}
	}

	void AtomLock::unlock()
	{
		InterlockedExchange(&_lock, UNLOCKED);
	}

}
}