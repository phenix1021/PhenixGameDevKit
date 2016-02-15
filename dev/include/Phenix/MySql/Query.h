/*******************************************************************************
 * @brief   query”Ôæ‰
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_QUERY_H
#define PHENIX_MYSQL_QUERY_H

#include <mysql.h>

namespace Phenix
{
namespace MySql
{

class Connection;
class QueryResult;

class Query
{
public:
	Query(Connection* conn, const Phenix::String& sql);
	virtual ~Query();

	template<typename T> void operator << (T& t);
	
	QueryResult* execute(){return 0;}

private:
	Connection*		_conn;
	Phenix::String	_sql;	
};



} // end namespace MySql
} // end namespace Phenix

#endif