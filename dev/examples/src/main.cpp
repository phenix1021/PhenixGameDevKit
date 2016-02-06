// thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Concurrent/FastMutex.h>
#include <Phenix/Concurrent/AtomLock.h>
#include <Phenix/Concurrent/ScopedLock.h>
//#include <Phenix/MySql/Connection.h>
//#include <Phenix/MySql/Query.h>
#include <mysql.h>

using Phenix::Concurrent::Thread;

class TestThread
{
public:
	TestThread(){}
	
	void run()
	{
		while (true)
		{			
			//Phenix::Concurrent::ScopedLock<Phenix::Concurrent::AtomLock> lock(mutex);
			dosth();			
		}		
	}	

private:
	void dosth()// 非同步，会出现访问冲突造成逻辑错误
	{
		Phenix::Concurrent::ScopedLock<Phenix::Concurrent::AtomLock> lock(mutex);
		//++cnt;
		Thread::sleep(1000);
		std::cout<<Thread::getThreadID()<<std::endl; //非线程安全
		//printf("%d\n", Thread::getThreadID());
	}

private:
	//volatile Phenix::Int32 cnt;	
	Phenix::Concurrent::AtomLock mutex;
};

int _tmain(int argc, _TCHAR* argv[])
{		
	/*TestThread func;	
	Thread t1("thread1");
	Thread t2("thread2");
 	t1.start(Phenix::Bind(&TestThread::run, &func));
 	t2.start(Phenix::Bind(&TestThread::run, &func));
	std::vector<Thread*> vec;
	vec.push_back(&t1);
	vec.push_back(&t2);
	Thread::join(vec);*/

// 	using Phenix::MySql::Connection;
// 	Connection* conn = new Connection();
// 	if (conn->connect("localhost", "test", "root", "123456"))
// 	{
// 		std::cout<<"yes"<<std::endl;
// 	}

	MYSQL mysql;
	mysql_init(&mysql);
	getchar();
	return 0;
}

