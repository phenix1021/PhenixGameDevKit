/*******************************************************************************
 * @brief	æ÷”ÚÀ¯
 * @date	2015-5-10
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_THREAD_SCOPED_LOCK_H
#define PHENIX_THREAD_SCOPED_LOCK_H

#include <Phenix/Thread/FastMutex.h>

namespace Phenix
{
namespace Thread
{

class ScopedLock
	:private Noncopyable
{
public:
	ScopedLock();
	virtual ~ScopedLock();

private:
	FastMutex m_lock;
};

}
}


#endif