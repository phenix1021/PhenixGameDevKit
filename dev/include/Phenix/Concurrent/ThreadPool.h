/*******************************************************************************
 * @brief	Ïß³Ìpool
 * @date	2015-6-22
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_THREAD_POOL_H
#define PHENIX_CONCURRENT_THREAD_POOL_H

#include <vector>
#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Concurrent/FastMutex.h>


namespace Phenix
{
namespace Concurrent
{	

	class ThreadPool
		:private Phenix::Noncopyable
	{
		class PooledThread
			:private Phenix::Noncopyable
		{
		public:
			PooledThread();
			virtual ~PooledThread();

			void run();		
			void release();
			inline void setFunc(ThreadFunc& func);

			inline Thread& getThread(){ return _thread; }
			inline time_t getIdleTime(){ return _idleTime; }

		private:
			Thread			_thread;
			ThreadFunc		_func;
			FastMutex		_funcLock;	// the lock for _func set
			time_t			_idleTime;
		};

	public:	
		ThreadPool(Phenix::UInt8 init_capacity = 4, Phenix::UInt8 max_capacity = 8);
		virtual ~ThreadPool();		

		bool startThread(ThreadFunc& func);
		bool addCapacity(Phenix::Int8 num);

	private:
		PooledThread* getPooledThread();		
		void release();

	private:
		std::vector<PooledThread*> _pooledThreads;
		Phenix::UInt8 _maxCapacity;
	};	


} // end namespace Concurrent
} // end namespace Phenix


#endif