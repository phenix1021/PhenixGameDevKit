#include "stdafx.h"
#include <Phenix/Concurrent/Event.h>

namespace Phenix
{
namespace Concurrent
{

Event::Event(bool auto_reset/* = true*/)
{
	_hnd = CreateEventW(NULL, !auto_reset, false, NULL);
	if (!_hnd)
	{
		throw;
	}
}

Event::~Event()
{
	CloseHandle(_hnd);
}

void Event::set()
{
	if (!SetEvent(_hnd))
	{
		throw;
	}		
}

void Event::reset()
{
	if (!ResetEvent(_hnd))
	{
		throw;
	}		
}

void Event::wait()
{
	if (WAIT_OBJECT_0 != WaitForSingleObject(_hnd, INFINITE))
	{
		throw;
	}
		
}

bool Event::wait( long milliseconds )
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