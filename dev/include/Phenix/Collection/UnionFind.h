/*******************************************************************************
 * @brief	并查集
 * @date	2017-2-19
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_UNION_FIND_H
#define PHENIX_COLLECTION_UNION_FIND_H

#include <map>
#include <set>
#include <Phenix/Base/Index.h>

namespace Phenix
{
namespace Collection
{

template<typename T>
class UnionFind
	:Phenix::IndexMgr<T>
{
public:
	UnionFind(){}
	virtual ~UnionFind(){}

	void addRel(const T& t1, const T& t2);	// 并
	bool isRel(T& t1, T& t2);	// 查
	void stats(Phenix::Int32& total, Phenix::Int32& rootCount);

private:	
	Phenix::Int32 getRoot(Phenix::Int32 idx);

private:
	std::vector<Phenix::Int32> _roots;	// root列表
};

template<typename T>
void Phenix::Collection::UnionFind<T>::stats( Phenix::Int32& total, Phenix::Int32& rootCount )
{
	total = _roots.size();	
	std::set<Phenix::Int32> tmp;
	std::vector<Phenix::Int32>::iterator iter = _roots.begin();
	for (; iter != _roots.end(); ++iter)
	{
		tmp.insert(*iter);		
	}
	rootCount = tmp.size();
}

template<typename T>
Phenix::Int32 Phenix::Collection::UnionFind<T>::getRoot( Phenix::Int32 idx )
{
	if (_roots[idx] == idx)
	{
		return idx;
	}
	_roots[idx] = getRoot(_roots[idx]); // 同时进行路径压缩
	return _roots[idx];
}

template<typename T>
void Phenix::Collection::UnionFind<T>::addRel( const T& t1, const T& t2 )
{
	Phenix::Int32 idx1 = 0;
	if (!getIdx(t1, idx1))
	{
		addObj(t1, idx1);
		_roots.push_back(idx1);
	}
	Phenix::Int32 idx2 = 0;
	if (!getIdx(t2, idx2))
	{
		addObj(t2, idx2);
		_roots.push_back(idx2);
	}
	if (getRoot(idx1) == getRoot(idx2))
	{
		return;
	}
	_roots[getRoot(idx2)] = idx1;
}

template<typename T>
bool Phenix::Collection::UnionFind<T>::isRel( T& t1, T& t2 )
{
	Phenix::Int32 idx1 = 0;
	if (!getIdx(t1, idx1))
	{
		return false;
	}
	Phenix::Int32 idx2 = 0;
	if (!getIdx(t2, idx2))
	{
		return false;
	}

	return getRoot(idx1) == getRoot(idx2);
}



}
}


#endif // PHENIX_COLLECTION_UNION_FIND_H