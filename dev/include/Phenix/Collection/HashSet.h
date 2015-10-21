/*******************************************************************************
 * @brief	¹þÏ£set£¨ÏßÐÔ¹þÏ££©
 * @date	2015-10-5
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_HASHSET_H
#define PHENIX_COLLECTION_HASHSET_H

#include <Phenix/Base/Hash.h>
#include <Phenix/Collection/HashTable.h>

namespace Phenix
{
namespace Collection
{

template<typename Value, typename HashFunc = HashCode<Value>>
class HashSet
{
public:
	typedef HashTable<Value, HashFunc>::Iterator		Iterator;
	typedef HashTable<Value, HashFunc>::ConstIterator	ConstIterator;

	HashSet(std::size_t bucketReserveSize):_table(bucketReserveSize){}

	void swap(HashSet& o)
	{
		_table.swap(o._table);
	}

	void assign(const HashSet& o)
	{
		_table.assign(o._table);
	}

	HashSet& operator = (const HashSet& o)
	{
		assign(o);
		return *this;
	}

	Iterator begin()
	{
		return _table.begin();
	}

	ConstIterator begin() const;
	{
		return _table.begin();
	}
	
	Iterator end()
	{
		return _table.end();
	}

	ConstIterator end() const;
	{
		return _table.end();
	}

	Iterator find(const Value& value)
	{
		return _table.find(value);
	}

	ConstIterator find(const Value& value) const
	{
		return _table.find(value);
	}

	std::size_t size()
	{
		return _table.size();
	}

	bool empty()
	{
		return _table.empty();
	}

	void clear()
	{
		_table.clear();
	}

	std::pair<Iterator, bool> insert(const Value& value)
	{
		return _table.insert(value);
	}

	void erase(const Value& value)
	{
		_table.erase(value);
	}

	void erase(Iterator iter)
	{
		_table.erase(iter);
	}

private:
	HashTable<Value, HashFunc>	_table;
};	

}
}


#endif