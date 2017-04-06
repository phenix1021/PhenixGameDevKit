/*******************************************************************************
 * @brief	对象转数组下标
 * @date	2017-2-26
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_INDEX_H
#define PHENIX_BASE_INDEX_H

namespace Phenix
{

template<typename T>
class IndexMgr
{
public:
	IndexMgr():_nextIdx(0){}
	virtual ~IndexMgr(){}
	
	bool hasObj(const T& obj);
	bool getIdx(const T& obj, Phenix::Int32& idx) const;
	bool getObj(Phenix::Int32 idx, T& obj) const;
	
	bool addObj(const T& val, Phenix::Int32& idx);
	void delObj(const T& val);

	Phenix::Int32 getObjCount(){return _objs.size();}

private:
	std::map<T, Phenix::Int32>	_objs;
	std::map<Phenix::Int32, T>	_idxes;	
	Phenix::Int32				_nextIdx;
	std::vector<Phenix::Int32> _unuses;
};

template<typename T>
bool Phenix::IndexMgr<T>::hasObj( const T& obj )
{
	std::map<T, Phenix::Int32>::iterator iter = _objs.find(obj);
	return iter != _objs.end();
}

template<typename T>
bool Phenix::IndexMgr<T>::getObj( Phenix::Int32 idx, T& obj ) const
{
	std::map<Phenix::Int32, T>::const_iterator iter = _idxes.find(idx);
	if (iter == _idxes.end())
	{
		return false;
	}
	obj = iter->second;
	return true;
}

template<typename T>
bool Phenix::IndexMgr<T>::getIdx( const T& obj, Phenix::Int32& idx ) const
{
	std::map<T, Phenix::Int32>::const_iterator iter = _objs.find(obj);
	if (iter == _objs.end())
	{
		return false;
	}
	idx = iter->second;
	return true;
}

template<typename T>
void Phenix::IndexMgr<T>::delObj( const T& obj )
{
	Phenix::Int32 idx = 0;	
	if (getIdx(obj, idx))
	{
		_objs.erase(obj);
		_idxes.erase(idx);
		_unuses.push_back(idx);
	}
}

template<typename T>
bool Phenix::IndexMgr<T>::addObj( const T& obj, Phenix::Int32& idx )
{
	if (hasObj(obj))
	{
		return false;
	}
	if (_unuses.empty())
	{
		_idxes[_nextIdx] = obj;
		_objs[obj]		 = _nextIdx;
		idx				 = _nextIdx;
		++_nextIdx;
	} 
	else
	{
		_idxes[_unuses.back()] = obj;
		_objs[obj]		 = _unuses.back();
		idx				 = _unuses.back();
		_unuses.pop_back();
	}	
	return true;
}





} // end namespace Phenix

#endif // PHENIX_BASE_INDEX_H