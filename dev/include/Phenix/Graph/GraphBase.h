/*******************************************************************************
 * @brief	ͼ����
 * @date	2017-2-20
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_GRAPH_BASE_H
#define PHENIX_GRAPH_GRAPH_BASE_H

namespace Phenix
{
namespace Graph
{

template<typename NODE, typename WEIGHT>
struct Path
{
	WEIGHT				totalWeight;	// ·����Ȩֵ
	std::vector<NODE>	nodes;			// ·�����ڵ�
};

template<typename NODE, typename WEIGHT>
class GraphBase
{
public:
	// �Ƿ���ڽڵ�
	virtual bool hasNode(NODE node);
	// �����ڵ�
	virtual bool addNode(NODE node);
	// ����/�޸ı�
	virtual bool addEdge(NODE node1, NODE node2, WEIGHT weight) = 0;
	// ɾ���ڵ�
	virtual bool removeNode(NODE node) = 0;
	// ɾ����
	virtual bool removeEdge(NODE node1, NODE node2) = 0;
	// ��ñ�Ȩֵ
	bool getWeight(NODE node1, NODE node2, WEIGHT& weight);	

public:
	bool getPath(NODE start, NODE finish, Path<NODE, WEIGHT>& path);

protected:
	GraphBase(){}
	virtual ~GraphBase(){}

	inline  std::map<NODE, std::map<NODE, WEIGHT>>& getOutEdges() { return _outEdges; }

private:
	std::map<NODE, std::map<NODE, WEIGHT>>	_outEdges;	// ����(���������ڵ�)
	
	//std::map<NODE, std::map<NODE, Path>>	_pathCaches;	// ·������,�߻���ϵͳ
};

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::getPath( NODE start, NODE finish, Path<NODE, WEIGHT>& path )
{
	return false;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::addNode( NODE node )
{
	if (hasNode(node))
	{
		return false;
	}
	_outEdges[node] = std::map<NODE, WEIGHT>();
	return true;
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::hasNode( NODE node )
{
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator iter =	_outEdges.find(node);
	return iter != _outEdges.end();
}

template<typename NODE, typename WEIGHT>
bool Phenix::Graph::GraphBase<NODE, WEIGHT>::getWeight( NODE node1, NODE node2, WEIGHT& weight )
{	
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator iter1 = _outEdges.find(node1);
	if (iter1 == _outEdges.end())
	{
		return false;
	}
	std::map<NODE, std::map<NODE, WEIGHT>>::iterator iter2 = _outEdges.find(node2);
	if (iter2 == _outEdges.end())
	{
		return false;
	}

	std::map<NODE, WEIGHT>::iterator itr = iter1->second.find(node2);
	if (itr == iter1->second.end())
	{
		return false;
	}
	weight = itr->second;
	return true;	
}

}
}


#endif // PHENIX_GRAPH_GRAPH_BASE_H