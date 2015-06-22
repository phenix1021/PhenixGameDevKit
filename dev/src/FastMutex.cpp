#include "stdafx.h"
#include <Phenix/Concurrent/FastMutex.h>

namespace Phenix
{
namespace Concurrent
{

	FastMutex::FastMutex()
	{
		if (!InitializeCriticalSectionAndSpinCount(&_cs, 4000))		
		{
			throw;
		}
	}

	FastMutex::~FastMutex()
	{
		DeleteCriticalSection(&_cs);
	}

	void FastMutex::lock()
	{
		EnterCriticalSection(&_cs);
	}

	bool FastMutex::tryLock()
	{
		return TryEnterCriticalSection(&_cs);
	}

	void FastMutex::unlock()
	{
		LeaveCriticalSection(&_cs);
	}

}
}