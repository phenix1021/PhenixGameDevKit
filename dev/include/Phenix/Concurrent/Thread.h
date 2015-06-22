/*******************************************************************************
 * @brief	Ïß³Ì¿â
 * @date	2015-4-14
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_THREAD_H
#define PHENIX_CONCURRENT_THREAD_H

#include <vector>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Base/Noncopyable.h>

using Phenix::Function;

namespace Phenix
{
namespace Concurrent
{	

	typedef Function<void()> ThreadFunc;

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
		static void			sleep(long milliseconds);
		static void			yield();
		static HANDLE		getThreadHandle();
		static DWORD		getThreadID();
		static void			join(Thread& thread);
		static void			join(std::vector<Thread*>& thread_list);
		static bool			join(Thread& thread, long milliseconds);
		static bool			join(std::vector<Thread*>& thread_list, long milliseconds);
		static Phenix::Int32 getThreadPriority(){return GetThreadPriority(GetCurrentThread());}
		static void			cleanUp(std::vector<Thread*>& threads);

	public:	
		Thread();
		Thread(const Phenix::String& thread_name);

		virtual ~Thread();
		
		bool	isNull() const;
		bool	isRunning() const;
		bool	isCurThread() const;
		
		void	start(ThreadFunc& func);
		void	cleanUp();
		
		HANDLE	getHandle() const { return _hnd; }
		DWORD	getID()		const { return _id; }
		Phenix::Int32	getPriority()	const { return _priority; }
		bool			setPriority(Phenix::Int32 priority);
		const Phenix::String& getName() const {return _name;}
		
	private:
		ThreadFunc		_func;
		Phenix::String	_name;
		HANDLE			_hnd;
		DWORD			_id;
		Phenix::Int32	_priority;
	};	


} // end namespace Concurrent
} // end namespace Phenix


#endif