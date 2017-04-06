/*******************************************************************************
 * @brief	图基类
 * @date	2017-2-20
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_GRAPH_BASE_H
#define PHENIX_GRAPH_GRAPH_BASE_H

#include <Phenix/Base/Index.h>

namespace Phenix
{
namespace Graph
{

enum WeightType
{	
	SELF	= 0,
	NORMAL	= 1,	
	NONE	= 0xFF
};

template<typename NODE, typename WEIGHT>
class GraphBase
	:protected IndexMgr<NODE>
{
	template<typename NODE, typename WEIGHT> friend class Dijkstra;
	template<typename NODE, typename WEIGHT> friend class Floyd;

public:
	struct WeightData
	{
		WEIGHT			value; 
		Phenix::UInt8	type;	// WeightType
		WeightData():type(NONE){}

		bool operator <(const WeightData& o) const
		{
			if (type == o.type)
			{
				if (type == NONE)
				{
					return false;
				}
				return value < o.value;
			}
			return type < o.type;
		}

		WeightData operator +(const WeightData& o)
		{
			WeightData rlt;
			if (this->type != NONE && o.type != NONE)
			{
				if (this->type == SELF && o.type == SELF)
				{
					rlt.type = SELF;
				}
				else if (this->type == SELF)
				{
					rlt.type = NORMAL;
					rlt.value = o.value;
				} 
				else if (o.type == SELF)
				{
					rlt.type = NORMAL;
					rlt.value = this->value;
				}
				else
				{
					rlt.type = NORMAL;
					rlt.value = this->value + o.value;
				}				
			}			
			return rlt;
		}
	};

	typedef std::vector<std::vector<WeightData>>	Weights;
	typedef std::vector<Phenix::Int32>				Neighbors;	
	typedef std::map<Phenix::Int32, Neighbors>		Edges;	

public:
	// 是否存在节点
	bool hasNode(const NODE& node);
	// 获得节点数量
	Phenix::Int32 getNodeCount();
	// 新增节点
	void addNode(const NODE& node);
	// 获得边权值
	bool getWeight(const NODE& node1, const NODE& node2, WEIGHT& weight);	

	// 新增/修改边
	virtual void addEdge(const NODE& node1, const NODE& node2, const WEIGHT& weight) = 0;
	// 删除节点
	virtual void removeNode(const NODE& node) = 0;
	// 删除边
	virtual void removeEdge(const NODE& node1, const NODE& node2) = 0;	
	// 是否连通图
	virtual bool isConnected() = 0;

public:
	inline  const Edges&	getEdges() const { return _edges; }
	inline  const Weights&	getWeights() const { return _weights; }

protected:
	GraphBase(){}
	virtual ~GraphBase(){}

	inline  Edges&	 getEdges() { return _edges; }
	inline  Weights& getWeights() { return _weights; }	

	void	delFromNeighbors(Neighbors& neighbors, Phenix::Int32 idx);	

private:	
	Weights		_weights;	// 矩阵
	Edges		_edges;		// 边集
};

template<typename NODE, typename WEIGHT>
Phenix::Int32 Phenix::Graph::GraphBase<NODE, WEIGHT>::getNodeCount()
{
	return getObjCount();
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::GraphBase<NODE, WEIGHT>::delFromNeighbors( Neighbors& neighbors, Phenix::Int32 idx )
{
	Neighbors::iterator iter = neighbors.begin();
	for (; iter != neighbors.end(); ++iter)
	{
		if (*iter == idx)
		{
			neighbors.erase(iter);
			return;
		}
	}
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::GraphBase<NODE, WEIGHT>::addNode( const NODE& node )
{
	if (hasNode(node))
	{
		return;
	}
	Phenix::Int32 idx = 0;
	if (!addObj(node, idx))
	{
		return;
	}
	std::vector<WeightData> tmp;
	for (Phenix::Int32 i=0; i<_weights.size(); ++i)
	{
		_weights[i].push_back(WeightData());
		tmp.push_back(WeightData());
	}
	tmp.push_back(WeightData());
	_weights.push_back(tmp);	
	_weights[idx][idx].type = SELF;		
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::hasNode( const NODE& node )
{	
	return hasObj(node);
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::getWeight( const NODE& node1, const NODE& node2, WEIGHT& weight )
{	
	Phenix::Int32 idx1 = 0;
	Phenix::Int32 idx2 = 0;
	if (!getIdx(node1, idx1) || !getIdx(node2, idx2))
	{
		return false;
	}	
	WeightData data = _weights[idx1][idx2];
	if (data.type == SELF || data.type == NONE)
	{
		return false;
	}
	weight = data.value;
	return true;	
}

}
}


#endif // PHENIX_GRAPH_GRAPH_BASE_H