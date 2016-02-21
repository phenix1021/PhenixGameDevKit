/*******************************************************************************
 * @brief   结果集
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_RECORDSET_H
#define PHENIX_MYSQL_RECORDSET_H

#include <mysql.h>

namespace Phenix
{
namespace MySql
{

struct QueryResult
{
	Phenix::UInt32 affected_rows;
	QueryResult();	
};

class RecordSetBase
{
public:
	RecordSetBase();
	virtual ~RecordSetBase();

	void* getBuffer() const = 0;
};

template<typename T>
class RecordSet
	:public RecordSetBase
{
public:
	RecordSet();
	virtual ~RecordSet();

	void* getBuffer() const {return buffer;}

private:
	T* buffer;	// 数组首地址
};


} // end namespace MySql
} // end namespace Phenix

#endif