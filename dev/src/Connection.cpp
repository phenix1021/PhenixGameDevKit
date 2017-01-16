#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/Connection.h>

namespace Phenix
{
namespace MySql
{
	
Connection::Connection()
	:_connected(false)
{
	
}

Connection::~Connection()
{
	disconnect();
}

bool Connection::connect( const Phenix::String& host, const Phenix::String& user, 
	const Phenix::String& passwd, const Phenix::String& db, Phenix::UInt16 port /*= 3306*/ )
{
	disconnect();
	if (!mysql_init(&_mysql))
	{
		std::cout<<" fail to init mysql, the error "<<mysql_error(&_mysql)<<std::endl;
		return false;
	}	
	
	_mysql_stmt = *mysql_stmt_init(&_mysql);
	
	mysql_options(&_mysql, MYSQL_OPT_RECONNECT, &(const my_bool&) my_bool(true));
	//mysql_options(&_mysql, MYSQL_REPORT_DATA_TRUNCATION, &(const my_bool&) my_bool(true));
	mysql_options(&_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	
	if (mysql_real_connect(&_mysql, host.c_str(), user.c_str(), passwd.c_str(),
		db.c_str(), port, 0, CLIENT_MULTI_STATEMENTS))
	{
		_connected = true;
		return true;
	}
	std::cout<<mysql_error(&_mysql)<<std::endl;
	return false;
}

void Connection::disconnect()
{
	if (_connected)
	{
		mysql_stmt_close(&_mysql_stmt);
		mysql_close(&_mysql);		
		_connected = false;
	}
}

} // end namespace  MySql
} // end namespace Phenix

