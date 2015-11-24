/*******************************************************************************
 * @brief	IOCP∂‘œÛ
 * @date	2015-11-24
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_NET_IOCPOBJ_H
#define PHENIX_NET_IOCPOBJ_H

#include <Windows.h>

namespace Phenix
{
namespace Net
{

class IOCPObj
{
public:
	IOCPObj():_hnd(NULL){}
	virtual ~IOCPObj();

	bool Create(Phenix::UInt8 numberOfConcurrentThreads);
	bool Bind(HANDLE olObjHnd, void* olObjParam);
	bool CreateAndBind(Phenix::UInt8 numberOfConcurrentThreads, HANDLE olObjHnd, void* olObjParam);
	bool GetIOStatus();
	bool SetIOStatus();

private:
	HANDLE _hnd;
};

} // end namespace Net
} // end namespace Phenix

#endif