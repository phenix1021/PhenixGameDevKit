// thread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Concurrent/FastMutex.h>
#include <Phenix/Concurrent/AtomLock.h>
#include <Phenix/Concurrent/ScopedLock.h>
#include <Phenix/MySql/Connection.h>
#include <Phenix/MySql/Query.h>

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

//#pragma pack(push,1)
struct LZY	
{
	int a;
	short b;
	float c;
	//char d[3];
};
//#pragma pack(pop)
void main()
{
	using Phenix::MySql::Connection;
	Connection* conn = new Connection();
	if (conn->connect("127.0.0.1", "root", "123456", "test", 3307))
	{
		//std::cout<<"yes"<<std::endl;
	}


	Phenix::String s = "INSERT INTO workers3(bin) values(?);";
	mysql_stmt_prepare(conn->getStmt(), s.c_str(), s.size());
	MYSQL_BIND binds[1];
	memset(binds, 0, sizeof(binds));
    LZY val;
	val.a = 10;
	val.b = 20;
	val.c = 1.0;
	//val.d = "ok";
	//char buff[100];
	//memcpy(buff, &val, sizeof(val));
	binds[0].buffer = &val;//buff;
	binds[0].buffer_type = MYSQL_TYPE_VAR_STRING;	
	binds[0].buffer_length = sizeof(val);
// 	binds[1].buffer = (void*)&val;
// 	binds[1].buffer_type = MYSQL_TYPE_VAR_STRING;
// 	binds[1].buffer_length = sizeof(val);
// 	binds[2].buffer = (void*)&val;
// 	binds[2].buffer_type = MYSQL_TYPE_STRING;
// 	binds[2].buffer_length = sizeof(val);
// 	binds[3].buffer = (void*)&val;
// 	binds[3].buffer_type = MYSQL_TYPE_STRING;
// 	binds[3].buffer_length =sizeof(val);

	mysql_stmt_bind_param(conn->getStmt(), binds);	
	mysql_stmt_execute(conn->getStmt());
	mysql_stmt_free_result(conn->getStmt());

	s = "select bin from workers3;";
	mysql_stmt_prepare(conn->getStmt(), s.c_str(), s.size());
	MYSQL_BIND rlt[1];
	memset(rlt, 0, sizeof(rlt));
	mysql_stmt_execute(conn->getStmt());
	MYSQL_RES* meta = mysql_stmt_result_metadata(conn->getStmt());
	MYSQL_FIELD* field = NULL;
	int i = 0;
	LZY v[1];
	//char dst[100];
	while (field = mysql_fetch_field(meta))
	{
		rlt[i].buffer = v;
		rlt[i].buffer_type = field->type;
		rlt[i].buffer_length = field->length;
		++i;
	}
	//mysql_stmt_free_result(conn->getStmt());
	mysql_stmt_bind_result(conn->getStmt(), rlt);
    mysql_stmt_execute(conn->getStmt());
	mysql_stmt_store_result(conn->getStmt());
	//std::cout<<mysql_len_(conn->getStmt());
	while(1)
	{
		if (mysql_stmt_fetch(conn->getStmt()))
		{
			break;
		}
	}

	
	getchar();
	
}

int _tmain1(int argc, _TCHAR* argv[])
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

	using Phenix::MySql::Connection;
	Connection* conn = new Connection();
	if (conn->connect("127.0.0.1", "root", "123456", "test", 3307))
	{
		//std::cout<<"yes"<<std::endl;
	}
/*
 	Phenix::String s = "INSERT INTO workers values(?,'Tom',?,'a good girl!')";
 	mysql_stmt_prepare(conn->getStmt(), s.c_str(), s.size());

	for (int i=4; i<8; ++i)
	{
		MYSQL_BIND arr[2];
		memset(arr, 0, sizeof(arr));
		arr[0].buffer_type = MYSQL_TYPE_LONG;
		//arr[0].is_unsigned = true;
		arr[0].buffer_length = 4;
		Phenix::UInt32 id = i;
		arr[0].buffer = &id;
		arr[1].buffer_type = MYSQL_TYPE_SHORT;
		arr[1].buffer_length = 2;
		Phenix::Int16 score = 20;
		arr[1].buffer = &score;
		mysql_stmt_bind_param(conn->getStmt(), arr);
		if (!mysql_stmt_execute(conn->getStmt()))
		{
			std::cout<<mysql_stmt_affected_rows(conn->getStmt())<<std::endl;
		}
	}*/

	Phenix::String s1 = "SELECT id, score FROM workers;SELECT id, score FROM workers2;";
	mysql_stmt_prepare(conn->getStmt(), s1.c_str(), s1.size());

/*
	MYSQL_RES* meta = mysql_stmt_result_metadata(conn->getStmt());
	std::cout<<mysql_num_fields(meta)<<std::endl;
	MYSQL_FIELD* field = NULL;
	while (field = mysql_fetch_field(meta))
	{
		std::cout<<field->name<<","
			<<field->type<<","
			<<field->db_length<<","
			<<std::endl;
	}*/
	
	MYSQL_BIND rlt[2];
	memset(rlt, 0, sizeof(rlt));
	rlt[0].buffer_type = MYSQL_TYPE_LONG;	
	rlt[0].buffer_length = 4;
	Phenix::UInt32 id = 0;
	rlt[0].buffer = &id;
	rlt[1].buffer_type = MYSQL_TYPE_SHORT;
	rlt[1].buffer_length = 2;
	Phenix::Int16 score = 0;
	rlt[1].buffer = &score;
	mysql_stmt_bind_result(conn->getStmt(),rlt);
/*
	mysql_stmt_execute(conn->getStmt());
	mysql_stmt_store_result(conn->getStmt());
	//std::cout<<mysql_stmt_field_count(conn->getStmt())<<std::endl;
	//std::cout<<mysql_field_count(conn->getDriver())<<std::endl;
	//std::cout<<mysql_num_fields(conn->getStmt())<<std::endl;
	std::cout<<mysql_stmt_num_rows(conn->getStmt())<<std::endl;	
	while (!mysql_stmt_fetch(conn->getStmt()))
	{
		std::cout<<id<<","<<score<<std::endl;
	}*/
	unsigned long type = CURSOR_TYPE_READ_ONLY;//CURSOR_TYPE_NO_CURSOR;
	unsigned long prefetch_rows = 2;
	if (mysql_stmt_attr_set(conn->getStmt(), STMT_ATTR_CURSOR_TYPE, (void*)&type) ||
		mysql_stmt_attr_set(conn->getStmt(), STMT_ATTR_PREFETCH_ROWS, (void*)&prefetch_rows))
	{
		std::cout<<mysql_stmt_error(conn->getStmt())<<std::endl;
		return 0;
	}	
	mysql_stmt_execute(conn->getStmt());
	//std::cout<<mysql_num_rows(conn->getStmt())<<std::endl;	
	while (!mysql_stmt_fetch(conn->getStmt()))
	{
		std::cout<<id<<","<<score<<std::endl;
	}

	Phenix::MySql::Query query(conn, "select * from worker;");
		
	getchar();
	return 0;
}

