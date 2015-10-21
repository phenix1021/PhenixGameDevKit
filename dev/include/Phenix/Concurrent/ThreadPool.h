/*******************************************************************************
 * @brief	线程pool
 * @date	2015-6-22
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_THREAD_POOL_H
#define PHENIX_CONCURRENT_THREAD_POOL_H

#include <vector>
#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Concurrent/AtomLock.h>
#include <Phenix/Concurrent/FastMutex.h>

namespace Phenix
{
namespace Concurrent
{	

class ThreadPool
	:private Phenix::Noncopyable
{
	// 池内线程体
	class PooledThread
		:private Phenix::Noncopyable
	{
		enum{
			JOIN_TIMEOUT = 10000
		};
	public:
		PooledThread();
		virtual ~PooledThread();

		void run();
		void activate();
		void release();
		inline void setFunc(ThreadFunc& func);

		inline Thread&	getThread(){ return _thread; }
		inline void		setIdleTime(time_t val);
		inline time_t	getIdleTime();
		inline bool		isIdle(){ return getIdleTime() > 0; }

	private:
		Thread			_thread;
		ThreadFunc		_func;				// 线程执行体中执行的传入函数体
		
		volatile time_t	_idleTime;			// 空闲开始时间
		bool			_isFinished;		// 线程执行体结束开关

		AtomLock		_completedCLock;	// 传入函数体执行完成锁
		FastMutex		_idleTimeLock;			
	};

public:	
	ThreadPool(Phenix::UInt8 init_capacity = 4, Phenix::UInt8 max_capacity = 8, 
		Phenix::Int32 expire_duration = 30);
	virtual ~ThreadPool();		

	bool startThread(ThreadFunc& func);
	bool addCapacity(Phenix::Int8 num);
	void collect();	// 回收过期线程

private:
	PooledThread* getThread();		
	void release();

private:
	std::vector<PooledThread*> _pooledThreads;
	Phenix::UInt8 _initCapacity;
	Phenix::UInt8 _maxCapacity;
	Phenix::Int32 _expireDuration;	// 空闲线程过期时间（从开始idle算起，单位秒）
};	


} // end namespace Concurrent
} // end namespace Phenix


#endif