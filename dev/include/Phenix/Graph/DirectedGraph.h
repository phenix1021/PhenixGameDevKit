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
		
	virtual bool addEdge(NODE node1, NODE node2, WEIGHT weight);			
	virtual bool removeNode(NODE node);	
	virtual bool removeEdge(NODE node1, NODE node2);

private:
	std::map<NODE, std::map<NODE, WEIGHT>>	_inEdges;	// 入边（不包括无入边的节点）
};

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::DirectedGraph<NODE, WEIGHT>::removeEdge( NODE node1, NODE node2 )
{
	if (!hasNode(node1) || !hasNode(node2))
	{
		return false;
	}
	getOutEdges()[node1].erase(node2);
	_inEdges[node2].erase(node1);
	return true;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::DirectedGraph<NODE, WEIGHT>::removeNode( NODE node )
{
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator outIter = getOutEdges().find(node);
	if (outIter == getOutEdges().end())
	{
		return false; // 即使无边的节点，在outEdges也保留空数据
	}
	// 处理出边
	std::map<NODE, WEIGHT>::iterator outItr = outIter->second.begin();
	for (; outItr != outIter->second.end(); ++outItr)
	{
		_inEdges[outItr->first].erase(node);
	}
	getOutEdges().erase(node);
	
	// 处理入边
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator inIter = _inEdges.find(node);
	if (inIter != _inEdges.end())
	{
		std::map<NODE, WEIGHT>::iterator inItr = inIter->second.begin();
		for (; inItr != inIter->second.end(); ++inItr)
		{
			getOutEdges()[inItr->first].erase(node);
		}
		_inEdges.erase(node);
	}	
	return true;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::DirectedGraph<NODE, WEIGHT>::addEdge( NODE node1, NODE node2, WEIGHT weight )
{
	if (node1 == node2)
	{
		return false;
	}
	getOutEdges()[node1][node2] = weight;
	addNode(node2);
	_inEdges[node2][node1] = weight;
	return true;
}


}
}


#endif // PHENIX_GRAPH_DIRECTED_GRAPH_H