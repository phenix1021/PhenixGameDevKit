/*******************************************************************************
 * @brief	semaphore∂‘œÛ
 * @date	2015-6-22
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_SEMAPHORE_H
#define PHENIX_CONCURRENT_SEMAPHORE_H

#include <Windows.h>
#include <Phenix/Base/Noncopyable.h>

namespace Phenix
{
namespace Concurrent
{

using Phenix::Noncopyable;

class Semaphore
	:private Noncopyable
{
public:
	Semaphore(Phenix::Int32 init, Phenix::Int32 max);
	virtual ~Semaphore();

	void set();	
	void wait();
	bool wait(long milliseconds);

private:
	HANDLE	_hnd;	
};

}
}


#endif