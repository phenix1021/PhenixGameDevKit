/*******************************************************************************
 * @brief   数据结果集
 * @date	2016-11-28
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_RESULTSET_H
#define PHENIX_MYSQL_RESULTSET_H

#include <mysql.h>

namespace Phenix
{
namespace MySql
{

class Connection
{
public:
	Connection();
	virtual ~Connection();

	bool connect(const Phenix::String& host, const Phenix::String& database,
		const Phenix::String& user, const Phenix::String& passwd, Phenix::UInt16 port = 3306);
	void disconnect();
	bool reconnect();

	inline MYSQL* getDriver() { return &_driver; }	
	inline MYSQL_STMT* getStmt() { return &_stmt; }

private:
	MYSQL		_driver;
	MYSQL_STMT	_stmt;
	
	bool		_connected;
};


} // end namespace MySql
} // end namespace Phenix

#endif