#include "stdafx.h"
#include <Phenix/MySql/ResultSet.h>

namespace Phenix
{
namespace MySql
{

ResultSet::ResultSet( const MYSQL_RES& mysql_res )
	:_cur_row_idx(-1)
{
	Field field;
	for (Phenix::Int32 i=0; i<mysql_res.field_count; ++i)
	{
		field.idx = i;
		field.name = mysql_res.fields[i].name;		
		field.data_type = mysql_res.fields[i].type;
		_fields[field.name] = field;
	}
	
}

void ResultSet::addRow( const MYSQL_ROW& mysql_row, unsigned long* lengths )
{
	char* buffer = new char[200]; // ÁÙÊ±·ÖÅä
	memset(buffer, 0, 200);

	Data data;
	++_cur_row_idx;
	for (Phenix::Int32 i=0; i<_fields.size(); ++i)
	{
		data.head = buffer;
		data.len = lengths[i];
		memcpy(data.head, mysql_row[i], data.len);		 
		buffer += (data.len+1); // +1Îª/0
		_rows[_cur_row_idx].data_str.push_back(data);
	}	
}

void ResultSet::firstRow()
{
	_cur_row_idx = (_rows.empty()? -1 : 0);	
}

bool ResultSet::nextRow()
{
	if (_cur_row_idx+1 < _rows.size())
	{
		++_cur_row_idx;
		return true;
	}
	return false;
}




} // end namespace  MySql
} // end namespace Phenix

