/*******************************************************************************
 * @brief	½ûÖ¹¿½±´
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_NONCOPYABLE_H
#define PHENIX_BASE_NONCOPYABLE_H

namespace Phenix
{

class Noncopyable
{
public:
	Noncopyable() = default;
	Noncopyable& operator = (const Noncopyable& other) = delete;
	Noncopyable(const Noncopyable& other) = delete;
};


} // end namespace Phenix

#endif