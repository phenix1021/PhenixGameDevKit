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


Query::Query(Connection* conn, const Phenix::String& sql, Phenix::UInt8 bind_param_cnt)
	:_conn(conn), _bind_idx(0), _bind_param_cnt(bind_param_cnt)
{
	if (_bind_param_cnt > BIND_MAX_NUM)
	{
		throw;
	}
	memset(_bind, 0, sizeof(_bind));
	mysql_stmt_prepare(_conn->getStmt(), sql.c_str(), sql.size());
}

Query::~Query()
{
	
}

template<typename T>
Query& Query::operator<<(T& t)
{	
	//Phenix::UInt8 param_cnt = *(_bind-sizeof(void*))/sizeof(MYSQL_BIND);
	if (_bind_idx >= _bind_param_cnt)
	{
		return *this;
	}
	_bind[_bind_idx]->buffer = &t;
	_bind[_bind_idx]->buffer_length = sizeof(T);
	type_info& ti = typeid(T);
	if (ti == typeid(Phenix::Int8))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_TINY;
		_bind[_bind_idx]->is_unsigned = false;		
	}
	else if (ti == typeid(Phenix::UInt8))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_TINY;
		_bind[_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int16))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_SHORT;
		_bind[_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt16))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_SHORT;
		_bind[_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int32))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_LONG;
		_bind[_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt32))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_LONG;
		_bind[_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(Phenix::Int64))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_LONGLONG;
		_bind[_bind_idx]->is_unsigned = false;
	}
	else if (ti == typeid(Phenix::UInt64))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_LONGLONG;
		_bind[_bind_idx]->is_unsigned = true;
	} 
	else if (ti == typeid(float))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_FLOAT;		
	} 
	else if (ti == typeid(double))
	{
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_DOUBLE;		
	} 
	else if (ti == typeid(Phenix::String))
	{
		// phenix todo: 处理字符串类型
		//_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_VAR_STRING;
		//_param_bind[_param_bind_idx]->buffer_length = t.size();
		//mysql_real_escape_string
	}
	else
	{
		// 视为struct或class, 注意应按1字节对齐，二进制形式写入
		_bind[_bind_idx]->buffer_type = MYSQL_TYPE_STRING;// MYSQL_TYPE_VAR_STRING 也行		
	}	
	
	if (++_bind_idx >= _bind_param_cnt)
	{
		mysql_stmt_bind_param(_conn->getStmt(), _bind);		
	}

	return *this;
}

void Query::select( std::vector<RecordSetBase*>& results, Phenix::UInt32 prefetch_rows/* = 0*/ )
{
	Phenix::Int32 next_rlt_code = 0;
	Phenix::Int32 result_idx = 0;
	do 
	{
		QueryResult rlt;
		MYSQL_RES* meta = mysql_stmt_result_metadata(_conn->getStmt());
		if (!meta)
		{
			//rlt.affected_rows = mysql_stmt_affected_rows(_conn->getStmt());
			//results.push_back(rlt);
			continue;
		}		
		
		if (mysql_stmt_attr_set(_conn->getStmt(), STMT_ATTR_CURSOR_TYPE, 
			(void*)new Phenix::UInt32(!prefetch_rows? CURSOR_TYPE_NO_CURSOR : CURSOR_TYPE_READ_ONLY))
			||
			mysql_stmt_attr_set(_conn->getStmt(), STMT_ATTR_PREFETCH_ROWS, 
			(void*)new Phenix::UInt32(prefetch_rows)))
		{
			std::cout<<mysql_stmt_error(_conn->getStmt())<<std::endl;
			throw; // 抛出异常
		}
		mysql_stmt_execute(_conn->getStmt());
// 		if (!prefetch_rows)
// 		{
// 			mysql_stmt_store_result(_conn->getStmt()); // 可以不用，直接fetch就行
// 		}	
		
		char* buffer_pos = char*(results[result_idx].getBuffer());
		do 
		{
			if (_bind_idx + meta->field_count >= BIND_MAX_NUM)
			{
				throw;
			}
			for (Phenix::Int32 i=0; i<meta->field_count; ++i)
			{
				_bind[_bind_idx+i].buffer_length = meta->fields[i].length;
				_bind[_bind_idx+i].buffer_type = meta->fields[i].type;
				_bind[_bind_idx+i].buffer = buffer_pos;
				buffer_pos += meta->fields[i].length;
			}
		} while (!mysql_stmt_fetch(_conn->getStmt()));

		mysql_free_result(meta);
		next_rlt_code = mysql_stmt_next_result(_conn->getStmt());
		if (next_rlt_code > 0)
		{
			std::cout<<mysql_stmt_error(_conn->getStmt())<<std::endl;
			throw; // 抛出异常
		}
		++result_idx;
	} while (!next_rlt_code);
	_bind_idx = 0;
}

Phenix::UInt32 Query::execute()
{
	mysql_stmt_execute(_conn->getStmt());
	return mysql_stmt_affected_rows(_conn->getStmt());
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

