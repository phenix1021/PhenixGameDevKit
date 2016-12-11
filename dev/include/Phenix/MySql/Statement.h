/*******************************************************************************
 * @brief   数据库语句
 * @date	2016-12-5
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_STATEMENT_H
#define PHENIX_MYSQL_STATEMENT_H

#include <Phenix/MySql/ResultSet.h>

namespace Phenix
{
namespace MySql
{

class Connection;

class Statement
{
	enum
	{
		STMT_PARAM_BUFFER_SIZE	= 1024*4	// 预处理参数缓存长度		
	};

public:
	Statement(Connection& con):_con(con), _stmt_buffer_len(0){}
	virtual ~Statement();

public: 
	void	query(const Phenix::String& sql, std::vector<ResultSet>& results);
	void	execute(const Phenix::String& sql);
	void	executeBatch(const std::vector<Phenix::String>& sqls);
	
	// 预处理
	void	buildPrepare(const Phenix::String& prepare_sql);
	template<typename T>
	void	bindPrepare(T param);// 不能用T&，否则无法支持传入常量	
	void	executePrepare();	

	Phenix::UInt32	escape(const Phenix::String& in, Phenix::String& out);
	Phenix::UInt32	escape(const char* in, Phenix::UInt32 in_len, char* out);

private:
	Connection&					_con;
	char						_escape_buffer[STR_BUFFER_SIZE];		// escape字符串缓冲区
	char						_stmt_buffer[STMT_PARAM_BUFFER_SIZE];	// 预处理参数缓冲区
	std::vector<MYSQL_BIND>		_stmt_params;
	Phenix::UInt16				_stmt_buffer_len;						// 预处理参数缓冲区当前长度
};

template<typename T>
void Phenix::MySql::Statement::bindPrepare( T param )
{	
	Phenix::UInt16 param_size = sizeof(T);
	if (param_size + _stmt_buffer_len > STMT_PARAM_BUFFER_SIZE)
	{
		throw OutOfMemoryException();
	}
	
	MYSQL_BIND bind;
	if (typeid(T) == typeid(Phenix::String))
	{		
		bind.buffer_type = MYSQL_TYPE_VAR_STRING;
		bind.buffer = _stmt_buffer+_stmt_buffer_len;
		bind.buffer_length = escape(param.c_str(), param.size(), (char*)bind.buffer);
	}
	else
	{
		bind.buffer = memcpy(_stmt_buffer+_stmt_buffer_len, &param, sizeof(T));
		bind.buffer_length = sizeof(T);
	}
	
	if (typeid(T) == typeid(Phenix::Int8))
	{
		bind.buffer_type = MYSQL_TYPE_TINY;
		bind.is_unsigned = false;		
	}
	else if (typeid(T) == typeid(Phenix::UInt8))
	{
		bind.buffer_type = MYSQL_TYPE_TINY;
		bind.is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int16))
	{
		bind.buffer_type = MYSQL_TYPE_SHORT;
		bind.is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt16))
	{
		bind.buffer_type = MYSQL_TYPE_SHORT;
		bind.is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int32))
	{
		bind.buffer_type = MYSQL_TYPE_LONG;
		bind.is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt32))
	{
		bind.buffer_type = MYSQL_TYPE_LONG;
		bind.is_unsigned = true;
	} 
	else if (typeid(T) == typeid(Phenix::Int64))
	{
		bind.buffer_type = MYSQL_TYPE_LONGLONG;
		bind.is_unsigned = false;
	}
	else if (typeid(T) == typeid(Phenix::UInt64))
	{
		bind.buffer_type = MYSQL_TYPE_LONGLONG;
		bind.is_unsigned = true;
	} 
	else if (typeid(T) == typeid(float))
	{
		bind.buffer_type = MYSQL_TYPE_FLOAT;		
	} 
	else if (typeid(T) == typeid(double))
	{
		bind.buffer_type = MYSQL_TYPE_DOUBLE;		
	} 	
	else if (__is_class(T))
	{
		if (__alignof(T) != 1)
		{
			throw OutOfMemoryException(" the struct/class align must be 1! ");
		}
		bind.buffer_type = MYSQL_TYPE_STRING;// MYSQL_TYPE_VAR_STRING 也行	
	}

	_stmt_params.push_back(bind);
	_stmt_buffer_len += sizeof(T);
}


} // end namespace MySql
} // end namespace Phenix

#endif