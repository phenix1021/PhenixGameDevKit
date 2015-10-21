#include "stdafx.h"
#include <Windows.h>
#include <Phenix/Concurrent/AtomCounter.h>

namespace Phenix
{
namespace Concurrent
{	

AtomCounter::AtomCounter()
{
	_val = 0;
}

AtomCounter::AtomCounter( Phenix::Int32 val )
{
	_val = val;
}

AtomCounter::~AtomCounter()
{

}

AtomCounter& AtomCounter::operator=( Phenix::Int32 val )
{
	_val = val;
	return *this;
}

Phenix::Int32 AtomCounter::value()
{
	return Phenix::Int32(_val);
}

Phenix::Int32 AtomCounter::inc()
{
	return add(1);
}

Phenix::Int32 AtomCounter::dec()
{
	return add(-1);
}

Phenix::Int32 AtomCounter::add( Phenix::Int32 delta )
{
	InterlockedExchangeAdd(&_val, delta);
	return value();
}

}
}