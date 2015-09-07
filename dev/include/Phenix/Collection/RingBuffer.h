/*******************************************************************************
 * @brief	环形缓冲区
 * @date	2015-9-8
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_ATOM_LOCK_H
#define PHENIX_CONCURRENT_ATOM_LOCK_H

#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/Event.h>

namespace Phenix
{
namespace Concurrent
{

	using Phenix::Noncopyable;

	class AtomLock
	{
		enum
		{
			UNLOCKED	= 0,
			LOCKED		= 1,
			SPIN_DURATION	= 5	/*单位秒*/
		};

	public:
		AtomLock();
		AtomLock(bool locked);
		virtual ~AtomLock();

		void lock();	
		void unlock();

	private:
		volatile long	_lock;
		time_t			_spinStartTime;
		Event			_lock2;
	};

}
}


#endif