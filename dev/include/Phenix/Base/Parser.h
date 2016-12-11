/*******************************************************************************
 * @brief	数据类型转换
 * @date	2015-4-4
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_PARSER_H
#define PHENIX_BASE_PARSER_H

#include <sstream>
#include <Phenix/Base/Exception.h>

namespace Phenix
{

class Parser
{	
public:
	template<typename T>
	static Phenix::String ToString(const T& arg)
	{
		if (__is_class(T))
		{
			Phenix::UInt32 size = sizeof(T);
			if (size > STR_BUFFER_SIZE)
			{
				throw OutOfMemoryException();
			}
			memset(buffer, 0, STR_BUFFER_SIZE);
			memcpy(buffer, &arg, size);
			return Phenix::String(buffer);
		}
		std::ostringstream sm;
		sm << arg;
		return sm.str();		
	}

private:
	static char buffer[STR_BUFFER_SIZE];
};

} // end namespace Phenix

#endif