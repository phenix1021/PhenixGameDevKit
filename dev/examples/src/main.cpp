#include "stdafx.h"
#include <Phenix/Concurrent/Thread.h>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Concurrent/FastMutex.h>
#include <Phenix/Concurrent/AtomLock.h>
#include <Phenix/Concurrent/ScopedLock.h>
#include <Phenix/MySql/Connection.h>
#include <Phenix/MySql/Query.h>

#include "UnionFindTest.h"
#include "IndexTest.h"
#include "GraphTest.h"

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

#pragma pack(push,1)
struct LZY	
{
	int a;
	short b;
	float c;
	//char d[3];
};
struct DBR
{
	short num;
	int age;
	DBR(){memset(this, 0, sizeof(*this));}
};
#pragma pack(pop)

void main123()
{
	using Phenix::MySql::Connection;
	Connection* conn = new Connection();
	if (!conn->connect("127.0.0.1", "root", "123456", "test", 3306))
	{
		std::cout<<"fail to connect..."<<std::endl;
		getchar();
		return;
	}

	Phenix::String sql = "select * from temp;";
	if (mysql_real_query(conn->getMySql(), sql.c_str(), sql.length()))
	{
		std::cout<<mysql_error(conn->getMySql())<<std::endl;
	}
	MYSQL_RES* res = mysql_use_result(conn->getMySql());	
// 	Phenix::Int32 len = 0;
// // 	while (MYSQL_FIELD* col = mysql_fetch_field(res))

	while (MYSQL_ROW row = mysql_fetch_row(res))
	{		
		unsigned long* each = mysql_fetch_lengths(res);
		DBR dbr;
		memcpy(&dbr, &row, 8);
		int a = atoi(row[0]);
		std::cout<<dbr.num<<", "<<dbr.age<<std::endl;
	}
	std::cout<<mysql_num_rows(res)<<std::endl;
	getchar();

}

void main1()
{
	using Phenix::MySql::Connection;
	Connection* conn = new Connection();
	if (!conn->connect("127.0.0.1", "root", "123456", "test", 3306))
	{
		std::cout<<"fail to connect..."<<std::endl;
		getchar();
		return;
	}

	Phenix::MySql::Query query(conn, "SELECT * FROM temp WHERE number > 1000 \
		SELECT * FROM temp WHERE number <= 150;", 0);	
	//query << 150 << 1000;
	std::vector<Phenix::MySql::RecordSet> rlt;
	query.select(rlt);

	DBR* p = NULL;
	rlt[0].first();
	while (rlt[0].getData(p))
	{
		std::cout<<p->num<<", "<<p->age<<std::endl;
		rlt[0].next();
	}
	std::cout<<" ========================================= "<<std::endl;
	rlt[1].first();
	while (rlt[1].getData(p))
	{
		std::cout<<p->num<<", "<<p->age<<std::endl;
		rlt[1].next();
	}
	getchar();

}
void main2()
{
	using Phenix::MySql::Connection;
	Connection* conn = new Connection();
	if (!conn->connect("127.0.0.1", "root", "123456", "test", 3306))
	{
		std::cout<<"fail to connect..."<<std::endl;
		getchar();
		return;
	}


	Phenix::String s = "INSERT INTO workers3(bin) values(?);";
	mysql_stmt_prepare(conn->getMySqlStmt(), s.c_str(), s.size());
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

	mysql_stmt_bind_param(conn->getMySqlStmt(), binds);	
	mysql_stmt_execute(conn->getMySqlStmt());
	mysql_stmt_free_result(conn->getMySqlStmt());

	s = "select * from chenyu;";
	mysql_stmt_prepare(conn->getMySqlStmt(), s.c_str(), s.size());
	MYSQL_BIND rlt[1];
	memset(rlt, 0, sizeof(rlt));
	mysql_stmt_execute(conn->getMySqlStmt());
	MYSQL_RES* meta = mysql_stmt_result_metadata(conn->getMySqlStmt());
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
	mysql_stmt_bind_result(conn->getMySqlStmt(), rlt);
    mysql_stmt_execute(conn->getMySqlStmt());
	mysql_stmt_store_result(conn->getMySqlStmt());
	//std::cout<<mysql_len_(conn->getStmt());
	while(1)
	{
		if (mysql_stmt_fetch(conn->getMySqlStmt()))
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
	mysql_stmt_prepare(conn->getMySqlStmt(), s1.c_str(), s1.size());

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
	mysql_stmt_bind_result(conn->getMySqlStmt(),rlt);
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
	if (mysql_stmt_attr_set(conn->getMySqlStmt(), STMT_ATTR_CURSOR_TYPE, (void*)&type) ||
		mysql_stmt_attr_set(conn->getMySqlStmt(), STMT_ATTR_PREFETCH_ROWS, (void*)&prefetch_rows))
	{
		std::cout<<mysql_stmt_error(conn->getMySqlStmt())<<std::endl;
		return 0;
	}	
	mysql_stmt_execute(conn->getMySqlStmt());
	//std::cout<<mysql_num_rows(conn->getStmt())<<std::endl;	
	while (!mysql_stmt_fetch(conn->getMySqlStmt()))
	{
		std::cout<<id<<","<<score<<std::endl;
	}

	Phenix::MySql::Query query(conn, "select * from worker;", 0);
		
	getchar();
	return 0;
}

int main(int argc, char* argv[])
{
	//unionFindTest(argc, argv);
	graphTest(argc, argv);
	//indexTest(argc, argv);
	getchar();
	return 0;
}