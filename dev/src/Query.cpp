#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/Query.h>
#include <Phenix/MySql/Connection.h>
#include <Phenix/MySql/RecordSet.h>

namespace Phenix
{
namespace MySql
{
	
// Query::Query( Connection* conn )
// 	:_conn(conn)
// {
// 
// }
// 
// Query::~Query()
// {
// 
// }
// 
// bool Query::prepare()
// {
// 	if (mysql_stmt_prepare(_conn->getStmt(), _sql.c_str(), _sql.size()))
// 	{
// 		std::cout<<mysql_stmt_error(_conn->getStmt())<<std::endl;
// 		return false;
// 	}
// 	return true;
// }


Query::Query(Connection* conn, const Phenix::String& sql)
{

}

Query::~Query()
{

}

template<typename T>
void Phenix::MySql::Query::operator<<(T& t)
{

}

} // end namespace  MySql
} // end namespace Phenix

