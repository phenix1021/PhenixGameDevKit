/*******************************************************************************
 * @brief	ÎÞÏòÍ¼
 * @date	2017-2-20
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_UNDIRECTED_GRAPH_H
#define PHENIX_GRAPH_UNDIRECTED_GRAPH_H

#include <Phenix/Graph/GraphBase.h>

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
		
	virtual bool addEdge(NODE node1, NODE node2, WEIGHT weight);			
	virtual bool removeNode(NODE node);	
	virtual bool removeEdge(NODE node1, NODE node2);	
};

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::removeEdge( NODE node1, NODE node2 )
{
	if (!hasNode(node1) || !hasNode(node2))
	{
		return false;
	}
	getOutEdges()[node1].erase(node2);
	getOutEdges()[node2].erase(node1);
	return true;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::removeNode( NODE node )
{	
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator iter = getOutEdges().find(node);
	if (iter == getOutEdges().end())
	{
		return false;
	}
	std::map<NODE, WEIGHT>::iterator itr = iter->second.begin();
	for (; itr != iter->second.end(); ++itr)
	{
		getOutEdges()[itr->first].erase(node);
	}
	getOutEdges().erase(node);
	return true;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::UndirectedGraph<NODE, WEIGHT>::addEdge( NODE node1, NODE node2, WEIGHT weight )
{
	if (node1 == node2)
	{
		return false;
	}
	getOutEdges()[node1][node2] = weight;
	getOutEdges()[node2][node1] = weight;
	return true;
}


}
}


#endif // PHENIX_GRAPH_UNDIRECTED_GRAPH_H