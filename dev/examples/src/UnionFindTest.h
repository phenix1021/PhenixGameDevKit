#include <Phenix/Collection/UnionFind.h>

/*
����˵�����ò��鼯��������ϵ
*/

void unionFindTest(int argc, char* argv[])
{	
	using namespace Phenix::Collection;
	UnionFind<Phenix::String>	uf;
	Phenix::String liuxin = "����";
	Phenix::String liuzhiyong = "��־��";
	Phenix::String liujun = "����";
	Phenix::String liujian = "����";
	Phenix::String liuzhenting = "����͢";
	Phenix::String luoqiongling = "������";
	Phenix::String guohuiyue = "���ԫh";
	Phenix::String zhangsan = "����";
	Phenix::String lisi = "����";

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

