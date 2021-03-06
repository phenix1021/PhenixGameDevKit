#include "stdafx.h"
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Base/Parser.h>
#include <Phenix/Base/BugCheck.h>

namespace Phenix
{
namespace Concurrent
{	

DWORD WINAPI Thread::entry( void* thread )
{
	BEGIN_SEH
	reinterpret_cast<Thread*>(thread)->_func();	
	END_SEH("")
	return 0;
}

void Thread::sleep( long milliseconds )
{
	Sleep((DWORD)milliseconds);
}

void Thread::yield()
{
	Sleep(0);
}

void Thread::start( ThreadFunc& func )
{
	if (isRunning())
	{
		return;
	}
	_func = func;
	_hnd = CreateThread(0, 0, &Thread::entry, this, 0, &_id);
	if (isNull())
	{
		throw;
	}		
	if (_priority != NORMAL && !SetThreadPriority(_hnd, _priority))
		throw;
}

bool Thread::isRunning() const
{
	if (!isNull())
	{
		DWORD ec = 0;
		return GetExitCodeThread(_hnd, &ec) && ec == STILL_ACTIVE;
	}
	return false;
}

Thread::Thread()
	:_hnd(0), _id(0), _priority(NORMAL)
{
	static Phenix::UInt32 uid = 0;
	_name.append("thread#").append(Phenix::Parser::ToString(++uid));
}

Thread::Thread( const Phenix::String& name )
	:_hnd(0), _id(0), _priority(NORMAL), _name(name)
{

}

Thread::~Thread()
{
	if (!isNull())
	{
		CloseHandle(_hnd);
	}
}

void Thread::cleanUp()
{
	if (!isNull())
	{
		CloseHandle(_hnd);
		_hnd = 0;
		_id = 0;
		_priority = NORMAL;
		_name.clear();
	}
}

DWORD Thread::getCurThreadID()
{
	return GetCurrentThreadId();
}

HANDLE Thread::getCurThread()
{
	return GetCurrentThread();
}

void Thread::cleanUp(std::vector<Thread*>& threads)
{
	for (Phenix::UInt32 i=0; i<threads.size(); ++i)
	{
		threads[i]->cleanUp();
	}
}

void Thread::join( Thread& thread )
{
	if (thread.isNull())
	{
		return;
	}
	switch (WaitForSingleObject(thread._hnd, INFINITE))
	{		
	case WAIT_OBJECT_0:
		thread.cleanUp();
		return;
	default:
		throw;
	}
}

void Thread::join( std::vector<Thread*>& thread_list )
{
	HANDLE handles[MAXIMUM_WAIT_OBJECTS];
	Phenix::Int32 cnt = 0;
	for (Phenix::UInt32 i=0; i<thread_list.size() && i<MAXIMUM_WAIT_OBJECTS; ++i)
	{
		if (thread_list[i]->isNull())
		{
			continue;
		}
		handles[cnt++] = thread_list[i]->getHandle();
	}

	if (!cnt)
	{
		return;
	}

	if (WAIT_OBJECT_0 == WaitForMultipleObjects(cnt, handles, true, INFINITE))
	{
		Thread::cleanUp(thread_list);
	}
	else
	{
		throw;
	}
}	

bool Thread::join( Thread& thread, long milliseconds )
{
	if (!milliseconds)
	{
		join(thread);
		return true;
	}
	if (thread.isNull())
	{
		return false;
	}
	switch (WaitForSingleObject(thread._hnd, milliseconds))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		thread.cleanUp();
		return true;
	default:
		throw;
	}
}

bool Thread::join( std::vector<Thread*>& thread_list, long milliseconds )
{
	HANDLE handles[MAXIMUM_WAIT_OBJECTS];
	Phenix::Int32 cnt = 0;
	for (Phenix::UInt32 i=0; i<thread_list.size() && i<MAXIMUM_WAIT_OBJECTS; ++i)
	{
		if (thread_list[i]->isNull())
		{
			continue;
		}
		handles[cnt++] = thread_list[i]->getHandle();
	}

	if (!cnt)
	{
		return false;
	}

	switch (WaitForMultipleObjects(cnt, handles, true, milliseconds))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		Thread::cleanUp(thread_list);
		return true;
	default:
		throw;		
	}
}

bool Thread::isNull() const
{
	return !_hnd;
}

bool Thread::setPriority( Phenix::Int32 priority )
{		
	if (_priority != priority && SetThreadPriority(_hnd, priority))
	{
		_priority = priority;
		return true;
	}		 
	return false;
}


} // end namespace Concurrent
} // end namespace Phenix