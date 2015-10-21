#include "stdafx.h"
#include <Phenix/base/Hash.h>

namespace Phenix
{
std::size_t HashCode::hashCode( const Phenix::Int8 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::UInt8 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::Int16 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::UInt16 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::Int32 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::UInt32 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::Int64 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::UInt64 n )
{
	return static_cast<std::size_t>(n)*2654435761U; 
}

std::size_t HashCode::hashCode( const Phenix::String& str )
{
	std::size_t h = 0;
	Phenix::String::const_iterator it  = str.begin();
	Phenix::String::const_iterator end = str.end();
	while (it != end)
	{
		h = h * 0xf4243 ^ *it++;
	}
	return h;
}

}