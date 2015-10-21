#include "stdafx.h"
#include <Phenix/Concurrent/RWLock.h>

namespace Phenix
{
namespace Concurrent
{	

void RWLock::readLock()
{
	// ����������Ϊ��ʵ������дʱ��������µĻ�"д��"����_readLock�Ķ�
	_writeLock.lock();
	_writeLock.unlock();

	if (_readCnt.inc() == 1)
	{
		_readLock.lock();	// ��֤���ֻ��һ�����߳̾�����������ʾ�ڶ�			
	}

	if (_readCntMax > 0 && _readCnt.value() >= _readCntMax)
	{
		_readMaxLock.lock();
	}		
}

void RWLock::readUnlock()
{		
	if (_readCntMax > 0 && _readCnt.value() >= _readCntMax)
	{
		_readMaxLock.unlock();
	}

	if (_readCnt.dec() == 0)
	{
		_readLock.unlock();	// �ͷŶ�������ʾĿǰû���ڶ�		
	}		
}

void RWLock::writeLock()
{
	_writeLock.lock();
	_readLock.lock();
}

void RWLock::writeUnlock()
{
	_readLock.unlock();
	_writeLock.unlock();		
}

RWLock::RWLock( Phenix::Int32 readCntMax /*= 0*/ )	
	:_readCntMax(readCntMax)
{
	
}

}
}