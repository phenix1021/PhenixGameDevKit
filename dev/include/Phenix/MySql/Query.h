/*******************************************************************************
 * @brief   query”Ôæ‰
 * @date	2016-2-6
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MYSQL_QUERY_H
#define PHENIX_MYSQL_QUERY_H

#include <mysql.h>

namespace Phenix
{
namespace MySql
{

class Connection;
class RecordSet;

class Query
{
public:
	Query(Connection* conn);
	virtual ~Query();

	RecordSet* select(){}
	void execute(){}

private:
	Connection* _conn;
};


} // end namespace MySql
} // end namespace Phenix

#endif