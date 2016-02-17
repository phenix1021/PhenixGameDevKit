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


Query::Query(Connection* conn, const Phenix::String& sql, Phenix::UInt8 param_cnt)
:_conn(conn), _param_bind_idx(0)
{
	mysql_stmt_prepare(_conn->getStmt(), sql.c_str(), sql.size());
	_param_bind = param_cnt ? new MYSQL_BIND[param_cnt] : NULL;
}

Query::~Query()
{
	delete [] _param_bind;
}

template<typename T>
Query& Query::operator<<(T& t)
{	
	Phenix::UInt8 param_cnt = *(_param_bind-sizeof(void*))/sizeof(MYSQL_BIND);
	if (!_param_bind || _param_bind_idx >= param_cnt)
	{
		return *this;
	}
	//_param_bind[_param_bind_idx]->buffer = ;
	_param_bind[_param_bind_idx]->buffer_length = sizeof(T);
	type_info& ti = typeid(T);
	if (ti == typeid(Phenix::Int8))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_TINY;
		_param_bind[_param_bind_idx]->is_unsigned = false;		
	}
	else if (ti == typeid(Phenix::UInt8))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_TINY;
		_param_bind[_param_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int16))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_SHORT;
		_param_bind[_param_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt16))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_SHORT;
		_param_bind[_param_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int32))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_LONG;
		_param_bind[_param_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt32))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_LONG;
		_param_bind[_param_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int64))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_LONGLONG;
		_param_bind[_param_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt64))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_LONGLONG;
		_param_bind[_param_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(float))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_FLOAT;		
	} 
	else if (ti == typeid(double))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_DOUBLE;		
	} 
	else if (ti == typeid(Phenix::String))
	{
		_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_VAR_STRING;
		_param_bind[_param_bind_idx]->buffer_length = t.size();
		//mysql_real_escape_string
	} 
	++_param_bind_idx;
	//dynamic_cast<>
	if (_param_bind_idx >= param_cnt)
	{
		mysql_stmt_bind_param(_conn->getStmt(), _param_bind);
	}

	return *this;
}
/*

const Poco::Int32 MysqlQuerier::GetTypeSize(MYSQL_FIELD* field) const
{
	// (TODO:类型的完善)
	switch(field->type)
	{
	case MYSQL_TYPE_TINY:
		return 1;
	case MYSQL_TYPE_SHORT:
		return 2;
	case MYSQL_TYPE_LONG:
		return 4;
	case MYSQL_TYPE_LONGLONG:
		return 8;
	case MYSQL_TYPE_FLOAT:
		return 4;
	case MYSQL_TYPE_DOUBLE:
		return 8;
	case MYSQL_TYPE_STRING:
		return field->length;
	case MYSQL_TYPE_VAR_STRING:
		return field->length;
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_TIMESTAMP:
		return sizeof(MYSQL_TIME);
	case MYSQL_TYPE_BLOB:
		return 32768;	// return for buffer limit
	default:
		return field->length;
	}
}*/

} // end namespace  MySql
} // end namespace Phenix

