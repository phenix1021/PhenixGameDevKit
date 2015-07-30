#include "stdafx.h"
#include <Phenix/Concurrent/ThreadPool.h>
#include <Phenix/Concurrent/ScopedLock.h>
#include <Phenix/Base/SharedPtr.h>
#include <ctime>

namespace Phenix
{
namespace Concurrent
{	

	//using ThreadPool::PooledThread;

	ThreadPool::PooledThread::PooledThread()
		:_funcLock(),_idleTime(0)
	{

	}

	ThreadPool::PooledThread::~PooledThread()
	{

	}

	void ThreadPool::PooledThread::run()
	{ //InterlockedExchange(shijian)
		for (;;)
		{//is_exit
		
			//ScopedLock<FastMutex> lock(_funcLock);
			_idleTime = 0;
			_func();
			_idleTime = std::time(NULL);
		}
	}

	void ThreadPool::PooledThread::setFunc( ThreadFunc& func )
	{		
		//ScopedLock<FastMutex> lock(_funcLock);
		_func = func;
	}

	void ThreadPool::PooledThread::release()
	{

	}


	ThreadPool::ThreadPool( Phenix::UInt8 initCapacity /*= 4*/, Phenix::UInt8 maxCapacity /*= 8*/ )
		:_maxCapacity(maxCapacity)
	{
		if (initCapacity > maxCapacity)
		{
			throw;
		}
		for (Phenix::UInt8 i=0; i<initCapacity; ++i)
		{
			_pooledThreads.push_back(new PooledThread);
		}
	}

	ThreadPool::~ThreadPool()
	{

	}

	bool ThreadPool::startThread( ThreadFunc& func )
	{
		PooledThread* pt = getPooledThread();
		if (!pt)
		{
			return false;
		}
		//Phenix::SharedPtr<PooledThread> pt = Phenix::SharedPtr<PooledThread>(new PooledThread);
		pt->setFunc(func);		
		pt->getThread().start(Phenix::Bind(&PooledThread::run, pt));
		return true;
	}

	bool ThreadPool::addCapacity(Phenix::Int8 num)
	{
		return false;
	}

	ThreadPool::PooledThread* ThreadPool::getPooledThread()
	{
		for (Phenix::UInt8 i=0; i<_pooledThreads.size(); ++i)
		{
			if (!_pooledThreads[i]->getIdleTime())
			{
				return _pooledThreads[i];
			}
		}
		
		return NULL;
	}

	void ThreadPool::release()
	{

	}

} // end namespace Concurrent
} // end namespace Phenix