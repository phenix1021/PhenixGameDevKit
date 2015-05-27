// delegate.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <Phenix/Base/Types.h>
#include <Phenix/Base/Delegate.h>
#include <Phenix/Base/SharedPtr.h>

using Phenix::Base::SharedPtr;
using Phenix::Base::Function;
using Phenix::Base::Bind;

void say0(){std::cout<<"hello world"<<std::endl;}
void say1(Phenix::Int32 i){std::cout<<i<<std::endl;}
void say2(Phenix::Int32 i, Phenix::String j){std::cout<<i<<" "<<j<<std::endl;}
void say3(Phenix::Int32 i, float j, Phenix::String k){std::cout<<i<<" "<<j<<" "<<k<<std::endl;}

class TFunc
{
public:
	TFunc(){std::cout<<"start"<<std::endl;}
	virtual ~TFunc(){std::cout<<"finished"<<std::endl;}
	void speak0(){std::cout<<"hello world"<<std::endl;}
	void speak1(Phenix::Int32 i){std::cout<<i<<std::endl;}
	void speak2(Phenix::Int32 i, Phenix::String& j){std::cout<<i<<" "<<j<<std::endl;}
	void speak3(Phenix::Int32 i, float j, Phenix::String k){std::cout<<i<<" "<<j<<" "<<k<<std::endl;}
};
class Father
{
public:
	virtual ~Father()
	{
		std::cout<<" father over "<<std::endl;
	}
};
class Son
	:public Father
{
public:
	~Son()
	{
		std::cout<<" son over "<<std::endl;
	}

};
class Box
{
public:
	Box(){}
	Box(Father* f):p(f){}
	~Box()
	{
		std::cout<<" box over, ref count "<<p.GetRefCount()<<std::endl;
	}
	SharedPtr<Father> p;
};

int _tmain(int argc, _TCHAR* argv[])
{
	{
		Function<void()> func2;
	SharedPtr<TFunc> tf(new TFunc());
	//TFunc* tf = new TFunc;
	
 		Function<void()> func1 = Bind(&TFunc::speak0, tf);
		std::cout<<tf.GetRefCount()<<std::endl;
 		func2 = func1;		
	
	//Phenix::String ss =	"hello";
	func2();

// 	{
// 		Box b1(new Son);
//  		Box b2,b3;
//  		b2 = b1;
// 		b1 = b2;
// 		b3 = b1;
// 	}	
	std::cout<<tf.GetRefCount()<<std::endl;
	}
	/*{	
	TFunc* f1 = new TFunc();
	TFunc* f2 = new TFunc();
	SharedPtr<TFunc> tf1(f1);
	SharedPtr<TFunc> tf2(f2);
	tf1 = tf2;
	}*/

	getchar();
	return 0;
}

