#include <Phenix/Graph/UndirectedGraph.h>
#include <Phenix/Graph/DirectedGraph.h>

/*
用例说明：图的有关操作
*/

using namespace Phenix::Graph;

// 无向图
void friends1()
{
	/*
	 1<=>2 好友度10
	 1<=>3 好友度15
	 2<=>4 好友度20
	 3<=>5 好友度30
	*/	
	UndirectedGraph<Phenix::UInt32, Phenix::UInt32> ug;
	
	ug.addEdge(1,2,10);	
	ug.addEdge(1,3,15);	
	ug.addEdge(2,4,20);	
	ug.addEdge(3,5,30);
	Phenix::UInt32 w = 0;
	if (ug.getWeight(2,4,w))
	{
		std::cout<<"2-4:"<<w<<std::endl;
	}
	if (ug.getWeight(4,2,w))
	{
		std::cout<<"4-2:"<<w<<std::endl;
	}
	if (ug.getWeight(3,1,w))
	{
		std::cout<<"3-1:"<<w<<std::endl;
	}
	if (!ug.getWeight(1,5,w))
	{
		std::cout<<"1-5:no"<<std::endl;
	}
	if (!ug.getWeight(2,3,w))
	{
		std::cout<<"2-3:no"<<std::endl;
	}
	ug.removeEdge(3,1);
	if (!ug.getWeight(1,3,w))
	{
		std::cout<<"1-3:no"<<std::endl;
	}
	ug.removeNode(2);
	if (!ug.getWeight(1,2,w))
	{
		std::cout<<"1-2:no"<<std::endl;
	}
	if (!ug.getWeight(2,4,w))
	{
		std::cout<<"4-2:no"<<std::endl;
	}
}

// 有向图
void friends2()
{
	/*
	 1=>2 好友度10
	 2=>1 好友度5
	 1=>3 好友度15
	 3=>2 好友度1
	 2=>4 好友度20
	 3=>5 好友度30
	*/	
	DirectedGraph<Phenix::UInt32, Phenix::UInt32> dg;
	
	dg.addEdge(1,2,10);
	dg.addEdge(2,1,5);
	dg.addEdge(1,3,15);	
	dg.addEdge(3,2,1);	
	dg.addEdge(2,4,20);	
	dg.addEdge(3,5,30);
	Phenix::UInt32 w = 0;
	if (dg.getWeight(2,4,w) && w == 20)
	{
		std::cout<<"2-4:yes"<<std::endl;
	}
	if (!dg.getWeight(4,2,w))
	{
		std::cout<<"4-2:yes"<<std::endl;
	}
	if (dg.getWeight(1,3,w) && w == 15)
	{
		std::cout<<"3-1:yes"<<std::endl;
	}
	if (!dg.getWeight(1,5,w))
	{
		std::cout<<"1-5:yes"<<std::endl;
	}
	if (dg.getWeight(1,2,w) && w == 10)
	{
		std::cout<<"1-2:yes"<<std::endl;
	}
	if (dg.getWeight(2,1,w) && w == 5)
	{
		std::cout<<"2-1:yes"<<std::endl;
	}

	dg.removeEdge(3,1);
	if (dg.getWeight(1,3,w) && w == 15 && !dg.getWeight(3,1,w))
	{
		std::cout<<"remove 3-1:yes"<<std::endl;
	}
	dg.removeNode(2);
	if (!dg.getWeight(1,2,w) && !dg.getWeight(2,1,w) && !dg.getWeight(3,2,w) && !dg.getWeight(2,4,w))
	{
		std::cout<<"remove 2:yes"<<std::endl;
	}
}

void graphTest(int argc, char* argv[])
{		
	friends1();
	friends2();
}

