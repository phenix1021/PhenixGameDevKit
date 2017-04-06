/*******************************************************************************
 * @brief	���·��
 * @date	2017-3-17
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_GRAPH_MIN_PATH_H
#define PHENIX_GRAPH_MIN_PATH_H

#include <Phenix/Graph/GraphBase.h>
#include <algorithm>

namespace Phenix
{
namespace Graph
{

template<typename NODE, typename WEIGHT>
struct Path
{
	std::vector<NODE>	nodes;
	WEIGHT				totalWeight;
};

// �Ͽ�˹�����㷨(��֧�ָ�Ȩ��)
template<typename NODE, typename WEIGHT>
class Dijkstra
{
public:	
	Dijkstra(const GraphBase<NODE, WEIGHT>&	graph):_graph(graph){}
	virtual ~Dijkstra(){}
	
	void operator ()(const NODE& start, std::map<NODE, Path<NODE, WEIGHT>>& paths);

private:
	const GraphBase<NODE, WEIGHT>&	_graph;	
};

template<typename NODE, typename WEIGHT>
void Phenix::Graph::Dijkstra<NODE, WEIGHT>::operator()( const NODE& start, std::map<NODE, Path<NODE, WEIGHT>>& paths )
{
	typedef GraphBase<NODE, WEIGHT>::Weights	Weights;
	typedef GraphBase<NODE, WEIGHT>::Edges		Edges;
	typedef GraphBase<NODE, WEIGHT>::Neighbors	Neighbors;
	typedef GraphBase<NODE, WEIGHT>::WeightData	WeightData;

	const Weights& weights	= _graph.getWeights();
	const Edges& edges		= _graph.getEdges();
	Phenix::Int32 startIdx = 0;
	if (!_graph.getIdx(start, startIdx))
	{
		return;
	}	

	// start�������·������
	std::vector<WeightData> dist;
	// �����close/open״̬
	std::vector<bool> closed;
	// ��㵽������м��
	std::vector<Phenix::Int32> step;
	
	// ��ʼ��
	for (Phenix::Int32 i=0; i<weights.size(); ++i)
	{
		WeightData data;
		if (i == startIdx)
		{
			data.type = SELF;			
		}		
		dist.push_back(data);
		closed.push_back(false);
		step.push_back(-1);
	}
	
	while (true)
	{
		Phenix::Int32 minDistIdx = 0;
		bool found = false; // �Ƿ��ҵ�open����������type����С����ڵ�
		WeightData min;
		
		for (Phenix::Int32 i=0; i<weights.size(); ++i)
		{
			if (closed[i])
			{
				continue;
			}
			if (dist[i] < min)
			{
				min			= dist[i];
				minDistIdx	= i;
				found		= true;
			}
		}
		if (!found)
		{
			break; // ���
		}
		closed[minDistIdx] = true;
		Edges::const_iterator iter = edges.find(minDistIdx);
		if (iter != edges.end())
		{
			const Neighbors& neighbors = iter->second;
			Neighbors::const_iterator itr = neighbors.begin();
			for (; itr != neighbors.end(); ++itr)
			{
				if (closed[*itr])
				{
					continue;
				}
				WeightData tmp = dist[minDistIdx] + weights[minDistIdx][*itr];
				if (minDistIdx == startIdx || tmp < dist[*itr])
				{
					dist[*itr] = tmp;
					step[*itr] = minDistIdx;
				}				
			}
		}
	}	

	// ��step����path
	paths.clear();

	for (Phenix::Int32 i=0; i<step.size(); ++i)
	{
		if (step[i] == -1 || i == startIdx)
		{
			continue;
		}
		Path<NODE, WEIGHT> path;
		path.totalWeight = dist[i].value;
		NODE finish;
		if (!_graph.getObj(i, finish))
		{
			throw;
		}		
		path.nodes.push_back(finish);
		Phenix::Int32 preStep = step[i];
		while (preStep != startIdx) 
		{
			NODE node;
			if (!_graph.getObj(preStep, node))
			{
				throw;
			}
			path.nodes.push_back(node);
			preStep = step[preStep];
		}
		path.nodes.push_back(start);
		std::reverse(path.nodes.begin(), path.nodes.end());
		paths[finish] = path;
	}
}

// ���������㷨
template<typename NODE, typename WEIGHT>
class Floyd
{
public:	
	Floyd(const GraphBase<NODE, WEIGHT>&	graph):_graph(graph){}
	virtual ~Floyd(){}

	void operator ()(std::map<NODE, std::map<NODE, Path<NODE, WEIGHT>>>& paths);

private:
	void calcPath(std::vector<NODE>& nodes, Phenix::Int32 start, Phenix::Int32 finish, 
		Phenix::Int32 oriFinish, std::vector<std::vector<Phenix::Int32>>& step);

private:
	const GraphBase<NODE, WEIGHT>&	_graph;	
};

template<typename NODE, typename WEIGHT>
void Phenix::Graph::Floyd<NODE, WEIGHT>::calcPath( std::vector<NODE>& nodes, Phenix::Int32 start, 
		Phenix::Int32 finish, Phenix::Int32 oriFinish, std::vector<std::vector<Phenix::Int32>>& step )
{
	int midStep = step[start][finish];
	if (midStep == -1)
	{		
		return;
	}
	if (midStep != start)
	{
		calcPath(nodes, start, midStep, oriFinish, step);
		calcPath(nodes, midStep, finish, oriFinish, step);
		return;
	}
	NODE startNode;
	if (!_graph.getObj(start, startNode))
	{
		throw;
	}
	nodes.push_back(startNode);
	if (finish == oriFinish)
	{
		NODE finishNode;
		if (!_graph.getObj(oriFinish, finishNode))
		{
			throw;
		}
		nodes.push_back(finishNode);
	}	
}

template<typename NODE, typename WEIGHT>
void Phenix::Graph::Floyd<NODE, WEIGHT>::operator()( std::map<NODE, std::map<NODE, Path<NODE, WEIGHT>>>& paths )
{
	typedef GraphBase<NODE, WEIGHT>::Weights	Weights;
	typedef GraphBase<NODE, WEIGHT>::Edges		Edges;
	typedef GraphBase<NODE, WEIGHT>::Neighbors	Neighbors;
	typedef GraphBase<NODE, WEIGHT>::WeightData	WeightData;
	
	const Edges& edges	= _graph.getEdges();
	Weights weightsCopy	= _graph.getWeights();
	Phenix::Int32 num = weightsCopy.size();
	// ��ʼ��step
	std::vector<std::vector<Phenix::Int32>>	step;
	for (Phenix::Int32 i=0; i<num; ++i)
	{
		std::vector<Phenix::Int32> temp;		
		for (Phenix::Int32 j=0; j<num; ++j)
		{			
			if (weightsCopy[i][j].type == NORMAL)
			{
				temp.push_back(i);
			}
			else
			{
				temp.push_back(-1);
			}			
		}
		step.push_back(temp);
	}

	// ����step����С·������
	for (Phenix::Int32 k=0; k<num; ++k)
	{
		for (Phenix::Int32 i=0; i<num; ++i)
		{
			for (Phenix::Int32 j=0; j<num; ++j)
			{
				WeightData sum = weightsCopy[i][k] + weightsCopy[k][j];
				if (sum < weightsCopy[i][j])
				{
					weightsCopy[i][j] = sum;
					step[i][j] = k;
				}
			}
		}
	}

	// ��step�Ƶ�path
	paths.clear();
	for (Phenix::Int32 i=0; i<num; ++i)
	{
		std::map<NODE, Path<NODE, WEIGHT>> pathList;
		for (Phenix::Int32 j=0; j<num; ++j)
		{
			if (step[i][j] == -1)
			{
				continue;
			}
			Path<NODE, WEIGHT> path;
			path.totalWeight = weightsCopy[i][j].value;			
			calcPath(path.nodes, i, j, j, step);
			pathList[j] = path;			
		}
		if (!pathList.empty())
		{		
			NODE node;
			if (!_graph.getObj(i, node))
			{
				throw;
			}
			paths[node] = pathList;
		}
	}
}

}
}


#endif // PHENIX_GRAPH_MIN_PATH_H