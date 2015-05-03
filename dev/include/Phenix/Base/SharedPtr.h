/*******************************************************************************
* @brief	����ָ��
 * @date	2015-5-1
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_SHARED_PTR_H
#define PHENIX_BASE_SHARED_PTR_H

#include <windows.h>

namespace Phenix
{
namespace Base
{
	template<typename T>
	class SharedPtr
	{
	public:
		SharedPtr():m_obj(NULL), m_ref_count(new volatile LONG(0)){}		
		SharedPtr(T* obj):m_obj(obj), m_ref_count(new volatile LONG(1)){}		

		SharedPtr(SharedPtr& other)
		{
			m_obj = other.m_obj;
			m_ref_count = other.m_ref_count;
			InterlockedIncrement(m_ref_count);
		}

		virtual ~SharedPtr()
		{
			if (!(InterlockedDecrement(m_ref_count)))
			{
				Release();
			}			
		}

		bool IsNull()
		{
			return !m_obj;
		}

		const T* GetPtr() const
		{
			return m_obj;
		}
		T* GetPtr()
		{
			return m_obj;
		}

		Phenix::Int32 GetRefCount()
		{
			return *m_ref_count;
		}

		SharedPtr& operator = (SharedPtr& other)
		{
			if (IsNull())
			{
				m_obj = other.m_obj;
				m_ref_count = other.m_ref_count;
				InterlockedIncrement(m_ref_count);
			} 
			else
			{
				if (!InterlockedDecrement(m_ref_count))
				{
					Release();
				}				
				InterlockedIncrement(other.m_ref_count);
				m_obj = other.m_obj;
				m_ref_count = other.m_ref_count;
			}			
			
			return *this;
		}

		T* operator -> ()
		{
			return m_obj;
		}
		const T* operator -> () const
		{
			return m_obj;
		}

		T& operator * ()
		{
			return *m_obj;
		}
		const T& operator * () const
		{
			return *m_obj;
		}

		bool operator == (const SharedPtr& other) const
		{
			return m_obj == other->m_obj;
		}
		bool operator == (SharedPtr& other) const
		{
			return m_obj == other->m_obj;
		}
		bool operator == (const T* other) const
		{
			return m_obj == other;
		}
		bool operator == (T* other) const
		{
			return m_obj == other;
		}

		bool operator != (const SharedPtr& other) const
		{
			return m_obj != other->m_obj;
		}
		bool operator != (SharedPtr& other) const
		{
			return m_obj != other->m_obj;
		}
		bool operator != (const T* other) const
		{
			return m_obj != other;
		}
		bool operator != (T* other) const
		{
			return m_obj != other;
		}

		bool operator < (const SharedPtr& other) const
		{
			return m_obj < other->m_obj;
		}
		bool operator < (SharedPtr& other) const
		{
			return m_obj < other->m_obj;
		}
		bool operator < (const T* other) const
		{
			return m_obj < other;
		}
		bool operator < (T* other) const
		{
			return m_obj < other;
		}

		bool operator <= (const SharedPtr& other) const
		{
			return m_obj <= other->m_obj;
		}
		bool operator <= (SharedPtr& other) const
		{
			return m_obj <= other->m_obj;
		}
		bool operator <= (const T* other) const
		{
			return m_obj <= other;
		}
		bool operator <= (T* other) const
		{
			return m_obj <= other;
		}

		bool operator > (const SharedPtr& other) const
		{
			return m_obj > other->m_obj;
		}
		bool operator > (SharedPtr& other) const
		{
			return m_obj > other->m_obj;
		}
		bool operator > (const T* other) const
		{
			return m_obj > other;
		}
		bool operator > (T* other) const
		{
			return m_obj > other;
		}

		bool operator >= (const SharedPtr& other) const
		{
			return m_obj >= other->m_obj;
		}
		bool operator >= (SharedPtr& other) const
		{
			return m_obj >= other->m_obj;
		}
		bool operator >= (const T* other) const
		{
			return m_obj >= other;
		}
		bool operator >= (T* other) const
		{
			return m_obj >= other;
		}
	private:
		void Release()
		{
			delete m_obj;
			m_obj = 0;
			delete m_ref_count;
			m_ref_count = 0;
		}
	private:
		volatile LONG*	m_ref_count;
		T*				m_obj;
	};

} // end namespace Base
} // end namespace Phenix


#endif