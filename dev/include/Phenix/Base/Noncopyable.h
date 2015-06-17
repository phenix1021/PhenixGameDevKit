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
protected:
	Noncopyable(){}
	~Noncopyable(){}
private:
	const Noncopyable& operator = (const Noncopyable& other);
	Noncopyable(const Noncopyable& other);
};


} // end namespace Phenix

#endif