#ifndef PHENIX_BASE_NONCOPYABLE_H
#define PHENIX_BASE_NONCOPYABLE_H

namespace Phenix
{
namespace Base
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

} // end namespace Base
} // end namespace Phenix

#endif