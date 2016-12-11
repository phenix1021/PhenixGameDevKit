#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/Query.h>

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
	:_conn(conn), _bind_idx(0), _bind_param_cnt(bind_param_cnt), _input_param_buffer_offset(0)
{
	if (_bind_param_cnt > BIND_MAX_NUM)
	{
		throw;
	}
	memset(_bind, 0, sizeof(_bind));
	memset(_input_params_buffer, 0, INPUT_PARAM_BUFFER_LENGTH);
	if (mysql_stmt_prepare(_conn->getMySqlStmt(), sql.c_str(), sql.size()))
	{
		std::cout<<mysql_stmt_error(_conn->getMySqlStmt())<<std::endl;
		throw;
	}
}

Query::~Query()
{
	
}

void Query::select( std::vector<RecordSet>& results, Phenix::UInt32 prefetch_rows/* = 0*/ )
{
	Phenix::Int32 next_rlt_code = 0;
	Phenix::Int32 result_idx = 0;
	results.clear();
	mysql_stmt_execute(_conn->getMySqlStmt());
	do 
	{			
		MYSQL_RES* meta = mysql_stmt_result_metadata(_conn->getMySqlStmt());
		if (meta)
		{			
			if (mysql_stmt_attr_set(_conn->getMySqlStmt(), STMT_ATTR_CURSOR_TYPE, 
				(void*)new Phenix::UInt32(!prefetch_rows? CURSOR_TYPE_NO_CURSOR : CURSOR_TYPE_READ_ONLY))
				||
				mysql_stmt_attr_set(_conn->getMySqlStmt(), STMT_ATTR_PREFETCH_ROWS, 
				(void*)new Phenix::UInt32(prefetch_rows)))
			{
				std::cout<<mysql_stmt_error(_conn->getMySqlStmt())<<std::endl;
				throw; // 抛出异常
			}

			//if (!prefetch_rows)
			//{
			//	mysql_stmt_store_result(_conn->getStmt()); // 可以不用，直接fetch就行
			//}	

			Phenix::UInt32 row_length = 0; // 计算行长
			while (MYSQL_FIELD* col = mysql_fetch_field(meta))
			{
				row_length += RecordSet::getColLength(*col);
			}
			RecordSet record_set(row_length);
			bool eof = false;		
			do 
			{
				char* buffer_pos = (char*)(record_set.allocRow());
				char* pos = buffer_pos;
				// 遍历各个字段
				if (_bind_idx + meta->field_count >= BIND_MAX_NUM)
				{
					throw;
				}
				for (Phenix::Int32 i=0; i<meta->field_count; ++i)
				{
					_bind[_bind_idx+i].buffer_length = RecordSet::getColLength(meta->fields[i]);
					_bind[_bind_idx+i].buffer_type = meta->fields[i].type;
					_bind[_bind_idx+i].buffer = pos;
					pos += _bind[_bind_idx+i].buffer_length;
				}
				mysql_stmt_bind_result(_conn->getMySqlStmt(), _bind+_bind_idx);
				eof = mysql_stmt_fetch(_conn->getMySqlStmt());
				if (!eof)
				{
					record_set.addRow(buffer_pos);				
				}
			} while (!eof); // 下一行

			results.push_back(record_set);
			mysql_free_result(meta);	
		}		
		
		next_rlt_code = mysql_stmt_next_result(_conn->getMySqlStmt());
		if (next_rlt_code > 0)
		{
			std::cout<<mysql_stmt_error(_conn->getMySqlStmt())<<std::endl;
			throw; // 抛出异常
		}
		++result_idx;
	} while (!next_rlt_code);
	_bind_idx = 0;
}

Phenix::UInt32 Query::execute()
{
	mysql_stmt_execute(_conn->getMySqlStmt());
	return mysql_stmt_affected_rows(_conn->getMySqlStmt());
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

