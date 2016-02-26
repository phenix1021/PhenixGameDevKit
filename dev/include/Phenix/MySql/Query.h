/*******************************************************************************
 * @brief   query语句
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_QUERY_H
#define PHENIX_MYSQL_QUERY_H

#include <mysql.h>
#include <Phenix/MySql/RecordSet.h>

namespace Phenix
{
namespace MySql
{

class Connection;
class RecordSetBase;

class Query
{
public:
	Query(Connection* conn, const Phenix::String& sql, Phenix::UInt8 bind_param_cnt);
	virtual ~Query();

	template<typename T> Query& operator << (T& t);
	
	void select(std::vector<RecordSet>& results, Phenix::UInt32 prefetch_rows = 0);
	Phenix::UInt32 execute();

private:
	enum{
		BIND_MAX_NUM = 100	 // 输入和输出的总绑定数上限
	};
	Connection*		_conn;
	MYSQL_BIND		_bind[BIND_MAX_NUM];
	Phenix::UInt8	_bind_idx;
	Phenix::UInt8	_bind_param_cnt;
};



} // end namespace MySql
} // end namespace Phenix

#endif