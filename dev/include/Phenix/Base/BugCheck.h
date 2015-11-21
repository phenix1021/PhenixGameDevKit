/*******************************************************************************
 * @brief	ต๗สิ
 * @date	2015-11-2
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BUGCHECK_H
#define PHENIX_BUGCHECK_H

struct _EXCEPTION_POINTERS;

namespace Phenix
{

class BugCheck
{
public:
	static void assertion(const Phenix::String& cond);
	static void nullPoionter(const Phenix::String& ptr);
	static long __stdcall CreateDumpFile(const char* dumpFileName, _EXCEPTION_POINTERS* ep);

private:
	static void invokeDebugger();
};


} // end namespace Phenix


#define BEGIN_SEH					__try{
#define END_SEH(dumpFileName)		}__except(Phenix::BugCheck::CreateDumpFile(dumpFileName, GetExceptionInformation())){}

#define PHENIX_ASSERT(cond)		\
	if (!cond) Phenix::BugCheck::assertion(#cond);		

#define PHENIX_CHECK_PTR(ptr)	\
	if (!ptr) Phenix::BugCheck::nullPoionter(#ptr);

#endif