/*******************************************************************************
 * @brief	固定大小内存池
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MEMORY_OBJECTPOOL_H
#define PHENIX_MEMORY_OBJECTPOOL_H

#include <algorithm>

namespace Phenix
{
namespace Memory
{

template<typename T, size_t max_obj_count = 0, bool is_single_thread = true>
class ObjectPool
{
	typedef T ObjType;

	enum
	{
		BLOCK_SIZE = 4096,
	};

	struct Chunk
	{
		ObjType	obj;		
	};

public:	
	explicit ObjectPool()
		:m_alloced_obj_count(0)
	{
		m_aligned_chunk_size = ((sizeof(Chunk)+sizeof(void*)-1) >> sizeof(void*)) << sizeof(void*);
		if (m_aligned_chunk_size > BLOCK_SIZE || !Expand())
		{
			throw;
		}		
	}

	virtual ~ObjectPool()
	{
		ReleaseAll();
	}

	T* Create()
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T();
		}
		catch (...)
		{
			Release(reinterpret_cast<T*>(ptr));
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename ARG>
	T* Create(const ARG& arg)
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T(arg);
		}
		catch (...)
		{
			Release(ptr);
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename ARG1, typename ARG2>
	T* Create(const ARG1& arg1, const ARG2& arg2)
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T(arg1, arg2);
		}
		catch (...)
		{
			Release(ptr);
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename ARG1, typename ARG2, typename ARG3>
	T* Create(const ARG1& arg1, const ARG2& arg2, const ARG3& arg3)
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T(arg1, arg2, arg3);
		}
		catch (...)
		{
			Release(reinterpret_cast<T*>(ptr));
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename ARG1, typename ARG2, typename ARG3, typename ARG4>
	T* Create(const ARG1& arg1, const ARG2& arg2, const ARG3& arg3, const ARG4& arg4)
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T(arg1, arg2, arg3, arg4);
		}
		catch (...)
		{
			Release(reinterpret_cast<T*>(ptr));
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
	T* Create(const ARG1& arg1, const ARG2& arg2, const ARG3& arg3, const ARG4& arg4, const ARG5& arg5)
	{
		void* ptr = Alloc();
		if (!ptr)
		{
			return NULL;
		}
		try
		{
			new (ptr) T(arg1, arg2, arg3, arg4, arg5);
		}
		catch (...)
		{
			Release(reinterpret_cast<T*>(ptr));
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	void Release(void* ptr)
	{
		((T*)ptr)->~T();
		m_free_chunks.push_back(ptr);		
	}

private:
	void* Alloc()
	{
		void* ptr = NULL;
		if (!m_free_chunks.empty())
		{
			ptr = m_free_chunks[0];
		}
		return ptr;
	}

	bool Expand()
	{
		if (max_obj_count && max_obj_count <= m_alloced_obj_count)
		{
			return false;
		}
		void* ptr = malloc(BLOCK_SIZE);
		m_blocks.push_back(ptr);
		char* head = reinterpret_cast<char*>(ptr);
		char* tail = head + BLOCK_SIZE - 1;		
		do
		{
			m_free_chunks.push_back(head);
			head += m_aligned_chunk_size;
		}while (head <= tail && ++m_alloced_obj_count < max_obj_count);

		return true;
	}

	void ReleaseAll()
	{
		std::for_each(m_blocks.begin(), m_blocks.end(), &free);
	}

private:
	size_t m_alloced_obj_count;
	size_t m_aligned_chunk_size;

	std::vector<void*> m_free_chunks;
	std::vector<void*> m_blocks;
};

} // end namespace Memory
} // end namespace Phenix

#endif