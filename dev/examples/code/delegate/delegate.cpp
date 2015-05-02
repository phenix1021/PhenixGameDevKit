// delegate.cpp : Defines the entry point for the console application.
//

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
void say2(Phenix::Int32 i, Phenix::String& j){std::cout<<i<<" "<<j<<std::endl;}
void say3(Phenix::Int32 i, float j, Phenix::String& k){std::cout<<i<<" "<<j<<" "<<k<<std::endl;}

class TFunc
{
public:
	void speak0(){std::cout<<"hello world"<<std::endl;}
	void speak1(Phenix::Int32 i){std::cout<<i<<std::endl;}
	void speak2(Phenix::Int32 i, Phenix::String& j){std::cout<<i<<" "<<j<<std::endl;}
	void speak3(Phenix::Int32 i, float j, Phenix::String& k){std::cout<<i<<" "<<j<<" "<<k<<std::endl;}
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
	Function<void(Phenix::Int32)> func2;
		TFunc tf;
		{
			Function<void(Phenix::Int32)> func1 = Bind(&TFunc::speak1, &tf);
			func2 = func1;
		}
		
		func2(10);

// 	{
// 		Box b1(new Son);
//  		Box b2,b3;
//  		b2 = b1;
// 		b1 = b2;
// 		b3 = b1;
// 	}	

	getchar();
	return 0;
}

