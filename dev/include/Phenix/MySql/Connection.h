/*******************************************************************************
 * @brief   数据库连接
 * @date	2016-2-5
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_CONNECTION_H
#define PHENIX_MYSQL_CONNECTION_H

#include <mysql.h>
#include <Phenix/Base/Noncopyable.h>

namespace Phenix
{
namespace MySql
{

class Connection
	:Phenix::Noncopyable
{
public:
	Connection();
	virtual ~Connection();

	bool connect(const Phenix::String& host, const Phenix::String& database,
		const Phenix::String& user, const Phenix::String& passwd, Phenix::UInt16 port = 3306);
	void disconnect();	

	inline MYSQL*		getMySql() { return &_mysql; }	
	inline MYSQL_STMT*	getMySqlStmt() { return &_mysql_stmt; }

private:
	MYSQL		_mysql;
	MYSQL_STMT	_mysql_stmt;
	
	bool		_connected;
};


} // end namespace MySql
} // end namespace Phenix

#endif