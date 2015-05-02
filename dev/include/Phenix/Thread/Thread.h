/*******************************************************************************
 * @brief	Ïß³Ì¿â
 * @date	2015-4-14
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_SIMPLEDELEGATE_H
#define PHENIX_BASE_SIMPLEDELEGATE_H

#include <windows.h>
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
		static void Sleep(long milliseconds);
		static void Yield();

	public:	
		Thread();
		virtual ~Thread();
		
		void	Start(ThreadFunc func);
		void	CleanUp();
		HANDLE	GetHandle() const { return m_hnd; }
		DWORD	GetID() const { return m_id; }
		bool	IsRunning();
	private:
		

		ThreadFunc		m_func;
		Phenix::String	m_name;
		HANDLE			m_hnd;
		DWORD			m_id;
	};	


} // end namespace Thread
} // end namespace Phenix


#endif