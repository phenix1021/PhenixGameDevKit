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
		//reinterpret_cast<Thread*>(thread)->m_func();
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

// 	void Thread::Start( ThreadFunc& func )
// 	{
// 		if (!IsRunning())
// 		{
// 			return;
// 		}
// 		m_func = func;
// 		m_hnd = CreateThread(0, 0, &Thread::Entry, this, 0, &m_id);
// 	}

	bool Thread::IsRunning()
	{
		if (m_hnd)
		{
			DWORD ec = 0;
			return GetExitCodeThread(m_hnd, &ec) && ec == STILL_ACTIVE;
		}
		return false;
	}

	Thread::Thread()
		:m_hnd(0), m_id(0)
	{
		static Phenix::UInt32 uid = 0;
		m_name.append("thread#").append(Phenix::Base::Parser::ToString(++uid));
	}

	Thread::Thread( const Phenix::String& thread_name )
		:m_hnd(0), m_id(0), m_name(thread_name)
	{

	}

	Thread::~Thread()
	{
		if (m_hnd)
		{
			CloseHandle(m_hnd);
		}
	}

	void Thread::CleanUp()
	{
		if (m_hnd)
		{
			CloseHandle(m_hnd);
			m_hnd = 0;
		}
	}

	bool Thread::IsCurrentThread()
	{
		return GetCurrentThread() == m_hnd;
	}

	DWORD Thread::GetCurThreadID()
	{
		return GetCurrentThreadId();
	}


} // end namespace Thread
} // end namespace Phenix