#include "stdafx.h"
#include <Phenix/Concurrent/RWLock.h>

namespace Phenix
{
namespace Concurrent
{	

void RWLock::readLock()
{
	// 以下两行是为了实现正在写时不再添加新的会"写锁"竞争_readLock的读
	_writeLock.lock();
	_writeLock.unlock();

	if (_readCnt.inc() == 1)
	{
		_readLock.lock();	// 保证最多只有一个读线程竞争该锁，表示在读			
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
		_readLock.unlock();	// 释放读锁，表示目前没有在读		
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