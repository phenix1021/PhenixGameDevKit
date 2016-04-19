/*******************************************************************************
 * @brief   ½á¹û¼¯
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_RECORDSET_H
#define PHENIX_MYSQL_RECORDSET_H

#include <mysql.h>
#include <Phenix/Memory/ObjectPool.h>
#include <Phenix/Base/SharedPtr.h>

namespace Phenix
{
namespace MySql
{

class RecordSet
{	
	friend class Query;
public:
	struct Row
	{
		void*			head;		
		
		Row(void* ptr);
		~Row();

		void operator delete (void* ptr);
	};

	static Phenix::Memory::ObjectPool<Row>	row_pool;
	//static MemoryPool<Row>	record_pool;

public:
	RecordSet(Phenix::UInt32 row_length):_row_idx(0), _row_length(row_length){}
	virtual ~RecordSet(){}

	void addRow(void* ptr);

	template<typename T>
	bool getData(T*& ptr);
	void first();
	void next();
	bool isEOF();

	static Phenix::UInt32 getColLength(MYSQL_FIELD& field);

private:
	void* allocRow();

private:
	std::vector<Phenix::SharedPtr<Row>> _rows;
	Phenix::Int32	_row_idx;
	Phenix::UInt32	_row_length;
};

template<typename T>
bool Phenix::MySql::RecordSet::getData( T*& ptr )
{
	if (_row_length != sizeof(T))
	{
		return false;
	}
	if (_rows.empty())
	{
		return false;
	}	
	if (_row_idx >= _rows.size())
	{
		return false;
	}
	ptr = (T*)_rows[_row_idx]->head;
	return true;
}


} // end namespace MySql
} // end namespace Phenix

#endif