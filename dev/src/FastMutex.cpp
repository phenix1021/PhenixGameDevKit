#include "stdafx.h"
#include <Phenix/Thread/FastMutex.h>

namespace Phenix
{
namespace Thread
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
		LeaveCriticalSection(&_cs);
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
		DeleteCriticalSection(&_cs);
	}

}
}