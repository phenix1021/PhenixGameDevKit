#include "stdafx.h"
#include <Phenix/Concurrent/ThreadPool.h>
#include <Phenix/Concurrent/ScopedLock.h>
#include <Phenix/Base/SharedPtr.h>
#include <ctime>

namespace Phenix
{
namespace Concurrent
{	
	ThreadPool::PooledThread::PooledThread()
		:_completedCLock(true),_idleTime(0),_isFinished(false)
	{

	}

	ThreadPool::PooledThread::~PooledThread()
	{
		// nothing
	}

	void ThreadPool::PooledThread::setIdleTime(time_t val)
	{
		ScopedLock<FastMutex> lock(_idleTimeLock);
		_idleTime = val;
	}

	time_t ThreadPool::PooledThread::getIdleTime()
	{
		ScopedLock<FastMutex> lock(_idleTimeLock);
		return _idleTime;
	}

	void ThreadPool::PooledThread::run()
	{ 
		for (;;)
		{
			if (_isFinished)
			{
				break;	// 结束线程执行体
			}
			
			setIdleTime(0);
			_func();	// 调用传入的实际执行函数
			setIdleTime(std::time(NULL));
			_func.reset();
			_completedCLock.lock();
		}
	}

	void ThreadPool::PooledThread::setFunc( ThreadFunc& func )
	{		
		_func = func;
	}

	void ThreadPool::PooledThread::release()
	{		
		_isFinished = true;
		activate();
		if (Thread::join(_thread, JOIN_TIMEOUT))
		{
			delete this;	// 由于可能在JOIN_TIMEOUT时间内对应线程还没有执行完毕，从而跳过了改行，会损失PooledThread内存泄露
		}	
	}

	void ThreadPool::PooledThread::activate()
	{
		_completedCLock.unlock();
	}


	ThreadPool::ThreadPool( Phenix::UInt8 initCapacity /*= 4*/, Phenix::UInt8 maxCapacity /*= 8*/,
		Phenix::Int32 expire_duration/* = 30*/ )
		:_initCapacity(initCapacity), _maxCapacity(maxCapacity), _expireDuration(expire_duration)
	{
		if (_initCapacity > _maxCapacity)
		{
			throw;
		}
		for (Phenix::UInt8 i=0; i<_initCapacity; ++i)
		{
			_pooledThreads.push_back(new PooledThread);
		}
	}

	ThreadPool::~ThreadPool()
	{
		release();
	}

	bool ThreadPool::startThread( ThreadFunc& func )
	{
		PooledThread* pt = getThread();
		if (!pt)
		{
			if (_pooledThreads.size() == _maxCapacity)
			{
				collect();
				pt = getThread();
				if (!pt)
				{
					return false;
				}
			}

			pt = new PooledThread;
			_pooledThreads.push_back(pt);
			pt->setFunc(func);		
			pt->getThread().start(Phenix::Bind(&PooledThread::run, pt));
			
			return true;
		}		
		pt->activate();
		return true;
	}

	bool ThreadPool::addCapacity(Phenix::Int8 num)
	{
		if (_maxCapacity+num < _initCapacity) // 允许减小_maxCapacity，即允许暂时存在线程池线程数大于_maxCapacity
		{
			return false;
		}
		_maxCapacity += num;
		collect();
		return true;
	}

	ThreadPool::PooledThread* ThreadPool::getThread()
	{
		for (Phenix::UInt8 i=0; i<_pooledThreads.size(); ++i)
		{
			if (_pooledThreads[i]->getIdleTime())
			{
				return _pooledThreads[i];
			}
		}
				
		return NULL;
	}

	void ThreadPool::release()
	{
		for (Phenix::UInt32 i=0; i<_pooledThreads.size(); ++i)
		{
			_pooledThreads[i]->release();
		}
		_pooledThreads.clear();
	}

	void ThreadPool::collect()
	{
		std::vector<PooledThread*> working_threads;
		std::vector<PooledThread*> idle_threads;
		std::vector<PooledThread*> expire_threads;

		for (Phenix::UInt32 i=0; i<_pooledThreads.size(); ++i)
		{
			if (!_pooledThreads[i]->isIdle())
			{
				working_threads.push_back(_pooledThreads[i]);
				continue;
			}			
			else
			{
				if (_pooledThreads[i]->getIdleTime() >= _expireDuration)
				{
					expire_threads.push_back(_pooledThreads[i]);
					continue;
				}
				idle_threads.push_back(_pooledThreads[i]);
			}
		}

		_pooledThreads.clear();
		_pooledThreads.insert(_pooledThreads.end(), working_threads.begin(), working_threads.end());
		_pooledThreads.insert(_pooledThreads.end(), idle_threads.begin(), idle_threads.end());
		Phenix::Int32 n = working_threads.size()+idle_threads.size();
		n = (n < _initCapacity ? _initCapacity - n : 0);
		for (Phenix::Int32 i=0; i<n; ++i)
		{
			_pooledThreads.push_back(expire_threads[i]);
		}
	}

} // end namespace Concurrent
} // end namespace Phenix