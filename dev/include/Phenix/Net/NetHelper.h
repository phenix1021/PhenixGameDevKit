/*******************************************************************************
 * @brief	���縨������
 * @date	2015-12-9
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_NET_HELPER_H
#define PHENIX_NET_HELPER_H

#include <Windows.h>

namespace Phenix
{
namespace Net
{

class Helper
{
public:
	// ��ȡ��������IP
	static bool getIPs(std::vector<Phenix::String>& IPs);
};

} // end namespace Net
} // end namespace Phenix

#endif