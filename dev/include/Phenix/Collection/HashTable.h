/*******************************************************************************
 * @brief	哈希表（线性哈希）
 * @date	2015-10-5
 * @author	phenix
 * @mark    
			原理：
			1. hash(x) % _front == bucket_idx
			   hash(x) % (_front*2) == bucket_idx 或者 bucket_idx + _front
		    2. slice_idx 从0开始，表示当前要分裂的bucket，对该bucket进行hash(x) % (_front*2)，
			   重新归置bucket内各项。
		    3. 初始bucket数量==_front，每次slice时++_front，同时pushback一个新的bucket。
		 	4. slice触发时机可以根据实际情况，这里用的是每次insert数据都slice一次。
			5. 当slice_idx == _front时，说明桶里本轮所有bucket已经slice过一遍。
			   这时slice_idx = 0,_front *= 2。然后开始新一轮slice。
			6. 每次erase数据都触发一次merge，处理逻辑和slice相反。

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
		friend class HashTable;
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
				while (_vecIt != _endIt && _bucketIt == _vecIt->end())
				{						
					if (++_vecIt != _endIt)
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
		friend class HashTable;
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
	HashTable(std::size_t bucketReserveSize = 64)
		:_front(1),
		_sliceIdx(0),
		_size(0)
	{
		_buckets.reserve(calcInitReserveBucketSize(bucketReserveSize));
		_buckets.push_back(Bucket());
	}
	HashTable(const HashTable& o)
		:_front(o._front),
		_sliceIdx(o._sliceIdx),
		_size(o._size),
		_buckets(o._buckets)
	{

	}

	static std::size_t calcInitReserveBucketSize(std::size_t initialSize)
	{
		std::size_t minInitReserveBucketsSize = 32;
		while (minInitReserveBucketsSize < initialSize) 
			minInitReserveBucketsSize <<= 1;
		return minInitReserveBucketsSize;
	}

	void swap(const HashTable& o)
	{
		using std::swap;
		swap(_buckets, o._buckets);
		swap(_size, o._size);
		swap(_front, o._front);
		swap(_sliceIdx, o._sliceIdx);
	}

	void clear()
	{			
		HashTable tmp;
		swap(tmp);	
	}

	void assign(const HashTable& o)
	{
		_buckets = o._buckets;
		_size = o._size;
		_front = o._front;
		_sliceIdx = o._sliceIdx;
	}

	HashTable& operator = (const HashTable& o)
	{
		assign(o);
		return *this;
	}

	ConstIterator	find(const Value& value) const
	{
		std::size_t hashAddr = _hash(value);
		BucketVecIterator vecIt(_buckets.begin() + hashAddr);
		assert(vecIt != _buckets.end());			
		BucketIterator bucketIt(std::find(vecIt->begin(), vecIt->end(), value));
		if (bucketIt == vecIt->end())
		{
			return end();
		}
		return ConstIterator(vecIt, _buckets.end(), bucketIt);
	}
	
	Iterator		find(const Value& value)
	{
		std::size_t hashAddr = _hash(value);
		BucketVecIterator vecIt(_buckets.begin() + hashAddr);
		assert(vecIt != _buckets.end());			
		BucketIterator bucketIt(std::find(vecIt->begin(), vecIt->end(), value));
		if (bucketIt == vecIt->end())
		{
			return end();
		}
		return Iterator(vecIt, _buckets.end(), bucketIt);
	}

	ConstIterator	begin() const
	{
		BucketVecIterator begin_it(_buckets.begin());
		BucketVecIterator end_it(_buckets.end());
		BucketVecIterator iter = begin_it;
		while(iter != end_it && iter->empty())
		{
			++iter;
		}
		if (iter == end_it)
		{
			return end();
		}
		else
		{
			return ConstIterator(begin_it, end_it, iter->begin());
		}
	}

	Iterator	begin()
	{
		BucketVecIterator vec_it(_buckets.begin());
		BucketVecIterator end_it(_buckets.end());
		BucketVecIterator iter = vec_it;
		while(iter != end_it && iter->empty())
		{
			++iter;
		}
		if (iter == end_it)
		{
			return end();
		}
		else
		{
			return Iterator(vec_it, end_it, iter->begin());
		}
	}

	ConstIterator	end() const
	{
		return ConstIterator(_buckets.end(), _buckets.end(), _buckets.begin()->end() /*此处可以是任何值*/);
	}

	Iterator	end()
	{
		return Iterator(_buckets.end(), _buckets.end(), _buckets.begin()->end() /*此处可以是任何值*/);
	}

	std::pair<Iterator, bool> insert(const Value& value)
	{
		std::size_t hashAddr = getHashAddress(value);
		BucketVecIterator iter(_buckets.begin()+hashAddr);
		BucketIterator bucketIt = std::find(iter.begin(), iter.end(), value); 
		if (bucketIt != iter.end())
		{
			return std::make_pair(Iterator(iter, iter->end(), bucketIt), false);
		} 
		else
		{
			slice();
			hashAddr = getHashAddress(value);
			iter = _buckets.begin()+hashAddr;
			bucketIt = iter->insert(iter->end(), value);				
			++_size;
			
			return std::make_pair(Iterator(iter, _buckets.end(), bucketIt), true);
		}
	}

	void erase(const Value& value)
	{
		Iterator iter = find(value);
		erase(iter);
	}

	void erase(Iterator& iter)
	{
		if (iter == end())
		{
			return;
		}
		iter._bucketIt->erase(iter._bucketIt);
		--_size;
		merge();
	}

private:
	std::size_t getHashAddress(const Value& value) const
	{
		std::size_t hashCode = _hash(value);
		if (hashCode % _front < _sliceIdx)
		{
			// 已分裂
			return hashCode % (_front<<1);
		} 
		else
		{
			// 未分裂
			return hashCode % _front;
		}
	}

	void slice()
	{
		if (_sliceIdx == _front)
		{
			_sliceIdx = 0;
			_front <<= 1;
			_buckets.reserve(_front<<1);
		}

		using std::swap;
		Bucket tmp;
		_buckets.push_back(tmp);
		swap(_buckets[_sliceIdx], tmp);
		++_sliceIdx;
		BucketIterator iter = tmp.begin();
		for (; iter != tmp.end(); ++iter)
		{
			std::size_t addr = getHashAddress(*iter);
			_buckets[addr].push_back(*iter);
		}			
	}

	void merge()
	{
		if (_sliceIdx == 0)
		{
			_front >>= 1;
			_sliceIdx = _front;
		}
		--_sliceIdx;
		BucketIterator iter = _buckets.back().begin();
		for (; iter != _buckets.back().end(); ++iter)
		{
			_buckets[getHashAddress(*iter)].push_back(*iter);
		}
		_buckets.pop_back();
	}

	std::size_t size()
	{
		return _size;
	}

	bool empty()
	{
		return size() == 0;
	}

	std::size_t bucketSize()
	{
		return _buckets.size();
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