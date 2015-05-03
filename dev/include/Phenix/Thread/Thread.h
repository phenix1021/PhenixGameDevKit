/*******************************************************************************
 * @brief	�߳̿�
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
		static void SleepCurThread(long milliseconds);
		static void YieldCurThread();
		static DWORD GetCurThreadID();

	public:	
		Thread();
		Thread(const Phenix::String& thread_name);

		virtual ~Thread();
		
		bool	IsRunning();
		bool	IsCurrentThread();
		//void	Start(ThreadFunc& func);
		void	CleanUp();
		
		HANDLE	GetHandle() const { return m_hnd; }
		DWORD	GetID() const { return m_id; }
		const Phenix::String& GetName() const {return m_name;}
		
	private:
		//ThreadFunc		m_func;
		Phenix::String	m_name;
		HANDLE			m_hnd;
		DWORD			m_id;
	};	


} // end namespace Thread
} // end namespace Phenix


#endif