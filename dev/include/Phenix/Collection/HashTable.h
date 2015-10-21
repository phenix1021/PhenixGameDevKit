/*******************************************************************************
 * @brief	��ϣ�����Թ�ϣ��
 * @date	2015-10-5
 * @author	phenix
 * @mark    
			ԭ��
			1. hash(x) % _front == bucket_idx
			   hash(x) % (_front*2) == bucket_idx ���� bucket_idx + _front
		    2. slice_idx ��0��ʼ����ʾ��ǰҪ���ѵ�bucket���Ը�bucket����hash(x) % (_front*2)��
			   ���¹���bucket�ڸ��
		    3. ��ʼbucket����==_front��ÿ��sliceʱ++_front��ͬʱpushbackһ���µ�bucket��
		 	4. slice����ʱ�����Ը���ʵ������������õ���ÿ��insert���ݶ�sliceһ�Ρ�
			5. ��slice_idx == _frontʱ��˵��Ͱ�ﱾ������bucket�Ѿ�slice��һ�顣
			   ��ʱslice_idx = 0,_front *= 2��Ȼ��ʼ��һ��slice��
			6. ÿ��erase���ݶ�����һ��merge�������߼���slice�෴��

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
	typedef typename Bucket::iterator BucketIterator;	// ע��Ҫ��typename,����������޷�֪��iterator	
	typedef std::vector<Bucket> BucketVec;
	typedef typename BucketVec::iterator BucketVecIterator;		// ע��Ҫ��typename,����������޷�֪��iterator	

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
		// Ҳ����д��inline const typename Bucket::value_type& operator * () const
		{
			return *_bucketIt;
		}

		inline const typename Value* operator -> () const
		// Ҳ����д��inline const typename Bucket::value_type* operator * () const
		{
			return &*_bucketIt;
		}

		inline bool operator == (const ConstIterator& o) const	// �Ƿ�ָ��ͬһλ��
		{
			return _vecIt == o._vecIt && (_vecIt == _endIt || _bucketIt == o._bucketIt);
		}

		inline bool operator != (const ConstIterator& o) const	// �Ƿ�ָ��ͬλ��
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
		BucketVecIterator	_vecIt;		// ��ǰBucketVecλ��
		BucketVecIterator	_endIt;		// ��βBucketVecλ��(ָ�������)
		BucketIterator		_bucketIt;	// ��ǰBucketλ��			
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
		// Ҳ����д��inline typename Bucket::value_type& operator * ()
		{
			return *_bucketIt;
		}

		inline const typename Value& operator * () const
			// Ҳ����д��inline typename Bucket::value_type& operator * ()
		{
			return *_bucketIt;
		}

		inline typename Value* operator -> ()
		// Ҳ����д��inline const typename Bucket::value_type* operator * () const
		{
			return &*_bucketIt;
		}

		inline const typename Value* operator -> () const
			// Ҳ����д��inline const typename Bucket::value_type* operator * () const
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
		return ConstIterator(_buckets.end(), _buckets.end(), _buckets.begin()->end() /*�˴��������κ�ֵ*/);
	}

	Iterator	end()
	{
		return Iterator(_buckets.end(), _buckets.end(), _buckets.begin()->end() /*�˴��������κ�ֵ*/);
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
			// �ѷ���
			return hashCode % (_front<<1);
		} 
		else
		{
			// δ����
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
	std::size_t	_sliceIdx;	// ��ǰ׼�����ѵ�_buckets�±�
	std::size_t	_front;		// ģ��		
	std::size_t	_size;		// Ԫ�ظ���
	
	BucketVec	_buckets;
	HashFunc	_hash;
};	

}
}


#endif