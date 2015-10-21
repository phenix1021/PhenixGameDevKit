#include "stdafx.h"
#include <Windows.h>
#include <ctime>
#include <Phenix/Concurrent/AtomLock.h>
#include <Phenix/Concurrent/Thread.h>

namespace Phenix
{
namespace Concurrent
{

AtomLock::AtomLock()		
{
	_lock = (long)UNLOCKED;
}

AtomLock::AtomLock( bool locked )
{
	_lock = (long)locked;
}

AtomLock::~AtomLock()
{
	
}

void AtomLock::lock()
{
	_spinStartTime = std::time(NULL);
	while (InterlockedExchange(&_lock, LOCKED) == LOCKED)
	{
		if (std::time(NULL) - _spinStartTime > SPIN_DURATION)
		{
			_spinStartTime = 0;
			_lock2.wait();
		} 
		else
		{
			Thread::sleep(0);
		}			
	}
}

void AtomLock::unlock()
{
	InterlockedExchange(&_lock, UNLOCKED);
	_lock2.set();
}

}
}