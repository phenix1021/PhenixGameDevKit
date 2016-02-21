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
	Query(Connection* conn, const Phenix::String& sql, Phenix::UInt8 param_cnt);
	virtual ~Query();

	template<typename T> Query& operator << (T& t);
	
	QueryResult* execute(){return 0;}

private:
	Connection*		_conn;
	MYSQL_BIND*		_param_bind;
	Phenix::UInt8	_param_bind_idx;
};



} // end namespace MySql
} // end namespace Phenix

#endif