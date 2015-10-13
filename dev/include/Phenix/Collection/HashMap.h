/*******************************************************************************
 * @brief	哈希map（线性哈希）
 * @date	2015-10-13
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_HASHMAP_H
#define PHENIX_COLLECTION_HASHMAP_H

#include <Phenix/Base/Hash.h>
#include <Phenix/Collection/HashTable.h>


namespace Phenix
{
namespace Collection
{

	template<typename Key, typename Value, typename HashFunc = HashCode<Key>>
	class HashMap
	{
		struct HashMapEntry
		{
			Key		first;
			Value	second;

			HashMapEntry(){}
			HashMapEntry(const Key& key):first(key){}
			HashMapEntry(const Key& key, const Value& value):first(key), second(value){}
			
			bool operator == (const HashMapEntry& o) const
			{
				return first == o.first;
			}

			bool operator != (const HashMapEntry& o) const
			{
				return !(*this == o);
			}
		};

		struct HashMapEntryHashFunc
		{			
			std::size_t operator () (const HashMapEntry& entry)
			{
				return _hashFunc(entry.first);	
			}

		private:
			HashFunc _hashFunc;
		};

	public:
		typedef typename HashTable<HashMapEntry, HashMapEntryHashFunc>::ConstIterator	ConstIterator;
		typedef typename HashTable<HashMapEntry, HashMapEntryHashFunc>::Iterator		Iterator;

		HashMap(){}
		HashMap(std::size_t bucketReserveSize):_table(bucketReserveSize){}

		HashMap& operator = (const HashMap& o)
		{
			this->assign(0);
		}

		HashMap& assign(const HashMap& o)
		{
			_table.assign(o._table);
			return *this;
		}

		void swap(HashMap& o)
		{
			_table.swap(o._table);
		}

		std::size_t size()
		{
			_table.size();
		}

		bool empty()
		{
			return _table.empty();
		}

		ConstIterator begin() const
		{
			return _table.begin();
		}

		Iterator begin()
		{
			return _table.begin();
		}

		ConstIterator end() const
		{
			return _table.end();
		}

		Iterator end()
		{
			return _table.end();
		}

		ConstIterator find(const Key& key) const
		{
			return _table.find(HashMapEntry(key));
		}

		Iterator find(const Key& key)
		{
			return _table.find(HashMapEntry(key));
		}

		std::pair<Iterator, bool> insert(const std::pair<Key, Value>& pair)
		{
			return _table.insert(HashMapEntry(pair.first, pair.second));
		}

		void erase(Iterator iter)
		{
			_table.erase(iter);
		}

		void erase(const Key& key)
		{
			_table.erase(HashMapEntry(key));
		}

		void clear()
		{
			_table.clear();
		}

		Value& operator [] (const Key& key)
		{
			std::pair<Iterator, bool> rlt = _table.insert(HashMapEntry(key));// 注意：_table.insert如果已有值会返回所在值，否则插入
			return rlt.first->second; // rlt.second为true，返回插入的新值，否则返回已有的值
		}

		const Value& operator [] (const Key& key) const
		{
			ConstIterator iter = _table.find(HashMapEntry(key));
			if (iter != end())
			{
				return iter->second;
			}
			throw;
		}

	private:		
		HashTable<HashMapEntry, HashMapEntryHashFunc> _table;
	};
}
}


#endif