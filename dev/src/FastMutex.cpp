#include "stdafx.h"
#include <Phenix/Thread/FastMutex.h>

namespace Phenix
{
namespace Thread
{

	FastMutex::FastMutex()
	{
		if (!InitializeCriticalSectionAndSpinCount(&m_cs, 4000))
		{
			throw;
		}		
	}

	FastMutex::~FastMutex()
	{
		LeaveCriticalSection(&m_cs);
	}

	void FastMutex::Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	bool FastMutex::TryLock()
	{
		return TryEnterCriticalSection(&m_cs);
	}

	void FastMutex::Unlock()
	{
		DeleteCriticalSection(&m_cs);
	}

}
}