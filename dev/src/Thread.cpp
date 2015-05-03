#include "stdafx.h"
#include <windows.h>
#include <Phenix/Thread/Thread.h>
#include <Phenix/Base/Parser.h>

namespace Phenix
{
namespace Thread
{	


	DWORD WINAPI Thread::Entry( void* thread )
	{
		reinterpret_cast<Thread*>(thread)->m_func();
		return 0;
	}

	void Thread::SleepCurThread( long milliseconds )
	{
		Sleep((DWORD)milliseconds);
	}

	void Thread::YieldCurThread()
	{
		Sleep(0);
	}

	void Thread::Start( ThreadFunc& func )
	{
		if (!IsRunning())
		{
			return;
		}
		m_func = func;
		m_hnd = CreateThread(0, 0, &Thread::Entry, this, 0, &m_id);
		if (IsNull())
		{
			throw;
		}		
		if (m_priority != NORMAL && !SetThreadPriority(m_hnd, m_priority))
			throw;
	}

	bool Thread::IsRunning() const
	{
		if (!IsNull())
		{
			DWORD ec = 0;
			return GetExitCodeThread(m_hnd, &ec) && ec == STILL_ACTIVE;
		}
		return false;
	}

	Thread::Thread()
		:m_hnd(0), m_id(0), m_priority(NORMAL)
	{
		static Phenix::UInt32 uid = 0;
		m_name.append("thread#").append(Phenix::Base::Parser::ToString(++uid));
	}

	Thread::Thread( const Phenix::String& thread_name )
		:m_hnd(0), m_id(0), m_priority(NORMAL), m_name(thread_name)
	{

	}

	Thread::~Thread()
	{
		if (!IsNull())
		{
			CloseHandle(m_hnd);
		}
	}

	void Thread::CleanUp()
	{
		if (!IsNull())
		{
			CloseHandle(m_hnd);
			m_hnd = 0;
			m_id = 0;
			m_priority = NORMAL;
			m_name.clear();
		}
	}

	bool Thread::IsCurThread() const
	{
		return GetCurrentThread() == m_hnd;
	}

	DWORD Thread::GetCurThreadID()
	{
		return GetCurrentThreadId();
	}

	void Thread::WaitCurThreadCompleted( Thread& thread )
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

	bool Thread::WaitCurThreadCompleted( Thread& thread, long milliseconds )
	{
		if (!milliseconds)
		{
			WaitCurThreadCompleted(thread);
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

	bool Thread::IsNull() const
	{
		return !m_hnd;
	}

	bool Thread::SetPriority( Phenix::Int32 priority )
	{		
		if (m_priority != priority && SetThreadPriority(m_hnd, priority))
		{
			m_priority = priority;
			return true;
		}		 
		return false;
	}


} // end namespace Thread
} // end namespace Phenix