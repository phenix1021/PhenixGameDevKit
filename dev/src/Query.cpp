#include "stdafx.h"
#include <iostream>
#include <Phenix/MySql/Query.h>
#include <Phenix/MySql/Connection.h>
#include <Phenix/MySql/RecordSet.h>

namespace Phenix
{
namespace MySql
{
	
Query::Query( Connection* conn )
	:_conn(conn)
{

}

Query::~Query()
{

}

} // end namespace  MySql
} // end namespace Phenix

