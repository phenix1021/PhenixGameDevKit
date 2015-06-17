// thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Concurrent/FastMutex.h>
#include <Phenix/Concurrent/AtomLock.h>

using Phenix::Concurrent::Thread;

class TestThread
{
public:
	TestThread():cnt(0){}
	void run()
	{
		while (cnt++ <= 100)
		{			
			std::cout<<Thread::getThreadID()<<": cnt is "<<cnt<<std::endl;
		}		
	}

private:
	volatile Phenix::Int32 cnt;
};

int _tmain(int argc, _TCHAR* argv[])
{	
	TestThread func;	
	Thread t1("thread1");
	Thread t2("thread2");
 	t1.start(Phenix::Bind(&TestThread::run, &func));
 	t2.start(Phenix::Bind(&TestThread::run, &func));
	//Thread::join()

	return 0;
}

