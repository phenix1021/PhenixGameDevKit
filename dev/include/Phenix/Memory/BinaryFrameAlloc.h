/*******************************************************************************
 * @brief	Ë«¶ËÖ¡ÄÚ´æ·ÖÅäÆ÷
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_MEMORY_BINARY_FRAME_ALLOC_H
#define PHENIX_MEMORY_BINARY_FRAME_ALLOC_H

namespace Phenix
{
namespace Memory
{

template<size_t block_size, bool is_single_thread = true>
class BinaryFrameAlloc
{
public:
	enum FrameType
	{
		INVALID		= 0,
		FRAME_HIGH	= 1,
		FRAME_LOW	= 2		
	};

	struct Frame
	{
		Phenix::UInt16	frame_type;
		void*			frame_addr;
		Frame(){memset(this, 0, sizeof(Frame));}
	};

public:
	BinaryFrameAlloc();
	virtual ~BinaryFrameAlloc(){ReleaseAll();}

public:	
	Frame GetFrame(Phenix::UInt16 frame_type);
	void ReleaseFrame(const Frame& frame);

public:
	template<typename T>
	T* Create(Phenix::UInt16 frame_type)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename T, typename ARG>
	T* Create(Phenix::UInt16 frame_type, const ARG& arg)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename T, typename ARG1, typename ARG2>
	T* Create(Phenix::UInt16 frame_type, const ARG& arg1, const ARG& arg2)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename T, typename ARG1, typename ARG2, typename ARG3>
	T* Create(Phenix::UInt16 frame_type, const ARG& arg1, const ARG& arg2, const ARG& arg3)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename T, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
	T* Create(Phenix::UInt16 frame_type, const ARG& arg1, const ARG& arg2, const ARG& arg3, const ARG& arg4)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

	template<typename T, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
	T* Create(Phenix::UInt16 frame_type, const ARG& arg1, const ARG& arg2, const ARG& arg3, const ARG& arg4, const ARG& arg5)
	{
		void* ptr = Alloc(frame_type, sizeof(T));
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
			throw;
		}
		return reinterpret_cast<T*>(ptr);
	}

private:
	void* Alloc(Phenix::UInt16 frame_type, size_t size);	

	void ReleaseAll();
	bool CheckRelease(const Frame& frame);

	inline void* AlignAddr(void* addr, bool up);

private:
	std::vector<void*> m_overflow_blocks;
	char* m_block_head;
	char* m_high_end;
	char* m_low_end;
};

template<size_t block_size, bool is_single_thread = true>
void* Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::AlignAddr( void* addr, bool up )
{
	return (addr+ (up ? sizeof(void*)-1 : 0)) >> sizeof(void*) << sizeof(void*);
}

template<size_t block_size, bool is_single_thread = true>
bool Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::CheckRelease( const Frame& frame )
{
	if (FrameType::FRAME_HIGH == frame.frame_type && frame.frame_addr >= m_high_end && frame.frame_addr <= m_block_head+block_size)
	{
		return true;
	}
	if (FrameType::FRAME_LOW == frame.frame_type && frame.frame_addr <= m_low_end && frame.frame_addr >= m_block_head)
	{
		return true;
	}
	return false;
}

template<size_t block_size, bool is_single_thread = true>
void Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::ReleaseAll()
{
	free(m_block_head);
	std::for_each(m_overflow_blocks.begin(), m_overflow_blocks.end(), &free);
}

template<size_t block_size, bool is_single_thread = true>
void* Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::Alloc( Phenix::UInt16 frame_type, size_t size )
{
	char* ptr = NULL;
	if (FrameType::FRAME_HIGH == frame_type)
	{
		ptr = AlignAddr(m_high_end+size, false);
		if (m_low_end >= ptr)
		{
			// todo: log
			ptr = malloc(size);
			m_overflow_blocks.push_back(ptr);
			return ptr;
		}
		m_high_end = ptr;
	} 

	if (FrameType::FRAME_LOW == frame_type)
	{
		ptr = AlignAddr(m_low_end, true);
		if (m_high_end <= ptr+size)
		{
			// todo: log
			ptr = malloc(size);
			m_overflow_blocks.push_back(ptr);
			return ptr;
		}
		m_low_end = ptr+size;
	}
	return ptr;
}

template<size_t block_size, bool is_single_thread = true>
void Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::ReleaseFrame( const Frame& frame )
{
	if (CheckRelease(frame))
	{
		if (FrameType::FRAME_HIGH == frame.frame_type)
		{
			m_high_end = reinterpret_cast<char*>(frame.frame_addr);
		}
		else
		{
			m_low_end = reinterpret_cast<char*>(frame.frame_addr);
		}
	}
}

template<size_t block_size, bool is_single_thread = true>
Frame Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::GetFrame( Phenix::UInt16 frame_type )
{
	Frame frame;
	frame.frame_type = frame_type;
	frame.frame_addr = (FrameType::FRAME_HIGH == frame.frame_type ? m_high_end : m_low_end);
	return frame;
}

template<size_t block_size, bool is_single_thread = true>
Phenix::Memory::BinaryFrameAlloc<block_size, is_single_thread>::BinaryFrameAlloc()
{
	m_block_head = reinterpret_cast<char*>(malloc(block_size));
	m_low_end = reinterpret_cast<char*>(m_block_head);
	m_high_end = m_low_end+block_size;

}

} // end namespace Memory
} // end namespace Phenix

#endif