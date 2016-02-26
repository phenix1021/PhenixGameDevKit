#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/RecordSet.h>

namespace Phenix
{
namespace MySql
{

Phenix::Memory::ObjectPool<RecordSet::Row>	RecordSet::row_pool;

void* RecordSet::addRow()
{
	void* ptr = NULL;
	if (!(ptr = allocRow()))
	{
		return NULL;
	}		
	_rows.push_back(Phenix::SharedPtr<Row>(row_pool.Create(ptr)));
	return ptr;
}

void* RecordSet::allocRow()
{
	// ��record_pool�ط���
	return malloc(_row_length);	
}


RecordSet::Row::Row( void* ptr )
	:head(ptr)
{
}

RecordSet::Row::~Row()
{
	// �黹record_pool��
	free(head);
}


} // end namespace  MySql
} // end namespace Phenix

