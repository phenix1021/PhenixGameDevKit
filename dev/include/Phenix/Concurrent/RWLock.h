/*******************************************************************************
 * @brief	��д��            
 * @date	2015-9-7
 * @author	phenix
 * @mark
 1. ��д�������дʱ��ռ����ʱ����
 2. ֧�������������������ڶ�+�ȴ�����
 3. д���ȣ�����д�ȴ�ʱ�������������ڶ�������
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_RWLOCK_H
#define PHENIX_CONCURRENT_RWLOCK_H

#include <Phenix/Base/Noncopyable.h>
#include <Phenix/Concurrent/AtomCounter.h>
#include <Phenix/Concurrent/FastMutex.h>

namespace Phenix
{
namespace Concurrent
{	

	class RWLock
		:private Phenix::Noncopyable
	{
	public:	
		RWLock(Phenix::Int32 readCntMax = 0); // 0��ʾ������
		virtual ~RWLock();		

		void readLock();	
		void readUnlock();
		void writeLock();
		void writeUnlock();

	private:

	private:
		FastMutex	_readLock;			// ��д����
		FastMutex	_readMaxLock;		// ��������
		FastMutex	_writeLock;			// дд��������д����
		AtomCounter	_readCnt;			// ��͡�д��������_readLock�Ķ��ĵ�ǰ������������дʱ�����ص���ͼ����
		Phenix::Int32 _readCntMax;		// ��͡�д��������_readLock�Ķ������������������дʱ�����ص���ͼ����
	};	


} // end namespace Concurrent
} // end namespace Phenix


#endif