#include <Phenix/Collection/UnionFind.h>

/*
用例说明：用并查集辨别人物关系
*/

void unionFindTest(int argc, char* argv[])
{	
	using namespace Phenix::Collection;
	UnionFind<Phenix::String>	uf;
	Phenix::String liuxin = "刘新";
	Phenix::String liuzhiyong = "刘志勇";
	Phenix::String liujun = "刘军";
	Phenix::String liujian = "刘建";
	Phenix::String liuzhenting = "刘振廷";
	Phenix::String luoqiongling = "罗琼玲";
	Phenix::String guohuiyue = "郭辉玥";
	Phenix::String zhangsan = "张三";
	Phenix::String lisi = "李四";

	uf.addRel(liuxin, liuzhiyong);
	uf.addRel(liuxin,liuzhenting);
	uf.addRel(liuxin,liujun);
	uf.addRel(liujian,liujun);
	uf.addRel(luoqiongling,liujun);
	uf.addRel(luoqiongling,liuzhenting);
	uf.addRel(guohuiyue,liujun);
	uf.addRel(zhangsan,lisi);

	std::cout<<uf.isRel(liuzhiyong,guohuiyue)<<std::endl;	
	std::cout<<uf.isRel(liuxin,luoqiongling)<<std::endl;
	std::cout<<uf.isRel(liujian,luoqiongling)<<std::endl;	
	std::cout<<uf.isRel(liuzhiyong,liujian)<<std::endl;
	std::cout<<uf.isRel(liuzhiyong,luoqiongling)<<std::endl;
	std::cout<<uf.isRel(zhangsan,lisi)<<std::endl;
	std::cout<<uf.isRel(liuzhiyong,liuzhenting)<<std::endl;	
	std::cout<<uf.isRel(guohuiyue,liuzhenting)<<std::endl;	
	
	std::cout<<uf.isRel(liuzhiyong,zhangsan)<<std::endl;
	std::cout<<uf.isRel(liuzhiyong,lisi)<<std::endl;
}

