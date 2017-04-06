/*******************************************************************************
 * @brief	有向图
 * @date	2017-2-20
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_DIRECTED_GRAPH_H
#define PHENIX_GRAPH_DIRECTED_GRAPH_H

#include <Phenix/Graph/GraphBase.h>

namespace Phenix
{
namespace Graph
{

template<typename NODE, typename WEIGHT>
class DirectedGraph
	:public GraphBase<NODE, WEIGHT>
{	
public:
	DirectedGraph(){}
	virtual ~DirectedGraph(){}
		
	virtual void addEdge(const NODE& node1, const NODE& node2, const WEIGHT& weight);			
	virtual void removeNode(const NODE& node);	
	virtual void removeEdge(const NODE& node1, const NODE& node2);
	virtual bool isConnected();

private:
	Edges	_inEdges;	// 入边
};

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::DirectedGraph<NODE, WEIGHT>::isConnected()
{
	return false;
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::DirectedGraph<NODE, WEIGHT>::removeEdge( const NODE& node1, const NODE& node2 )
{
	if (!hasNode(node1) || !hasNode(node2))
	{
		return;
	}
	Phenix::Int32 idx1 = 0;
	getIdx(node1, idx1);
	Phenix::Int32 idx2 = 0;	
	getIdx(node2, idx2);

	getWeights()[idx1][idx2].type = NONE;	

	Edges::iterator iter = getEdges().find(idx1);
	if (iter != getEdges().end())
	{
		Neighbors& neighbors = iter->second;
		delFromNeighbors(neighbors, idx2);
		if (neighbors.empty())
		{
			getEdges().erase(idx1);
		}
	}
	
	iter = _inEdges.find(idx2);
	if (iter != _inEdges.end())
	{
		Neighbors& neighbors = iter->second;
		delFromNeighbors(neighbors, idx1);
		if (neighbors.empty())
		{
			_inEdges.erase(idx2);
		}
	}
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::DirectedGraph<NODE, WEIGHT>::removeNode( const NODE& node )
{
	Phenix::Int32 idx = 0;
	if (!getIdx(node, idx))
	{
		return;
	}

	for (Phenix::Int32 i=0; i<getWeights().size(); ++i)
	{
		getWeights()[i][idx].type = NONE;
		getWeights()[idx][i].type = NONE;
	}

	// 出边
	Edges::iterator outIter = getEdges().find(idx);
	if (outIter != getEdges().end())
	{
		Neighbors& neighbors = outIter->second;
		for (Neighbors::iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); ++neighbor_iter)
		{
			Phenix::Int32 neighborIdx = *neighbor_iter;
			Neighbors& neighborsOfNeighbors = _inEdges[neighborIdx];
			delFromNeighbors(neighborsOfNeighbors, idx);
			if (neighborsOfNeighbors.empty())
			{
				_inEdges.erase(neighborIdx);
			}
		}
		getEdges().erase(idx);		
	}
	// 入边
	Edges::iterator inIter = _inEdges.find(idx);
	if (inIter != _inEdges.end())
	{
		Neighbors& neighbors = inIter->second;
		for (Neighbors::iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); ++neighbor_iter)
		{
			Phenix::Int32 neighborIdx = *neighbor_iter;
			Neighbors& neighborsOfNeighbors = getEdges()[neighborIdx];
			delFromNeighbors(neighborsOfNeighbors, idx);
			if (neighborsOfNeighbors.empty())
			{
				getEdges().erase(neighborIdx);
			}
		}
		_inEdges.erase(idx);		
	}

	delObj(node);	
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::DirectedGraph<NODE, WEIGHT>::addEdge( const NODE& node1, const NODE& node2, const WEIGHT& weight )
{
	if (node1 == node2)
	{
		return;
	}
	if (!hasNode(node1))
	{
		addNode(node1);
	}
	if (!hasNode(node2))
	{
		addNode(node2);
	}
	Phenix::Int32 idx1 = 0;
	getIdx(node1, idx1);
	Phenix::Int32 idx2 = 0;
	getIdx(node2, idx2);

	WeightData data;
	data.type = NORMAL;
	data.value = weight;
	getWeights()[idx1][idx2] = data;	

	getEdges()[idx1].push_back(idx2);
	_inEdges[idx2].push_back(idx1);
}


}
}


#endif // PHENIX_GRAPH_DIRECTED_GRAPH_H