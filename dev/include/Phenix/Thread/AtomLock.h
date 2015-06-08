/*******************************************************************************
 * @brief	ԭ����
 * @date	2015-6-8
 * @author	phenix
 * @mark    ע��ԭ�����������룬���ǵݹ��������ϻ���������
 ******************************************************************************/
#ifndef PHENIX_THREAD_ATOM_LOCK_H
#define PHENIX_THREAD_ATOM_LOCK_H

#include <Windows.h>
#include <Phenix/Base/Noncopyable.h>

namespace Phenix
{
namespace Thread
{

using Phenix::Base::Noncopyable;

class AtomLock
	:private Noncopyable
{
	enum
	{
		UNLOCKED = 0,
		LOCKED
	};
public:
	AtomLock();
	virtual ~AtomLock();

	void lock();	
	void unlock();

private:
	volatile long	_lock;	
};

}
}


#endif