/*******************************************************************************
 * @brief   query语句
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_QUERY_H
#define PHENIX_MYSQL_QUERY_H

#include <mysql.h>
#include <Phenix/Base/Noncopyable.h>
#include <Phenix/MySql/RecordSet.h>
#include <Phenix/MySql/Connection.h>

namespace Phenix
{
namespace MySql
{

class Connection;

class Query
	:Phenix::Noncopyable	// 禁止copy，防止数据拷贝影响效率
{
public:
	Query(Connection* conn, const Phenix::String& sql, Phenix::UInt8 bind_param_cnt);
	virtual ~Query();

	template<typename T> Query& operator << (T t); // 不能用T&，否则无法支持传入常量
	
	void select(std::vector<RecordSet>& results, Phenix::UInt32 prefetch_rows = 0);
	Phenix::UInt32 execute();

private:
	enum{
		BIND_MAX_NUM				= 100,	 // 输入和输出的总绑定数上限
		INPUT_PARAM_BUFFER_LENGTH	= 4096	 // 输入参数缓冲区长度
	};
	Connection*		_conn;
	char			_input_params_buffer[INPUT_PARAM_BUFFER_LENGTH];
	MYSQL_BIND		_bind[BIND_MAX_NUM];
	Phenix::UInt8	_bind_idx;
	Phenix::UInt8	_bind_param_cnt;
	Phenix::UInt32  _input_param_buffer_offset;
};

template<typename T>
Query& Phenix::MySql::Query::operator<<( T t )
{	
	if (_bind_idx >= _bind_param_cnt)
	{
		return *this;
	}
	_bind[_bind_idx].buffer = memcpy(_input_params_buffer+_input_param_buffer_offset, &t, sizeof(T));
	_input_param_buffer_offset += sizeof(T);
	if (_input_param_buffer_offset > INPUT_PARAM_BUFFER_LENGTH)
	{
		throw;
	}
	_bind[_bind_idx].buffer_length = sizeof(T);
	
	if (typeid(T) == typeid(Phenix::Int8))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_TINY;
		_bind[_bind_idx].is_unsigned = false;		
	}
	else if (typeid(T) == typeid(Phenix::UInt8))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_TINY;
		_bind[_bind_idx].is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int16))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_SHORT;
		_bind[_bind_idx].is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt16))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_SHORT;
		_bind[_bind_idx].is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int32))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_LONG;
		_bind[_bind_idx].is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt32))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_LONG;
		_bind[_bind_idx].is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int64))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_LONGLONG;
		_bind[_bind_idx].is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt64))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_LONGLONG;
		_bind[_bind_idx].is_unsigned = true;
	} 
	else if (typeid(T) == typeid(float))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_FLOAT;		
	} 
	else if (typeid(T) == typeid(double))
	{
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_DOUBLE;		
	} 
	else if (typeid(T) == typeid(Phenix::String))
	{
		// phenix todo: 处理字符串类型
		//_param_bind[_param_bind_idx]->buffer_type = MYSQL_TYPE_VAR_STRING;
		//_param_bind[_param_bind_idx]->buffer_length = t.size();
		//mysql_real_escape_string
	}
	else
	{
		// 视为struct或class, 注意应按1字节对齐，二进制形式写入
		_bind[_bind_idx].buffer_type = MYSQL_TYPE_STRING;// MYSQL_TYPE_VAR_STRING 也行		
	}	

	if (++_bind_idx >= _bind_param_cnt)
	{
		mysql_stmt_bind_param(_conn->getMySqlStmt(), _bind);
		_input_param_buffer_offset = 0;
	}

	return *this;
}



} // end namespace MySql
} // end namespace Phenix

#endif