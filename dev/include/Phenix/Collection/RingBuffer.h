/*******************************************************************************
 * @brief	环形缓冲区
 * @date	2015-9-8
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_RING_BUFFER_H
#define PHENIX_CONCURRENT_RING_BUFFER_H

#include <Phenix/Memory/ObjectPool.h>

using namespace Phenix::Memory;

namespace Phenix
{
namespace Collection
{
	template<typename T>
	class RingBuffer
	{
		enum 
		{
			EVERY_BUFFER_SIZE	= 1<<6,					// 每个环的元素个数
			MOD_NUM				= EVERY_BUFFER_SIZE-1	// 和其进行“&”操作即为取模
		};
		
		class Buffer
		{
		public:
			Buffer():_pushIdx(0),_popIdx(0){}
			virtual ~Buffer(){}

			inline bool isFull(){return isMeet() && _pushIdx > _popIdx;}
			inline bool isEmpty(){return _pushIdx == _popIdx;}
			
			inline bool push(T data)
			{
				if (isFull())
				{
					return false;
				}
				_datum[_pushIdx & MOD_NUM] = data;
				++_pushIdx;
				return true;
			}

			inline bool pop(T& data)
			{
				if (isEmpty())
				{
					return false;
				}
				data = _datum[_popIdx & MOD_NUM];
				++_popIdx;
				return true;
			}

		private:
			inline bool isMeet(){return (_pushIdx & MOD_NUM) == (_popIdx & MOD_NUM);}

		private:
			Phenix::Int32 _pushIdx;
			Phenix::Int32 _popIdx;
			T _datum[EVERY_BUFFER_SIZE];
		};

	public:
		RingBuffer();		
		virtual ~RingBuffer();

		bool push(T data);
		bool pop(T& data);
		void collect();		// 回收_buffers中空的buffer

	private:
		std::list<Buffer*> _buffers;
		ObjectPool<T> _pool;
	};

	template<typename T>
	Phenix::Collection::RingBuffer<T>::RingBuffer()
	{
		_buffers.insert(_pool.Create());
	}

	template<typename T>
	Phenix::Collection::RingBuffer<T>::~RingBuffer()
	{

	}

	template<typename T>
	bool Phenix::Collection::RingBuffer<T>::push( T data )
	{
		std::list<Buffer*>::reverse_iterator iter = _buffers.begin();
		if (iter->isFull())
		{
			_buffers.insert(_pool.Create());
		}
	}

	template<typename T>
	bool Phenix::Collection::RingBuffer<T>::pop( T& data )
	{
		if (_buffers.empty())
		{
			return false;
		}
	}


}
}


#endif