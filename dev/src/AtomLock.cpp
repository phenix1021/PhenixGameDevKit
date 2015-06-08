#include "stdafx.h"
#include <Phenix/Thread/AtomLock.h>
#include <Windows.h>
#include <Phenix/Thread/Thread.h>

namespace Phenix
{
namespace Thread
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