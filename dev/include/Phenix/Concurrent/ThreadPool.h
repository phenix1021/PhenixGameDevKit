/*******************************************************************************
 * @brief	�߳�pool
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
	// �����߳���
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
		ThreadFunc		_func;				// �߳�ִ������ִ�еĴ��뺯����
		
		volatile time_t	_idleTime;			// ���п�ʼʱ��
		bool			_isFinished;		// �߳�ִ�����������

		AtomLock		_completedCLock;	// ���뺯����ִ�������
		FastMutex		_idleTimeLock;			
	};

public:	
	ThreadPool(Phenix::UInt8 init_capacity = 4, Phenix::UInt8 max_capacity = 8, 
		Phenix::Int32 expire_duration = 30);
	virtual ~ThreadPool();		

	bool startThread(ThreadFunc& func);
	bool addCapacity(Phenix::Int8 num);
	void collect();	// ���չ����߳�

private:
	PooledThread* getThread();		
	void release();

private:
	std::vector<PooledThread*> _pooledThreads;
	Phenix::UInt8 _initCapacity;
	Phenix::UInt8 _maxCapacity;
	Phenix::Int32 _expireDuration;	// �����̹߳���ʱ�䣨�ӿ�ʼidle���𣬵�λ�룩
};	


} // end namespace Concurrent
} // end namespace Phenix


#endif