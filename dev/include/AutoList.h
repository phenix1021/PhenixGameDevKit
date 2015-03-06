#ifndef PHENIX_BASE_AUTOLIST_H
#define PHENIX_BASE_AUTOLIST_H

namespace Phenix
{
namespace Base
{

template<typename T>
class AutoList
{
public:
	AutoList()
	{
		m_objs.push_back(static_cast<T*>(this));
	}

	virtual ~AutoList()
	{
		T* obj = static_cast<T*>(this);
		std::list<const T*>::const_iterator c_it = m_objs.begin();
		for (; c_it != m_objs.end(); ++c_it)
		{
			if (*c_it == obj)
			{
				m_objs.erase(c_it);
				break;
			}
		}	
	}

	static const std::list<const T*>& GetObjs(){return m_objs;}

private:
	static std::list<const T*> m_objs;
};

template<typename T> std::list<const T*> AutoList<T>::m_objs;

} // end namespace Base
} // end namespace Phenix

#endif