#include "stdafx.h"
#include <Phenix/Base/BugCheck.h>
#include <Windows.h>
#include <dbghelp.h>
#include <Phenix/Base/Exception.h>
#include <Phenix/Concurrent/Thread.h>


#pragma comment(lib, "dbghelp.lib")

namespace Phenix
{

void BugCheck::assertion( const Phenix::String& cond )
{
	invokeDebugger();
	throw new AssertionViolationException(cond);
}

void BugCheck::nullPoionter( const Phenix::String& ptr )
{
	invokeDebugger();
	throw new NullPointerException(ptr);
}

void BugCheck::invokeDebugger()
{
#if defined(_DEBUG)
	if (IsDebuggerPresent())
	{
		DebugBreak();
	}
#endif
	void(0);
}

long __stdcall BugCheck::CreateDumpFile( const char* dumpFileName, _EXCEPTION_POINTERS* ep )
{
	HANDLE dumpFile;
	// 创建MiniDump文件	
	try
	{
		dumpFile = CreateFileA(dumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = ep;
		dumpInfo.ThreadId = Phenix::Concurrent::Thread::getCurThreadID();
		dumpInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	}
	catch (...)
	{
		CloseHandle(dumpFile);	
	}
	return EXCEPTION_EXECUTE_HANDLER;
}


} // end namespace Phenix

