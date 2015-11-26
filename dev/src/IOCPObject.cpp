#include "stdafx.h"
#include <Phenix/Net/IOCPObject.h>

namespace Phenix
{
namespace Net
{

IOCPObject::~IOCPObject()
{
	if (!_hnd)
	{
		CloseHandle(_hnd);
	}	
}

bool IOCPObject::Create( Phenix::UInt8 numberOfConcurrentThreads )
{
	if (_hnd)
	{
		return false;
	}
	_hnd = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, IGNORED_VAL, numberOfConcurrentThreads);		 
	return _hnd;
}

bool IOCPObject::Bind( HANDLE olObjHnd, void* olObjParam )
{
	if (!_hnd || !olObjHnd)
	{
		return false;
	}
	return CreateIoCompletionPort(olObjHnd, _hnd, (ULONG_PTR)olObjParam, IGNORED_VAL);	
}

bool IOCPObject::CreateAndBind( Phenix::UInt8 numberOfConcurrentThreads, HANDLE olObjHnd, void* olObjParam )
{
	if (_hnd || !olObjHnd)
	{
		return false;
	}
	_hnd = CreateIoCompletionPort(olObjHnd, NULL, (ULONG_PTR)olObjParam, numberOfConcurrentThreads);	
	return true;
}

}
}
