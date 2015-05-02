/*******************************************************************************
* @brief	仿boost::bind, boost::function
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_DELEGATE_H
#define PHENIX_BASE_DELEGATE_H

#include <Phenix/Base/SharedPtr.h>

namespace Phenix
{
namespace Base
{
	// Function基类模板
	template<typename Signature>
	class Function;

	// 前置引用
	template<typename Ret, typename Arg1> class FunctionImplBase1;	

	// Function特化类模板
	template<typename Ret, typename Arg1>
	class Function<Ret(Arg1)>
	{		
		typedef SharedPtr<FunctionImplBase1<Ret, Arg1>>	FunctionImplBasePtr1;

	public:		
		Function():m_func_impl(NULL){}		
		Function(FunctionImplBase1<Ret, Arg1>* func):m_func_impl(func){}

		virtual ~Function(){}
		
		Ret operator () (Arg1 arg1){return (*m_func_impl)(arg1);}

	private:
		FunctionImplBasePtr1 m_func_impl;
	};

	template<typename Ret, typename Arg1>
	class FunctionImplBase1
	{		
	public:
		virtual Ret operator () (Arg1 arg1) = 0;
	};

	// for 全局函数、静态函数
	template<typename Ret, typename Arg1>
	class FunctionImplFunc1
		:public FunctionImplBase1<Ret, Arg1>
	{		
		typedef Ret(*CallBack)(Arg1);
	public:
		FunctionImplFunc1(CallBack& call_back):m_call_back(call_back){}
		Ret operator () (Arg1 arg1){return m_call_back(arg1);}
	private:
		CallBack m_call_back;
	};

	// for 类成员函数
	template<typename Ret, typename Arg1, typename Cls, typename Obj>
	class FunctionImplMethod1
		:public FunctionImplBase1<Ret, Arg1>
	{		
		typedef Ret(Cls::*CallBack)(Arg1);
	public:
		FunctionImplMethod1(CallBack& call_back, Obj obj)
			:m_obj(obj), m_call_back(call_back){}
		Ret operator () (Arg1 arg1)
		{
			return (m_obj->*m_call_back)(arg1);
		}
	private:
		Obj		 m_obj;
		CallBack m_call_back;
	};

	template<typename Ret, typename Arg1>
	FunctionImplFunc1<Ret, Arg1>* Bind(Ret(*CallBack)(Arg1))
	{
		return new FunctionImplFunc1<Ret, Arg1>(CallBack);
	}

	template<typename Ret, typename Arg1, typename Cls, typename Obj>
	FunctionImplMethod1<Ret, Arg1, Cls, Obj>* Bind(Ret(Cls::*CallBack)(Arg1), Obj obj)
	{
		return new FunctionImplMethod1<Ret, Arg1, Cls, Obj>(CallBack, obj);
	}

} // end namespace Base
} // end namespace Phenix


#endif