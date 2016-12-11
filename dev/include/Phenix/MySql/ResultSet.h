/*******************************************************************************
 * @brief   ���ݽ����
 * @date	2016-11-28
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_RESULTSET_H
#define PHENIX_MYSQL_RESULTSET_H

#include <mysql.h>

namespace Phenix
{
namespace MySql
{

class ResultSet
{	
	friend class Statement;
	struct Field
	{		
		Phenix::String	name;		// �ֶ���
		Phenix::UInt16	idx;		// �ֶ����
		Phenix::UInt16	data_type;	// �ֶ�����	

		Field():idx(0), data_type(0){}
	};

	struct Data
	{
		char*			head;		// �����ַ����׵�ַ
		Phenix::UInt32	len;		// �����ַ�������	
		Data():head(NULL), len(0){}
	};

	struct Row
	{		
		std::vector<Data> data_str;
	};

public:
	ResultSet(const MYSQL_RES& mysql_res);
	virtual ~ResultSet(){}

	template<typename T>
	bool getData(Phenix::String& col_name, const T& val);

	inline void firstRow();
	inline bool nextRow();

private:
	void addRow(const MYSQL_ROW& mysql_row, unsigned long* lengths);

private:
	std::map<Phenix::String, Field>	_fields;	
	std::vector<Row>				_rows;	
	Phenix::Int32					_cur_row_idx;	// ��ǰ�����	
};

template<typename T>
bool Phenix::MySql::ResultSet::getData( Phenix::String& col_name, const T& val )
{
	if (_rows.empty())
	{
		return false;
	}
	std::map<Phenix::String, Field>::iterator iter = _fields.find(col_name);
	if (iter == _fields.end())
	{
		return false;
	}	
	Data& data = _rows[iter->second.idx];
	switch (iter->second.data_type)
	{
	case MYSQL_TYPE_TINY:		
	case MYSQL_TYPE_SHORT:		
	case MYSQL_TYPE_LONG:		
	case MYSQL_TYPE_LONGLONG:
		val = atoi(data.head);
		break;
	case MYSQL_TYPE_FLOAT:		
	case MYSQL_TYPE_DOUBLE:
		val = atof(data.head);
		break;
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
		if (__is_class(T) && __alignof(T) != 1)
		{
			return false;
		}
		memcpy(&val, data.head, data.len);
		break;
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_TIMESTAMP:
		//return sizeof(MYSQL_TIME); ������
	case MYSQL_TYPE_BLOB:
		//return 32768;	// return for buffer limit  ������
	default:
		return false;
	}
}


} // end namespace MySql
} // end namespace Phenix

#endif