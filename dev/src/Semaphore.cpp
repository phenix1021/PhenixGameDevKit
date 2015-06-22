#include "stdafx.h"
#include <Phenix/Concurrent/Semaphore.h>

namespace Phenix
{
namespace Concurrent
{

	Semaphore::Semaphore(Phenix::Int32 init, Phenix::Int32 max)
	{
		if (init < 0 || max < 0 || init > max)
			throw;
		_hnd = CreateSemaphoreW(NULL, init, max, NULL);
		if (!_hnd)
		{
			throw;
		}
	}

	Semaphore::~Semaphore()
	{
		CloseHandle(_hnd);
	}

	void Semaphore::set()
	{
		if (!ReleaseSemaphore(_hnd, 1, NULL))
		{
			throw;
		}		
	}

	void Semaphore::wait()
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(_hnd, INFINITE))
		{
			throw;
		}
			
	}

	bool Semaphore::wait( long milliseconds )
	{
		if (!milliseconds)
		{
			wait();
			return true;
		}
		switch (WaitForSingleObject(_hnd, milliseconds))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		default:
			throw;
		}
	}

}
}