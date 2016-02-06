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

bool Connection::reconnect()
{
	if (mysql_ping(&_driver))
	{
		std::cout<<" fail to reconnect, the error "<<mysql_error(&_driver)<<std::endl;
		return false;
	}
	return true;
}

bool Connection::connect( const Phenix::String& host, const Phenix::String& user, 
	const Phenix::String& passwd, const Phenix::String& db, Phenix::UInt16 port /*= 3306*/ )
{
	disconnect();
	if (!mysql_init(&_driver))
	{
		std::cout<<" fail to init mysql, the error "<<mysql_error(&_driver)<<std::endl;
		return false;
	}	
	_stmt = *mysql_stmt_init(&_driver);
	mysql_options(&_driver, MYSQL_OPT_RECONNECT, NULL);
	if (mysql_real_connect(&_driver, host.c_str(), user.c_str(), passwd.c_str(),
		db.c_str(), port, 0, CLIENT_MULTI_STATEMENTS))
	{
		std::cout<<mysql_error(&_driver)<<std::endl;
		return false;
	}
	_connected = true;
	return true;
}

void Connection::disconnect()
{
	if (_connected)
	{
		mysql_close(&_driver);
		if (!mysql_stmt_close(&_stmt))
		{
			std::cout<<mysql_stmt_error(&_stmt)<<std::endl;
			throw;
		}		
		_connected = false;
	}
}

} // end namespace  MySql
} // end namespace Phenix

