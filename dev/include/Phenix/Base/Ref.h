/*******************************************************************************
* @brief	·Âboost::ref,boost::cref
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_REF_H
#define PHENIX_BASE_REF_H

namespace Phenix
{

template<typename T>
class ReferenceWrapper
{
public:
	explicit ReferenceWrapper(T& t):m_t(&t){}
	operator T& () const {return *m_t;}
private:
	T* m_t;	
};

template<typename T> inline ReferenceWrapper<T> Ref(T& t)
{
	return ReferenceWrapper<T>(t);
}

template<typename T> inline ReferenceWrapper<const T> CRef(const T& t)
{
	return ReferenceWrapper<const T>(t);
}

} // end namespace Phenix

#endif