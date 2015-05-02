/*******************************************************************************
 * @brief	基础数据类型
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_TYPES_H
#define PHENIX_TYPES_H

#include <string>

namespace Phenix
{

typedef signed char				Int8;
typedef unsigned char			UInt8;
typedef signed short			Int16;
typedef unsigned short			UInt16;
typedef signed int				Int32;
typedef unsigned int			UInt32;
typedef signed __int64			Int64;
typedef unsigned __int64		UInt64;
typedef std::string				String;

#define interface	class

} // end namespace Phenix

#endif