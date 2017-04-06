/*******************************************************************************
 * @brief	无向图
 * @date	2017-2-20
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_UNDIRECTED_GRAPH_H
#define PHENIX_GRAPH_UNDIRECTED_GRAPH_H

#include <Phenix/Graph/GraphBase.h>
#include <Phenix/Collection/UnionFind.h>

namespace Phenix
{
namespace Graph
{

template<typename NODE, typename WEIGHT>
class UndirectedGraph
	:public GraphBase<NODE, WEIGHT>
{	
public:
	UndirectedGraph(){}
	virtual ~UndirectedGraph(){}
		
	virtual void addEdge(const NODE& node1, const NODE& node2, const WEIGHT& weight);			
	virtual void removeNode(const NODE& node);	
	virtual void removeEdge(const NODE& node1, const NODE& node2);	
	virtual bool isConnected();
};

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::isConnected()
{
	Phenix::Collection::UnionFind<Phenix::Int32> uf;
	const Edges& edges = getEdges();
	for (Edges::const_iterator iter = edges.begin(); iter != edges.end(); ++iter)
	{
		const Neighbors& neighbors = iter->second;
		for (Neighbors::const_iterator itr = neighbors.begin(); itr != neighbors.end(); ++itr)
		{
			uf.addRel(iter->first, *itr);
		}
	}
	Phenix::Int32 nodeCount = getNodeCount(); // 图节点个数
	Phenix::Int32 total = 0, rootCount = 0;
	uf.stats(total, rootCount);
	
	return (nodeCount == total && rootCount == 1);	
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::removeEdge( const NODE& node1, const NODE& node2 )
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
	getWeights()[idx2][idx1].type = NONE;

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
	
	iter = getEdges().find(idx2);
	if (iter != getEdges().end())
	{
		Neighbors& neighbors = iter->second;
		delFromNeighbors(neighbors, idx1);
		if (neighbors.empty())
		{
			getEdges().erase(idx2);
		}
	}
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::removeNode( const NODE& node )
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
	
	Edges::iterator iter = getEdges().find(idx);
	if (iter == getEdges().end())
	{
		return;
	}
	Neighbors& neighbors = iter->second;
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

	getEdges().erase(idx);

	delObj(node);	
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::addEdge( const NODE& node1, const NODE& node2, const WEIGHT& weight )
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
	getWeights()[idx2][idx1] = data;

	getEdges()[idx1].push_back(idx2);
	getEdges()[idx2].push_back(idx1);
}


}
}


#endif // PHENIX_GRAPH_UNDIRECTED_GRAPH_H