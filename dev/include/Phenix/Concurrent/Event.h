/*******************************************************************************
 * @brief	事件对象
 * @date	2015-6-22
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_EVENT_H
#define PHENIX_CONCURRENT_EVENT_H

#include <Windows.h>
#include <Phenix/Base/Noncopyable.h>

namespace Phenix
{
namespace Concurrent
{

using Phenix::Noncopyable;

class Event
	:private Noncopyable
{
public:
	Event(bool auto_reset);
	virtual ~Event();

	void set();
	void reset();
	void wait();
	bool wait(long milliseconds);

private:
	HANDLE	_hnd;	
};

}
}


#endif