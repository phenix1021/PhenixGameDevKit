#include "stdafx.h"
#include <Phenix/Thread/Thread.h>

namespace Phenix
{
namespace Thread
{	


	DWORD WINAPI Thread::Entry( void* thread )
	{
		reinterpret_cast<Thread*>(thread)->m_func();
		return 0;
	}

	void Thread::Sleep( long milliseconds )
	{
		Sleep(milliseconds);
	}

	void Thread::Yield()
	{
		Sleep(0);
	}

	void Thread::Start( ThreadFunc func )
	{
		if (!IsRunning())
		{
			return;
		}
		m_func = func;
		m_hnd = CreateThread(0, 0, Thread::Entry(), this, 0, &m_id);
	}

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
		m_name.clear();
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


} // end namespace Thread
} // end namespace Phenix