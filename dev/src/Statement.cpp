#include "stdafx.h"
#include <Phenix/MySql/Statement.h>
#include <mysql.h>
#include <Phenix/MySql/Connection.h>
#include <Phenix/Base/Exception.h>

namespace Phenix
{
namespace MySql
{


void Statement::query( const Phenix::String& sql, std::vector<ResultSet>& results )
{	
	if (mysql_real_query(_con.getMySql(), sql.c_str(), sql.length()))	
	{		
		throw MySqlException(mysql_error(_con.getMySql()));
	}
	do 
	{
		MYSQL_RES* mysql_res = mysql_use_result(_con.getMySql());
		if (mysql_res)
		{
			ResultSet result(*mysql_res);
			MYSQL_ROW sql_row;
			while (sql_row = mysql_fetch_row(mysql_res))//获取具体的数据
			{
				result.addRow(sql_row, mysql_res->lengths);
			}
		}
		mysql_free_result(mysql_res);
	} while (!mysql_next_result(_con.getMySql()));			
}

void Statement::execute( const Phenix::String& sql )
{	
	if (mysql_real_query(_con.getMySql(), sql.c_str(), sql.length()))
	{		
		throw MySqlException(mysql_error(_con.getMySql()));
	}	
}

void Statement::executeBatch( const std::vector<Phenix::String>& sqls )
{		
	try
	{
		if (mysql_autocommit(_con.getMySql(), false))
		{			
			throw MySqlException(mysql_error(_con.getMySql()));
		}
		for (Phenix::Int32 i=0; i<sqls.size(); ++i)
		{
			execute(sqls[i]);
		}
		
		if (mysql_commit(_con.getMySql()))
		{			
			throw MySqlException(mysql_error(_con.getMySql()));
		}		
	}
	catch (Exception& e)
	{
		if (mysql_rollback(_con.getMySql()))
		{
			throw MySqlException(mysql_error(_con.getMySql()));
		}		
		e.rethrow();
	}
	
	if (mysql_autocommit(_con.getMySql(), true))
	{		
		throw MySqlException(mysql_error(_con.getMySql()));
	}
	
}

Phenix::UInt32 Statement::escape( const Phenix::String& in, Phenix::String& out )
{	
	memset(_escape_buffer, 0, STR_BUFFER_SIZE);
	Phenix::UInt32 real_len = mysql_real_escape_string(_con.getMySql(), _escape_buffer, in.c_str(), in.length());
	out.assign(in);
	return real_len;
}

Phenix::UInt32 Statement::escape( const char* in, Phenix::UInt32 in_len, char* out )
{	
	memset(_escape_buffer, 0, STR_BUFFER_SIZE);
	Phenix::UInt32 real_len = mysql_real_escape_string(_con.getMySql(), out, in, in_len);	
	return real_len;
}

void Statement::buildPrepare( const Phenix::String& prepare_sql )
{
	memset(_stmt_buffer, 0, STMT_PARAM_BUFFER_SIZE);
	_stmt_params.clear();
	_stmt_buffer_len = 0;
	if (mysql_stmt_prepare(_con.getMySqlStmt(), prepare_sql.c_str(), prepare_sql.size()))
	{		
		throw MySqlException(mysql_stmt_error(_con.getMySqlStmt()));
	}
}

void Statement::executePrepare()
{
	Phenix::Int32 size = _stmt_params.size();
	MYSQL_BIND* binds = new MYSQL_BIND[size];
	for (Phenix::Int32 i=0; i<size; ++i)
	{
		*(binds+i) = _stmt_params[i];
	}

	try
	{
		if (mysql_stmt_bind_param(_con.getMySqlStmt(), binds) || mysql_stmt_execute(_con.getMySqlStmt()))
		{			
			throw MySqlException(mysql_stmt_error(_con.getMySqlStmt()));
		}
	}
	catch (MySqlException& e)
	{
		delete [] binds;
		e.rethrow();
	}
	delete [] binds;	
}

}
}
