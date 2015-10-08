/*******************************************************************************
 * @brief	哈希表（线性哈希）
 * @date	2015-10-5
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_HASHTABLE_H
#define PHENIX_COLLECTION_HASHTABLE_H

#include <Phenix/Base/Hash.h>

namespace Phenix
{
namespace Collection
{

	template<typename Value, typename HashFunc = HashCode<Value>>
	class HashTable
	{		
		typedef std::vector<Value> Bucket;
		typedef typename Bucket::iterator BucketIterator;	// 注意要加typename,否则编译器无法知道iterator	
		typedef std::vector<Bucket> BucketVec;
		typedef typename BucketVec::iterator BucketVecIterator;		// 注意要加typename,否则编译器无法知道iterator	

	public:
		class ConstIterator
			:public std::iterator<std::forward_iterator_tag, Value>
		{
		public:
			ConstIterator(){}
			ConstIterator(const ConstIterator& o)
				:_vecIt(o._vecIt), _endIt(o._endIt), _bucketIt(o._bucketIt){}
			ConstIterator(const BucketVecIterator& vecIt, const BucketVecIterator& endIt,
				const BucketIterator bucketIt)
				:_vecIt(vecIt), _endIt(endIt), _bucketIt(bucketIt){}
			virtual ~ConstIterator(){}

			inline const typename Value& operator * () const
			// 也可以写成inline const typename Bucket::value_type& operator * () const
			{
				return *_bucketIt;
			}

			inline const typename Value* operator -> () const
			// 也可以写成inline const typename Bucket::value_type* operator * () const
			{
				return &*_bucketIt;
			}

			inline bool operator == (const ConstIterator& o) const	// 是否指向同一位置
			{
				return _vecIt == o._vecIt && (_vecIt == _endIt || _bucketIt == o._bucketIt);
			}

			inline bool operator != (const ConstIterator& o) const	// 是否指向不同位置
			{
				return !(this == o);
			}

			inline ConstIterator& swap(const ConstIterator& o)
			{
				using std::swap;
				swap(_vecIt, o._vecIt);
				swap(_endIt, o._endIt);
				swap(_bucketIt, o._bucketIt);
				return *this;
			}

			inline ConstIterator& assign(const ConstIterator& o)
			{				
				_vecIt = o._vecIt;
				_endIt = o._endIt;
				_bucketIt = o._bucketIt;
				return *this;
			}

			inline ConstIterator& operator = (const ConstIterator& o)
			{
				return assign(o);
			}

			inline ConstIterator& operator ++ () // ++n
			{				
				if (_vecIt != _endIt)
				{					
					++_bucketIt;
					while (_bucketIt == _vecIt->end())
					{
						++_vecIt;
						if (_vecIt != _endIt)
							_bucketIt = _vecIt->begin();
					}
				}
				return *this;
			}

			inline ConstIterator operator ++ (Phenix::Int32) // n++
			{
				ConstIterator tmp(*this);
				++*this;
				return tmp;
			}

		protected:
			BucketVecIterator	_vecIt;		// 当前BucketVec位置
			BucketVecIterator	_endIt;		// 结尾BucketVec位置(指向空数据)
			BucketIterator		_bucketIt;	// 当前Bucket位置			
		};

		class Iterator
			:public ConstIterator
		{
		public:
			Iterator(){}
			Iterator(const Iterator& o)
				:_vecIt(o._vecIt), _endIt(o._endIt), _bucketIt(o._bucketIt){}
			Iterator(const BucketVecIterator& vecIt, const BucketVecIterator& endIt,
				const BucketIterator bucketIt)
				:_vecIt(vecIt), _endIt(endIt), _bucketIt(bucketIt){}
			virtual ~Iterator(){}

			inline typename Value& operator * () 
			// 也可以写成inline typename Bucket::value_type& operator * ()
			{
				return *_bucketIt;
			}

			inline const typename Value& operator * () const
				// 也可以写成inline typename Bucket::value_type& operator * ()
			{
				return *_bucketIt;
			}

			inline typename Value* operator -> ()
			// 也可以写成inline const typename Bucket::value_type* operator * () const
			{
				return &*_bucketIt;
			}

			inline const typename Value* operator -> () const
				// 也可以写成inline const typename Bucket::value_type* operator * () const
			{
				return &*_bucketIt;
			}

			inline Iterator& swap(const Iterator& o)
			{
				ConstIterator::swap(o);
				return *this;
			}

			inline Iterator& assign(const Iterator& o)
			{				
				ConstIterator::assign(o);
				return *this;
			}

			inline Iterator& operator = (const Iterator& o)
			{
				return assign(o);
			}

			inline Iterator& operator ++ () // ++n
			{				
				ConstIterator::operator ++ ();
				return *this;
			}

			inline Iterator operator ++ (Phenix::Int32) // n++
			{
				Iterator tmp(*this);
				++*this;
				return tmp;
			}
		};

	public:
		HashTable(){}
		HashTable(const HashTable& o)
		{

		}

		void swap(const HashTable& o)
		{

		}

		HashTable& assign(const HashTable& o)
		{

		}

		HashTable& operator = (const HashTable& o)
		{
			return assign(o);
		}

		ConstIterator	find(const Value& value) const
		{

		}
		
		Iterator		find(const Value& value)
		{

		}

		ConstIterator	begin() const
		{

		}

		Iterator	begin()
		{

		}

		ConstIterator	end() const
		{

		}

		Iterator	end()
		{

		}

	private:
		void slice()
		{

		}

	private:		
		std::size_t	_sliceIdx;	// 当前准备分裂的_buckets下标
		std::size_t	_front;		// 模数		
		std::size_t	_size;		// 元素个数
		
		BucketVec	_buckets;
		HashFunc	_hash;
	};

	

}
}


#endif