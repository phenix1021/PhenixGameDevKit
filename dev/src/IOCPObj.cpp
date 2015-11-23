#include "stdafx.h"
#include <Phenix/Net/IOCPObj.h>

namespace Phenix
{
namespace Net
{

IOCPObj::~IOCPObj()
{
	if (_hnd != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hnd);
	}	
}

bool IOCPObj::Create( Phenix::UInt8 numberOfConcurrentThreads )
{
	if (_hnd != INVALID_HANDLE_VALUE)
	{
		return true;
	}
	HANDLE rlt = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, IGNORED_VAL, numberOfConcurrentThreads);
	if (!rlt)
	{
		return false;
	}
	_hnd = rlt;
	return true;
}

bool IOCPObj::Bind( HANDLE olObjHnd, void* olObjParam )
{
	if (_hnd == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	HANDLE rlt = CreateIoCompletionPort(olObjHnd, _hnd, (ULONG_PTR)olObjParam, IGNORED_VAL);
	if (!rlt)
	{
		return false;
	}
	
	return true;
}

}
}
