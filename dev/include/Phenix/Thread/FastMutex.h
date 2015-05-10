/*******************************************************************************
 * @brief	快速互斥锁（基于带自旋的临界区）
 * @date	2015-5-10
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_THREAD_FAST_MUTEX_H
#define PHENIX_THREAD_FAST_MUTEX_H

#include <Windows.h>
#include <Phenix/Base/Noncopyable.h>

namespace Phenix
{
namespace Thread
{

using Phenix::Base::Noncopyable;

class FastMutex
	:private Noncopyable
{
public:
	FastMutex();
	virtual ~FastMutex();

	void Lock();
	bool TryLock();
	void Unlock();

	const CRITICAL_SECTION& GetLockerInfo() const { return m_cs; }

private:
	CRITICAL_SECTION	m_cs;	
};

}
}


#endif