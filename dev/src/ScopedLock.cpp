#include "stdafx.h"
#include <Phenix/Thread/ScopedLock.h>

namespace Phenix
{
namespace Thread
{

	ScopedLock::ScopedLock()
	{
		m_lock.Lock();
	}

	ScopedLock::~ScopedLock()
	{
		m_lock.Unlock();
	}

}
}