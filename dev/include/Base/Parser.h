/*******************************************************************************
 * @brief	数据类型转换
 * @date	2015-4-4
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_PARSER_H
#define PHENIX_BASE_PARSER_H

#include <sstream>

namespace Phenix
{
namespace Base
{
	class Parser
	{
		template<typename T>
		inline static Phenix::String ToString(const T& arg);
		{
			ostringstream buffer;
			buffer << arg;
			return buffer.str();		
		}
	};

} // end namespace Base
} // end namespace Phenix

#endif