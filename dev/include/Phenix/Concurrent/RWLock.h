/*******************************************************************************
 * @brief	读写锁            
 * @date	2015-9-7
 * @author	phenix
 * @mark
 1. 单写多读。即写时独占，读时共享
 2. 支持限制最大读数量（正在读+等待读）
 3. 写优先，即有写等待时不允许再增加在读的数量
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_RWLOCK_H
#define PHENIX_CONCURRENT_RWLOCK_H

#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/AtomCounter.h>
#include <Phenix/Concurrent/FastMutex.h>

namespace Phenix
{
namespace Concurrent
{	

	class RWLock
		:private Phenix::Noncopyable
	{
	public:	
		RWLock(Phenix::Int32 readCntMax = 0); // 0表示不限制
		virtual ~RWLock();		

		void readLock();	
		void readUnlock();
		void writeLock();
		void writeUnlock();

	private:

	private:
		FastMutex	_readLock;			// 读写竞争
		FastMutex	_readMaxLock;		// 读读竞争
		FastMutex	_writeLock;			// 写写竞争，读写竞争
		AtomCounter	_readCnt;			// 会和“写锁”竞争_readLock的读的当前数量（不包括写时被拦截的试图读）
		Phenix::Int32 _readCntMax;		// 会和“写锁”竞争_readLock的读的最大数量（不包括写时被拦截的试图读）
	};	


} // end namespace Concurrent
} // end namespace Phenix


#endif