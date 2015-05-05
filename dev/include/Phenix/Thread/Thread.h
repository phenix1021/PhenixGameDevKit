/*******************************************************************************
 * @brief	Ïß³Ì¿â
 * @date	2015-4-14
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_THREAD_H
#define PHENIX_THREAD_H

#include <Phenix/Base/Delegate.h>

using Phenix::Base::Function;

namespace Phenix
{
namespace Thread
{	

	class Thread
	{
		typedef Function<void()> ThreadFunc;

		enum Priority
		{
			LOWEST  = THREAD_PRIORITY_LOWEST,
			LOW		= THREAD_PRIORITY_BELOW_NORMAL,
			NORMAL	= THREAD_PRIORITY_NORMAL,
			HIGH    = THREAD_PRIORITY_ABOVE_NORMAL,
			HIGHEST = THREAD_PRIORITY_HIGHEST
		};

	public:
		static DWORD WINAPI Entry(void* thread);
		static void			SleepCurThread(long milliseconds);
		static void			YieldCurThread();
		static DWORD		GetCurThreadID();
		static void			WaitCurThreadCompleted(Thread& thread);
		static bool			WaitCurThreadCompleted(Thread& thread, long milliseconds);
		static Phenix::Int32 GetCurThreadPriority(){return GetThreadPriority(GetCurrentThread());}

	public:	
		Thread();
		Thread(const Phenix::String& thread_name);

		virtual ~Thread();
		
		bool	IsNull() const;
		bool	IsRunning() const;
		bool	IsCurThread() const;
		void	Start(ThreadFunc& func);
		void	CleanUp();
		
		HANDLE	GetHandle() const { return m_hnd; }
		DWORD	GetID()		const { return m_id; }
		Phenix::Int32	GetPriority()	const { return m_priority; }
		bool			SetPriority(Phenix::Int32 priority);
		const Phenix::String& GetName() const {return m_name;}
		
	private:
		ThreadFunc		m_func;
		Phenix::String	m_name;
		HANDLE			m_hnd;
		DWORD			m_id;
		Phenix::Int32	m_priority;
	};	


} // end namespace Thread
} // end namespace Phenix


#endif