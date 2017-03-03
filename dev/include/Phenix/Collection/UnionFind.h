/*******************************************************************************
 * @brief	并查集
 * @date	2017-2-19
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_UNION_FIND_H
#define PHENIX_COLLECTION_UNION_FIND_H

#include <map>

namespace Phenix
{
namespace Collection
{

template<typename T>
class UnionFind
{
public:
	UnionFind(){}
	virtual ~UnionFind(){}

	bool isRel(T& t1, T& t2);
	void addRel(T& t1, T& t2);

private:
	bool getIdx(T& t, Phenix::UInt32& idx);
	Phenix::UInt32 getRoot(Phenix::UInt32 idx);

private:
	std::vector<Phenix::UInt32> _parentList;	// parent列表
	std::map<T,Phenix::UInt32>	_idxMap;		// T在_parentList中的下标
};

template<typename T>
Phenix::UInt32 Phenix::Collection::UnionFind<T>::getRoot( Phenix::UInt32 idx )
{
	if (_parentList[idx] == idx)
	{
		return idx;
	}
	_parentList[idx] = getRoot(_parentList[idx]); // 同时进行路径压缩
	return _parentList[idx];
}

template<typename T>
bool Phenix::Collection::UnionFind<T>::getIdx( T& t, Phenix::UInt32& idx )
{
	std::map<T,Phenix::UInt32>::iterator iter = _idxMap.find(t);
	if (iter == _idxMap.end())
	{
		return false;
	}
	idx = iter->second;
	return true;
}

template<typename T>
void Phenix::Collection::UnionFind<T>::addRel( T& t1, T& t2 )
{
	Phenix::UInt32 idx1 = 0;
	if (!getIdx(t1, idx1))
	{
		idx1 = _parentList.size();
		_idxMap[t1] = idx1;
		_parentList.push_back(idx1);
	}
	Phenix::UInt32 idx2 = 0;
	if (!getIdx(t2, idx2))
	{
		idx2 = _parentList.size();
		_idxMap[t2] = idx2;
		_parentList.push_back(idx2);
	}
	if (getRoot(idx1) == getRoot(idx2))
	{
		return;
	}
	_parentList[getRoot(idx2)] = idx1;
}

template<typename T>
bool Phenix::Collection::UnionFind<T>::isRel( T& t1, T& t2 )
{
	Phenix::UInt32 idx1 = 0;
	if (!getIdx(t1, idx1))
	{
		return false;
	}
	Phenix::UInt32 idx2 = 0;
	if (!getIdx(t2, idx2))
	{
		return false;
	}

	return getRoot(idx1) == getRoot(idx2);
}



}
}


#endif // PHENIX_COLLECTION_UNION_FIND_H