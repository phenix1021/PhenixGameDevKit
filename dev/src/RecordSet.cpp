#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/RecordSet.h>

namespace Phenix
{
namespace MySql
{

Phenix::Memory::ObjectPool<RecordSet::Row>	RecordSet::row_pool;

void RecordSet::addRow( void* ptr )
{	 
	_rows.push_back(Phenix::SharedPtr<Row>(row_pool.Create(ptr)));	
}

void* RecordSet::allocRow()
{
	// 从record_pool池分配
	void* ptr = malloc(_row_length);
	memset(ptr, 0, _row_length);
	return ptr;
}

void RecordSet::first()
{
	_row_idx = 0;
}

void RecordSet::next()
{
	if (_row_idx < _rows.size())
	{
		++_row_idx; // 最终到达_rows.size()
	}	
}

bool RecordSet::isEOF()
{
	return _row_idx == _rows.size();
}

Phenix::UInt32 RecordSet::getColLength( MYSQL_FIELD& field )
{
	switch(field.type)
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
	case MYSQL_TYPE_VAR_STRING:
		return field.length;	
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_TIMESTAMP:
		return sizeof(MYSQL_TIME);
	case MYSQL_TYPE_BLOB:
		return 32768;	// return for buffer limit
	default:
		return field.length;
	}
}

RecordSet::Row::Row( void* ptr )
	:head(ptr)
{
}

RecordSet::Row::~Row()
{
	// 归还record_pool池
	free(head);	
}

void RecordSet::Row::operator delete( void* ptr )
{
	RecordSet::row_pool.Release((RecordSet::Row*)ptr, false);
}


} // end namespace  MySql
} // end namespace Phenix

