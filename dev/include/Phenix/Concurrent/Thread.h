/*******************************************************************************
 * @brief	线程库
 * @date	2015-4-14
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_THREAD_H
#define PHENIX_CONCURRENT_THREAD_H

#include <windows.h>
#include <functional>
#include <Phenix/Base/Noncopyable.h>

using std::function;

typedef std::function<void()> ThreadFunc;

namespace Phenix
{
namespace Concurrent
{	

class Thread
	:private Phenix::Noncopyable
{
	enum Priority
	{
		LOWEST  = THREAD_PRIORITY_LOWEST,
		LOW		= THREAD_PRIORITY_BELOW_NORMAL,
		NORMAL	= THREAD_PRIORITY_NORMAL,
		HIGH    = THREAD_PRIORITY_ABOVE_NORMAL,
		HIGHEST = THREAD_PRIORITY_HIGHEST
	};

public:
	static DWORD WINAPI entry(void* thread);

	static void	sleep(long milliseconds);
	static void	yield();	
	static DWORD	getCurThreadID();
	static HANDLE	getCurThread(); // 获得当前线程伪句柄
	static void	join(Thread& thread);
	static void	join(std::vector<Thread*>& thread_list);
	static bool	join(Thread& thread, long milliseconds);
	static bool	join(std::vector<Thread*>& thread_list, long milliseconds);
	static Phenix::Int32 getThreadPriority(){return GetThreadPriority(GetCurrentThread());}
	static void	cleanUp(std::vector<Thread*>& threads);

public:	
	Thread();
	Thread(const Phenix::String& name);
	virtual ~Thread();	
	
	void	start(ThreadFunc& func);	
	
	inline HANDLE	getHandle() const { return _hnd; }
	inline DWORD	getID()		const { return _id; }
	inline const Phenix::String&	getName() const { return _name; }

	inline Phenix::Int32	getPriority()	const { return _priority; }
	inline bool	setPriority(Phenix::Int32 priority);		
	
private:
	bool	isNull() const;
	bool	isRunning() const;

	void	cleanUp();

private:
	ThreadFunc		_func;
	Phenix::String	_name;
	HANDLE			_hnd;	// 一个线程内核对象在不同进程里可以同时有多个各自的线程句柄（如CreateThread、OpenThread获得的句柄都不同），但指向的地址都一样
	DWORD			_id;	// 一个线程内核对象只有一个线程ID（全局）
	Phenix::Int32	_priority;
};	


} // end namespace Concurrent
} // end namespace Phenix


#endif