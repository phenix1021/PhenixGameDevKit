#include "stdafx.h"
#include <windows.h>
#include <Phenix/Thread/Thread.h>
#include <Phenix/Base/Parser.h>

namespace Phenix
{
namespace Thread
{	


	DWORD WINAPI Thread::entry( void* thread )
	{
		reinterpret_cast<Thread*>(thread)->_func();
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
		if (!isRunning())
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
		_name.append("thread#").append(Phenix::Base::Parser::ToString(++uid));
	}

	Thread::Thread( const Phenix::String& thread_name )
		:_hnd(0), _id(0), _priority(NORMAL), _name(thread_name)
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

	bool Thread::isCurThread() const
	{
		return GetCurrentThread() == _hnd;
	}

	DWORD Thread::getThreadID()
	{
		return GetCurrentThreadId();
	}

	void Thread::join( Thread& thread )
	{
		if (thread.IsNull())
		{
			return;
		}
		switch (WaitForSingleObject(thread.m_hnd, INFINITE))
		{		
		case WAIT_OBJECT_0:
			thread.CleanUp();
			return;
		default:
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
		if (thread.IsNull())
		{
			return false;
		}
		switch (WaitForSingleObject(thread.m_hnd, milliseconds))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			thread.CleanUp();
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


} // end namespace Thread
} // end namespace Phenix