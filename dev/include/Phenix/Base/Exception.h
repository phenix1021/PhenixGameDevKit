/*******************************************************************************
 * @brief	“Ï≥£π‹¿Ì
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_TYPES_H
#define PHENIX_TYPES_H

#include <string>

namespace Phenix
{

class Exception
	:public std::exception
{
public:
	Exception(){}
	Exception(const Phenix::String& msg):_msg(msg){}

	const char* what() const throw()
	{
		return "Exception";
	}

	void rethrow() const
	{
		throw *this;
	}

	virtual Phenix::String displayText() const
	{
		Phenix::String txt(what());
		if (!_msg.empty())
		{
			txt.append(": ");
			txt.append(_msg);
		}
		return txt;
	}

private:
	Phenix::String _msg;
};

#define DERIVE_EXCEPTION(DERIVE_CLASS, BASE_CLASS, WHAT)		\
class DERIVE_CLASS												\
	:public BASE_CLASS											\
{																\
public:															\
	DERIVE_CLASS(){}											\
	DERIVE_CLASS(const Phenix::String& msg):BASE_CLASS(msg){}	\											
	const char* what() const throw()							\
	{															\
		return WHAT;											\
	}															\
}

DERIVE_EXCEPTION(LogicException, Exception, "Logic exception")
DERIVE_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
DERIVE_EXCEPTION(NullPointerException, LogicException, "Null pointer")
DERIVE_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
DERIVE_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
DERIVE_EXCEPTION(RangeException, LogicException, "Out of range")
DERIVE_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
DERIVE_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
DERIVE_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

DERIVE_EXCEPTION(RuntimeException, Exception, "Runtime exception")
DERIVE_EXCEPTION(NotFoundException, RuntimeException, "Not found")
DERIVE_EXCEPTION(ExistsException, RuntimeException, "Exists")
DERIVE_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
DERIVE_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
DERIVE_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")


} // end namespace Phenix

#endif