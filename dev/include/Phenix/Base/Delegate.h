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

	// ----------------------无参数---------------------
	// 前置引用
	template<typename Ret> class FunctionImplBase0;	

	// Function特化类模板
	template<typename Ret>
	class Function<Ret(void)>
	{		
		typedef SharedPtr<FunctionImplBase0<Ret>>	FunctionImplBasePtr0;

	public:		
		Function():m_func_impl(NULL){}		
		Function(FunctionImplBase0<Ret>* func):m_func_impl(func){}

		virtual ~Function(){}

		Ret operator () (void){return (*m_func_impl)();}

	private:
		FunctionImplBasePtr0 m_func_impl;
	};

	template<typename Ret>
	class FunctionImplBase0
	{		
	public:
		virtual ~FunctionImplBase0(){}
		virtual Ret operator () (void) = 0;
	};

	// for 全局函数、静态函数
	template<typename Ret>
	class FunctionImplFunc0
		:public FunctionImplBase0<Ret>
	{		
		typedef Ret(*CallBack)(void);
	public:
		FunctionImplFunc0(CallBack& call_back):m_call_back(call_back){}
		Ret operator () (void){return m_call_back();}
	private:
		CallBack m_call_back;
	};

	// for 类成员函数
	template<typename Ret, typename Cls, typename Obj>
	class FunctionImplMethod0
		:public FunctionImplBase0<Ret>
	{		
		typedef Ret(Cls::*CallBack)(void);
	public:
		FunctionImplMethod0(CallBack& call_back, Obj obj)
			:m_obj(obj), m_call_back(call_back){}
		Ret operator () (void)
		{
			return (&*m_obj->*m_call_back)();	// 为了支持智能指针才写成&*m_obj，否则即使SharedPtr重载了->也会报错
		}
	private:
		Obj		 m_obj;
		CallBack m_call_back;
	};

	template<typename Ret>
	FunctionImplFunc0<Ret>* Bind(Ret(*CallBack)(void))
	{
		return new FunctionImplFunc0<Ret>(CallBack);
	}

	template<typename Ret, typename Cls, typename Obj>
	FunctionImplMethod0<Ret, Cls, Obj>* Bind(Ret(Cls::*CallBack)(void), Obj obj)
	{
		return new FunctionImplMethod0<Ret, Cls, Obj>(CallBack, obj);
	}

	// ----------------------1~10个参数---------------------
#define SUPPORT_FUNC_PARAM_COUNT_MAX(N) SUPPORT_FUNC_PARAM_COUNT_MAX_##N
#define SUPPORT_FUNC_PARAM_COUNT_MAX_1	FUNCTION(1)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_2	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_1		\
	FUNCTION(2)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_3	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_2		\
	FUNCTION(3)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_4	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_3		\
	FUNCTION(4)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_5	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_4		\
	FUNCTION(5)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_6	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_5		\
	FUNCTION(6)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_7	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_6		\
	FUNCTION(7)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_8	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_7		\
	FUNCTION(8)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_9	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_8		\
	FUNCTION(9)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_10	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_9		\
	FUNCTION(10)

#define TYPENAME_CALL_ARG1		\
	typename CallArg1
#define TYPENAME_CALL_ARG2		\
	TYPENAME_CALL_ARG1, typename CallArg2
#define TYPENAME_CALL_ARG3		\
	TYPENAME_CALL_ARG2, typename CallArg3
#define TYPENAME_CALL_ARG4		\
	TYPENAME_CALL_ARG3, typename CallArg4
#define TYPENAME_CALL_ARG5		\
	TYPENAME_CALL_ARG4, typename CallArg5
#define TYPENAME_CALL_ARG6		\
	TYPENAME_CALL_ARG5, typename CallArg6
#define TYPENAME_CALL_ARG7		\
	TYPENAME_CALL_ARG6, typename CallArg7
#define TYPENAME_CALL_ARG8		\
	TYPENAME_CALL_ARG7, typename CallArg8
#define TYPENAME_CALL_ARG9		\
	TYPENAME_CALL_ARG8, typename CallArg9
#define TYPENAME_CALL_ARG10		\
	TYPENAME_CALL_ARG9, typename CallArg10

#define CALL_ARG1		\
	CallArg1
#define CALL_ARG2		\
	CALL_ARG1, CallArg2
#define CALL_ARG3		\
	CALL_ARG2, CallArg3
#define CALL_ARG4		\
	CALL_ARG3, CallArg4
#define CALL_ARG5		\
	CALL_ARG4, CallArg5
#define CALL_ARG6		\
	CALL_ARG5, CallArg6
#define CALL_ARG7		\
	CALL_ARG6, CallArg7
#define CALL_ARG8		\
	CALL_ARG7, CallArg8
#define CALL_ARG9		\
	CALL_ARG8, CallArg9
#define CALL_ARG10		\
	CALL_ARG9, CallArg10

#define CALL_ARG_PARAM1		\
	CallArg1	arg1
#define CALL_ARG_PARAM2		\
	CALL_ARG_PARAM1, CallArg2  arg2
#define CALL_ARG_PARAM3		\
	CALL_ARG_PARAM2, CallArg3  arg3
#define CALL_ARG_PARAM4		\
	CALL_ARG_PARAM3, CallArg4  arg4
#define CALL_ARG_PARAM5		\
	CALL_ARG_PARAM4, CallArg5  arg5
#define CALL_ARG_PARAM6		\
	CALL_ARG_PARAM5, CallArg6  arg6
#define CALL_ARG_PARAM7		\
	CALL_ARG_PARAM6, CallArg7  arg7
#define CALL_ARG_PARAM8		\
	CALL_ARG_PARAM7, CallArg8  arg8
#define CALL_ARG_PARAM9		\
	CALL_ARG_PARAM8, CallArg9  arg9
#define CALL_ARG_PARAM10	\
	CALL_ARG_PARAM9, CallArg10  arg10

#define CALL_PARAM1		\
	arg1
#define CALL_PARAM2		\
	CALL_PARAM1, arg2
#define CALL_PARAM3		\
	CALL_PARAM2, arg3
#define CALL_PARAM4		\
	CALL_PARAM3, arg4
#define CALL_PARAM5		\
	CALL_PARAM4, arg5
#define CALL_PARAM6		\
	CALL_PARAM5, arg6
#define CALL_PARAM7		\
	CALL_PARAM6, arg7
#define CALL_PARAM8		\
	CALL_PARAM7, arg8
#define CALL_PARAM9		\
	CALL_PARAM8, arg9
#define CALL_PARAM10	\
	CALL_PARAM9, arg10

#define FUNCTION(N)																							\
	template<typename Ret, TYPENAME_CALL_ARG##N> class FunctionImplBase##N;									\
	template<typename Ret, TYPENAME_CALL_ARG##N>															\
	class Function<Ret(CALL_ARG##N)>																		\
	{																										\
		typedef SharedPtr<FunctionImplBase##N<Ret, CALL_ARG##N>>	FunctionImplBasePtr##N;					\
	public:																									\
		Function():m_func_impl(NULL){}																		\
		Function(FunctionImplBase##N<Ret, CALL_ARG##N>* func):m_func_impl(func){}							\
		virtual ~Function(){}																				\
		Ret operator () (CALL_ARG_PARAM##N){return (*m_func_impl)(CALL_PARAM##N);}							\
	private:																								\
		FunctionImplBasePtr##N m_func_impl;																	\
	};																										\
	template<typename Ret, TYPENAME_CALL_ARG##N>															\
	class FunctionImplBase##N																				\
	{																										\
	public:																									\
		virtual ~FunctionImplBase##N(){}																	\
		virtual Ret operator () (CALL_ARG_PARAM##N) = 0;													\
	};																										\
	template<typename Ret, TYPENAME_CALL_ARG##N>															\
	class FunctionImplFunc##N																				\
		:public FunctionImplBase##N<Ret, CALL_ARG##N>														\
	{																										\
		typedef Ret(*CallBack)(CALL_ARG##N);																\
	public:																									\
		FunctionImplFunc##N(CallBack& call_back):m_call_back(call_back){}									\
		Ret operator () (CALL_ARG_PARAM##N){return m_call_back(CALL_PARAM##N);}								\
	private:																								\
		CallBack m_call_back;																				\
	};																										\
	template<typename Ret, TYPENAME_CALL_ARG##N, typename Cls, typename Obj>								\
	class FunctionImplMethod##N																				\
		:public FunctionImplBase##N<Ret, CALL_ARG##N>														\
	{																										\
		typedef Ret(Cls::*CallBack)(CALL_ARG##N);															\
	public:																									\
		FunctionImplMethod##N(CallBack& call_back, Obj obj)													\
			:m_obj(obj), m_call_back(call_back){}															\
		Ret operator () (CALL_ARG_PARAM##N)																	\
		{																									\
			return (&*m_obj->*m_call_back)(CALL_PARAM##N);													\
		}																									\
	private:																								\
		Obj		 m_obj;																						\
		CallBack m_call_back;																				\
	};																										\
	template<typename Ret, TYPENAME_CALL_ARG##N>															\
	FunctionImplFunc##N<Ret, CALL_ARG##N>* Bind(Ret(*CallBack)(CALL_ARG##N))								\
	{																										\
		return new FunctionImplFunc##N<Ret, CALL_ARG##N>(CallBack);											\
	}																										\
	template<typename Ret, TYPENAME_CALL_ARG##N, typename Cls,	typename Obj>								\
	FunctionImplMethod##N<Ret, CALL_ARG##N, Cls, Obj>* Bind(Ret(Cls::*CallBack)(CALL_ARG##N), Obj obj)		\
	{																										\
		return new FunctionImplMethod##N<Ret, CALL_ARG##N, Cls, Obj>(CallBack, obj);						\
	}

	// 定义最多支持参数个数
	SUPPORT_FUNC_PARAM_COUNT_MAX(10)

/*********************宏展开后单参示例******************************
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
*******************************************************************/

} // end namespace Base
} // end namespace Phenix


#endif

/*********以下实现了占位符，但是未能支持bind的函数参数个数不同于function的签名***********

struct PlaceHolder1{} _1;
struct PlaceHolder2{} _2;
struct PlaceHolder3{} _3;
struct PlaceHolder4{} _4;
struct PlaceHolder5{} _5;
struct PlaceHolder6{} _6;
struct PlaceHolder7{} _7;

template<typename CallArgType1>
struct StorageArgsBase1
{
	virtual CallArgType1& GetArg1() = 0;
	CallArgType1 call_arg1;
};

template<typename BindArgType1, typename CallArgType1>
struct StorageArgs1
	:public StorageArgsBase1<CallArgType1>
{
	StorageArgs1(BindArgType1 p1):bind_arg1(p1){}

	inline CallArgType1& GetArg1()
	{
		return GetArg1_Impl(bind_arg1);
	}

	inline CallArgType1& GetArg1_Impl(PlaceHolder1)
	{
		return call_arg1;
	}

	inline CallArgType1& GetArg1_Impl(CallArgType1)
	{
		return bind_arg1;
	}

	BindArgType1 bind_arg1;	
};


// 前置引用
template<typename ReturnType, typename CallArgType1> class FunctionImplBase1;

// Function基类模板
template<typename Signature>
class Function;

// Function特化基类模板
template<typename ReturnType, typename CallArgType1>
class Function<ReturnType(CallArgType1)>
{	
public:
	Function(FunctionImplBase1<ReturnType, CallArgType1>* func, StorageArgsBase1<CallArgType1>* storage1)
		:m_func_impl(func), m_storage1(storage1)
	{
		
	}

	virtual ~Function()
	{
		delete m_func_impl; 
		delete m_storage1;
	}
	
	ReturnType operator () (CallArgType1 arg1)
	{		
		m_storage1->call_arg1 = arg1;		
		(*m_func_impl)(m_storage1->GetArg1()); // 也可以写成m_func_impl->operator ()(m_storage1->GetArg1());
	}
private:
	FunctionImplBase1<ReturnType, CallArgType1>* m_func_impl;
	StorageArgsBase1<CallArgType1>*				m_storage1;
};

template<typename ReturnType, typename CallArgType1>
class FunctionImplBase1
{	
public:
	virtual ReturnType operator () (CallArgType1 arg1) = 0;
};

// 派生类：for 全局函数、静态函数
template<typename ReturnType, typename CallArgType1>
class FunctionImplFunc1
	:public FunctionImplBase1<ReturnType, CallArgType1>
{	
	typedef ReturnType(*CallBack)(CallArgType1);
public:
	FunctionImplFunc1(CallBack& call_back):m_call_back(call_back){}
	ReturnType operator () (CallArgType1 arg1){return m_call_back(arg1);}
private:
	CallBack m_call_back;
};


// 派生类：for 类成员函数(支持智能指针)
template<typename ReturnType, typename CallArgType1, typename ClassType, typename Obj>
class FunctionImplMethod1
	:public FunctionImplBase1<ReturnType, CallArgType1>
{	
	typedef ReturnType(ClassType::*CallBack)(CallArgType1);
public:
	FunctionImplMethod1(Obj& obj, CallBack& call_back)
		:m_obj(obj), m_call_back(call_back){}
	ReturnType operator () (CallArgType1 arg1)
	{
		return (m_obj->*m_call_back)(arg1);
	}
private:
	Obj		 m_obj;	// 区分ClassType和Obj，且此处用Obj而不是Obj*是为了让Obj支持智能指针
	CallBack m_call_back;
};

template<typename ReturnType, typename CallArgType1, typename BindArgType1>
Function<ReturnType(CallArgType1)> Bind(ReturnType(*CallBack)(CallArgType1), BindArgType1 bind_arg1)
{
	return Function<ReturnType(CallArgType1)>(new FunctionImplFunc1<ReturnType, CallArgType1>(CallBack), 
											  new StorageArgs1<BindArgType1, CallArgType1>(bind_arg1));
}

template<typename ReturnType, typename CallArgType1, typename ClassType, typename Obj, typename BindArgType1>
Function<ReturnType(CallArgType1)> Bind(ReturnType(ClassType::*CallBack)(CallArgType1), Obj obj, BindArgType1 bind_arg1)
{
	return Function<ReturnType(CallArgType1)>(new FunctionImplMethod1<ReturnType, CallArgType1, ClassType, Obj>(obj, CallBack), 
											  new StorageArgs1<BindArgType1, CallArgType1>(bind_arg1));
}

// 整理成宏：

#define SUPPORT_FUNC_PARAM_COUNT_MAX(N) SUPPORT_FUNC_PARAM_COUNT_MAX_##N
#define SUPPORT_FUNC_PARAM_COUNT_MAX_1	FUNCTION(1)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_2	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_1		\
	FUNCTION(2)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_3	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_2		\
	FUNCTION(3)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_4	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_3		\
	FUNCTION(4)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_5	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_4		\
	FUNCTION(5)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_6	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_5		\
	FUNCTION(6)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_7	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_6		\
	FUNCTION(7)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_8	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_7		\
	FUNCTION(8)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_9	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_8		\
	FUNCTION(9)
#define SUPPORT_FUNC_PARAM_COUNT_MAX_10	\
	SUPPORT_FUNC_PARAM_COUNT_MAX_9		\
	FUNCTION(10)

#define TYPENAME_BIND_ARG_LIST_1		\
	typename BindArgType1
#define TYPENAME_BIND_ARG_LIST_2		\
	TYPENAME_BIND_ARG_LIST_1, typename BindArgType2
#define TYPENAME_BIND_ARG_LIST_3		\
	TYPENAME_BIND_ARG_LIST_2, typename BindArgType3
#define TYPENAME_BIND_ARG_LIST_4		\
	TYPENAME_BIND_ARG_LIST_3, typename BindArgType4
#define TYPENAME_BIND_ARG_LIST_5		\
	TYPENAME_BIND_ARG_LIST_4, typename BindArgType5
#define TYPENAME_BIND_ARG_LIST_6		\
	TYPENAME_BIND_ARG_LIST_5, typename BindArgType6
#define TYPENAME_BIND_ARG_LIST_7		\
	TYPENAME_BIND_ARG_LIST_6, typename BindArgType7
#define TYPENAME_BIND_ARG_LIST_8		\
	TYPENAME_BIND_ARG_LIST_7, typename BindArgType8
#define TYPENAME_BIND_ARG_LIST_9		\
	TYPENAME_BIND_ARG_LIST_8, typename BindArgType9
#define TYPENAME_BIND_ARG_LIST_10		\
	TYPENAME_BIND_ARG_LIST_9, typename BindArgType10

#define TYPENAME_CALL_ARG_LIST_1		\
	typename CallArgType1
#define TYPENAME_CALL_ARG_LIST_2		\
	TYPENAME_CALL_ARG_LIST_1, typename CallArgType2
#define TYPENAME_CALL_ARG_LIST_3		\
	TYPENAME_CALL_ARG_LIST_2, typename CallArgType3
#define TYPENAME_CALL_ARG_LIST_4		\
	TYPENAME_CALL_ARG_LIST_3, typename CallArgType4
#define TYPENAME_CALL_ARG_LIST_5		\
	TYPENAME_CALL_ARG_LIST_4, typename CallArgType5
#define TYPENAME_CALL_ARG_LIST_6		\
	TYPENAME_CALL_ARG_LIST_5, typename CallArgType6
#define TYPENAME_CALL_ARG_LIST_7		\
	TYPENAME_CALL_ARG_LIST_6, typename CallArgType7
#define TYPENAME_CALL_ARG_LIST_8		\
	TYPENAME_CALL_ARG_LIST_7, typename CallArgType8
#define TYPENAME_CALL_ARG_LIST_9		\
	TYPENAME_CALL_ARG_LIST_8, typename CallArgType9
#define TYPENAME_CALL_ARG_LIST_10		\
	TYPENAME_CALL_ARG_LIST_9, typename CallArgType10

#define BIND_ARG_LIST_1		\
	BindArgType1
#define BIND_ARG_LIST_2		\
	BIND_ARG_LIST_1, BindArgType2
#define BIND_ARG_LIST_3		\
	BIND_ARG_LIST_2, BindArgType3
#define BIND_ARG_LIST_4		\
	BIND_ARG_LIST_3, BindArgType4
#define BIND_ARG_LIST_5		\
	BIND_ARG_LIST_4, BindArgType5
#define BIND_ARG_LIST_6		\
	BIND_ARG_LIST_5, BindArgType6
#define BIND_ARG_LIST_7		\
	BIND_ARG_LIST_6, BindArgType7
#define BIND_ARG_LIST_8		\
	BIND_ARG_LIST_7, BindArgType8
#define BIND_ARG_LIST_9		\
	BIND_ARG_LIST_8, BindArgType9
#define BIND_ARG_LIST_10		\
	BIND_ARG_LIST_9, BindArgType10

#define CALL_ARG_LIST_1		\
	CallArgType1
#define CALL_ARG_LIST_2		\
	CALL_ARG_LIST_1, CallArgType2
#define CALL_ARG_LIST_3		\
	CALL_ARG_LIST_2, CallArgType3
#define CALL_ARG_LIST_4		\
	CALL_ARG_LIST_3, CallArgType4
#define CALL_ARG_LIST_5		\
	CALL_ARG_LIST_4, CallArgType5
#define CALL_ARG_LIST_6		\
	CALL_ARG_LIST_5, CallArgType6
#define CALL_ARG_LIST_7		\
	CALL_ARG_LIST_6, CallArgType7
#define CALL_ARG_LIST_8		\
	CALL_ARG_LIST_7, CallArgType8
#define CALL_ARG_LIST_9		\
	CALL_ARG_LIST_8, CallArgType9
#define CALL_ARG_LIST_10	\
	CALL_ARG_LIST_9, CallArgType10

#define BIND_ARG_PARAM_LIST_1		\
	BindArgType1	bind_arg1
#define BIND_ARG_PARAM_LIST_2		\
	BIND_ARG_PARAM_LIST_1, BindArgType2  bind_arg2
#define BIND_ARG_PARAM_LIST_3		\
	BIND_ARG_PARAM_LIST_2, BindArgType3  bind_arg3
#define BIND_ARG_PARAM_LIST_4		\
	BIND_ARG_PARAM_LIST_3, BindArgType4  bind_arg4
#define BIND_ARG_PARAM_LIST_5		\
	BIND_ARG_PARAM_LIST_4, BindArgType5  bind_arg5
#define BIND_ARG_PARAM_LIST_6		\
	BIND_ARG_PARAM_LIST_5, BindArgType6  bind_arg6
#define BIND_ARG_PARAM_LIST_7		\
	BIND_ARG_PARAM_LIST_6, BindArgType7  bind_arg7
#define BIND_ARG_PARAM_LIST_8		\
	BIND_ARG_PARAM_LIST_7, BindArgType8  bind_arg8
#define BIND_ARG_PARAM_LIST_9		\
	BIND_ARG_PARAM_LIST_8, BindArgType9  bind_arg9
#define BIND_ARG_PARAM_LIST_10		\
	BIND_ARG_PARAM_LIST_9, BindArgType10  bind_arg10

#define CALL_ARG_PARAM_LIST_1		\
	CallArgType1	arg1
#define CALL_ARG_PARAM_LIST_2		\
	CALL_ARG_PARAM_LIST_1, CallArgType2  arg2
#define CALL_ARG_PARAM_LIST_3		\
	CALL_ARG_PARAM_LIST_2, CallArgType3  arg3
#define CALL_ARG_PARAM_LIST_4		\
	CALL_ARG_PARAM_LIST_3, CallArgType4  arg4
#define CALL_ARG_PARAM_LIST_5		\
	CALL_ARG_PARAM_LIST_4, CallArgType5  arg5
#define CALL_ARG_PARAM_LIST_6		\
	CALL_ARG_PARAM_LIST_5, CallArgType6  arg6
#define CALL_ARG_PARAM_LIST_7		\
	CALL_ARG_PARAM_LIST_6, CallArgType7  arg7
#define CALL_ARG_PARAM_LIST_8		\
	CALL_ARG_PARAM_LIST_7, CallArgType8  arg8
#define CALL_ARG_PARAM_LIST_9		\
	CALL_ARG_PARAM_LIST_8, CallArgType9  arg9
#define CALL_ARG_PARAM_LIST_10		\
	CALL_ARG_PARAM_LIST_9, CallArgType10  arg10

#define BIND_PARAM_LIST_1		\
	bind_arg1
#define BIND_PARAM_LIST_2		\
	BIND_PARAM_LIST_1, bind_arg2
#define BIND_PARAM_LIST_3		\
	BIND_PARAM_LIST_2, bind_arg3
#define BIND_PARAM_LIST_4		\
	BIND_PARAM_LIST_3, bind_arg4
#define BIND_PARAM_LIST_5		\
	BIND_PARAM_LIST_4, bind_arg5
#define BIND_PARAM_LIST_6		\
	BIND_PARAM_LIST_5, bind_arg6
#define BIND_PARAM_LIST_7		\
	BIND_PARAM_LIST_6, bind_arg7
#define BIND_PARAM_LIST_8		\
	BIND_PARAM_LIST_7, bind_arg8
#define BIND_PARAM_LIST_9		\
	BIND_PARAM_LIST_8, bind_arg9
#define BIND_PARAM_LIST_10		\
	BIND_PARAM_LIST_9, bind_arg10

#define PARAM_LIST_1		\
	arg1
#define PARAM_LIST_2		\
	PARAM_LIST_1, arg2
#define PARAM_LIST_3		\
	PARAM_LIST_2, arg3
#define PARAM_LIST_4		\
	PARAM_LIST_3, arg4
#define PARAM_LIST_5		\
	PARAM_LIST_4, arg5
#define PARAM_LIST_6		\
	PARAM_LIST_5, arg6
#define PARAM_LIST_7		\
	PARAM_LIST_6, arg7
#define PARAM_LIST_8		\
	PARAM_LIST_7, arg8
#define PARAM_LIST_9		\
	PARAM_LIST_8, arg9
#define PARAM_LIST_10		\
	PARAM_LIST_9, arg10

#define STORAGE_ARGS_BASE_MEMBER_1		\
	virtual CallArgType1& GetArg1() = 0;\
	CallArgType1 call_arg1;
#define STORAGE_ARGS_BASE_MEMBER_2		\
	STORAGE_ARGS_BASE_MEMBER_1			\
	virtual CallArgType2& GetArg2() = 0;\
	CallArgType2 call_arg2;
#define STORAGE_ARGS_BASE_MEMBER_3		\
	STORAGE_ARGS_BASE_MEMBER_2			\
	virtual CallArgType3& GetArg3() = 0;\
	CallArgType3 call_arg3;
#define STORAGE_ARGS_BASE_MEMBER_4		\
	STORAGE_ARGS_BASE_MEMBER_3			\
	virtual CallArgType4& GetArg4() = 0;\
	CallArgType4 call_arg4;
#define STORAGE_ARGS_BASE_MEMBER_5		\
	STORAGE_ARGS_BASE_MEMBER_4			\
	virtual CallArgType5& GetArg5() = 0;\
	CallArgType5 call_arg5;
#define STORAGE_ARGS_BASE_MEMBER_6		\
	STORAGE_ARGS_BASE_MEMBER_5			\
	virtual CallArgType6& GetArg6() = 0;\
	CallArgType6 call_arg6;
#define STORAGE_ARGS_BASE_MEMBER_7		\
	STORAGE_ARGS_BASE_MEMBER_6			\
	virtual CallArgType7& GetArg7() = 0;\
	CallArgType7 call_arg7;
#define STORAGE_ARGS_BASE_MEMBER_8		\
	STORAGE_ARGS_BASE_MEMBER_7			\
	virtual CallArgType8& GetArg8() = 0;\
	CallArgType8 call_arg8;
#define STORAGE_ARGS_BASE_MEMBER_9		\
	STORAGE_ARGS_BASE_MEMBER_8			\
	virtual CallArgType9& GetArg9() = 0;\
	CallArgType9 call_arg9;
#define STORAGE_ARGS_BASE_MEMBER_10		\
	STORAGE_ARGS_BASE_MEMBER_9			\
	virtual CallArgType10& GetArg10() = 0;\
	CallArgType10 call_arg10;

#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_1	BindArgType1 p1
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_2	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_1, BindArgType2 p2
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_3	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_2, BindArgType3 p3
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_4	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_3, BindArgType4 p4
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_5	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_4, BindArgType5 p5
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_6	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_5, BindArgType6 p6
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_7	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_6, BindArgType7 p7
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_8	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_7, BindArgType8 p8
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_9	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_8, BindArgType9 p9
#define STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_10	\
	STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_9, BindArgType10 p10

#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_1		bind_arg1(p1)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_2		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_1, bind_arg2(p2)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_3		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_2, bind_arg3(p3)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_4		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_3, bind_arg4(p4)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_5		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_4, bind_arg5(p5)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_6		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_5, bind_arg6(p6)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_7		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_6, bind_arg7(p7)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_8		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_7, bind_arg8(p8)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_9		\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_8, bind_arg9(p9)
#define STORAGE_ARG_CONSTRUCTOR_INIT_LIST_10	\
	STORAGE_ARG_CONSTRUCTOR_INIT_LIST_9, bind_arg10(p10)

#define STORAGE_ARGS_MEMBER_1						\
	BindArgType1 bind_arg1;							\
	inline CallArgType1& GetArg1()					\
	{												\
		return GetArg1_Impl(bind_arg1);				\
	}												\
	inline CallArgType1& GetArg1_Impl(PlaceHolder1)	\
	{												\
		return call_arg1;							\
	}												\
	inline CallArgType1& GetArg1_Impl(CallArgType1)	\
	{												\
		return bind_arg1;							\
	}	
#define STORAGE_ARGS_MEMBER_2						\
	STORAGE_ARGS_MEMBER_1							\
	BindArgType2 bind_arg2;							\
	inline CallArgType2& GetArg2()					\
	{												\
		return GetArg2_Impl(bind_arg2);				\
	}												\
	inline CallArgType2& GetArg2_Impl(PlaceHolder2)	\
	{												\
		return call_arg2;							\
	}												\
	inline CallArgType2& GetArg2_Impl(CallArgType2)	\
	{												\
		return bind_arg2;							\
	}												\
	inline CallArgType1& GetArg1_Impl(PlaceHolder2)	\
	{												\
		return call_arg2;							\
	}												\
	inline CallArgType2& GetArg2_Impl(PlaceHolder1)	\
	{												\
		return call_arg1;							\
	}
#define STORAGE_ARGS_MEMBER_3						\
	STORAGE_ARGS_MEMBER_2							\
	inline CallArgType3& GetArg3()					\
	{												\
		return GetArg3_Impl(bind_arg3);				\
	}												\
	inline CallArgType1& GetArg2_Impl(PlaceHolder2)	\
	{												\
	return call_arg2;							\
	}												\
	inline CallArgType2& GetArg2_Impl(CallArgType2)	\
	{												\
	return bind_arg2;							\
	}												\
	BindArgType2 bind_arg2;							\
	inline CallArgType1& GetArg1_Impl(PlaceHolder2)	\
	{												\
	return call_arg2;								\
	}												\
	inline CallArgType1& GetArg2_Impl(PlaceHolder1)	\
	{												\
	return call_arg1;								\
	}


#define FUNCTION_OPERATOR_ASSIGN_1	\
	m_storage->call_arg1 = arg1;				
#define FUNCTION_OPERATOR_ASSIGN_2	\
	FUNCTION_OPERATOR_ASSIGN_1		\
	m_storage->call_arg2 = arg2;
#define FUNCTION_OPERATOR_ASSIGN_3	\
	FUNCTION_OPERATOR_ASSIGN_2		\
	m_storage->call_arg3 = arg3;
#define FUNCTION_OPERATOR_ASSIGN_4	\
	FUNCTION_OPERATOR_ASSIGN_3		\
	m_storage->call_arg4 = arg4;
#define FUNCTION_OPERATOR_ASSIGN_5	\
	FUNCTION_OPERATOR_ASSIGN_4		\
	m_storage->call_arg5 = arg5;
#define FUNCTION_OPERATOR_ASSIGN_6	\
	FUNCTION_OPERATOR_ASSIGN_5		\
	m_storage->call_arg6 = arg6;
#define FUNCTION_OPERATOR_ASSIGN_7	\
	FUNCTION_OPERATOR_ASSIGN_6		\
	m_storage->call_arg7 = arg7;
#define FUNCTION_OPERATOR_ASSIGN_8	\
	FUNCTION_OPERATOR_ASSIGN_7		\
	m_storage->call_arg8 = arg8;
#define FUNCTION_OPERATOR_ASSIGN_9	\
	FUNCTION_OPERATOR_ASSIGN_8		\
	m_storage->call_arg9 = arg9;
#define FUNCTION_OPERATOR_ASSIGN_10	\
	FUNCTION_OPERATOR_ASSIGN_9		\
	m_storage->call_arg10 = arg10;

#define FUNCTION_OPERATOR_GET_ARG_1	\
	m_storage->GetArg1()
#define FUNCTION_OPERATOR_GET_ARG_2	\
	FUNCTION_OPERATOR_GET_ARG_1,	\
	m_storage->GetArg2()
#define FUNCTION_OPERATOR_GET_ARG_3	\
	FUNCTION_OPERATOR_GET_ARG_2,	\
	m_storage->GetArg3()
#define FUNCTION_OPERATOR_GET_ARG_4	\
	FUNCTION_OPERATOR_GET_ARG_3,	\
	m_storage->GetArg4()
#define FUNCTION_OPERATOR_GET_ARG_5	\
	FUNCTION_OPERATOR_GET_ARG_4,	\
	m_storage->GetArg5()
#define FUNCTION_OPERATOR_GET_ARG_6	\
	FUNCTION_OPERATOR_GET_ARG_5,	\
	m_storage->GetArg6()
#define FUNCTION_OPERATOR_GET_ARG_7	\
	FUNCTION_OPERATOR_GET_ARG_6,	\
	m_storage->GetArg7()
#define FUNCTION_OPERATOR_GET_ARG_8	\
	FUNCTION_OPERATOR_GET_ARG_7,	\
	m_storage->GetArg8()
#define FUNCTION_OPERATOR_GET_ARG_9	\
	FUNCTION_OPERATOR_GET_ARG_8,	\
	m_storage->GetArg9()
#define FUNCTION_OPERATOR_GET_ARG_10\
	FUNCTION_OPERATOR_GET_ARG_9,	\
	m_storage->GetArg10()

#define STORAGE_ARGS_BASE(N)					\
	template<TYPENAME_CALL_ARG_LIST_##N>		\
	struct StorageArgsBase##N					\
	{											\
		STORAGE_ARGS_BASE_MEMBER_##N			\
	};

#define STORAGE_ARGS(N)													\
	template<TYPENAME_BIND_ARG_LIST_##N, TYPENAME_CALL_ARG_LIST_##N>	\
	struct StorageArgs##N												\
		:public StorageArgsBase##N<CALL_ARG_LIST_##N>					\
	{																	\
		StorageArgs##N(STORAGE_ARG_CONSTRUCTOR_PARAM_LIST_##N)			\
			:STORAGE_ARG_CONSTRUCTOR_INIT_LIST_##N{}					\
		STORAGE_ARGS_MEMBER_##N											\
	};

template<typename Signature>
class Function;

#define FUNCTION(N)																			\
	STORAGE_ARGS_BASE(N)																	\
	STORAGE_ARGS(N)																			\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N> class FunctionImplBase##N;	\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N>								\
	class Function<ReturnType(CALL_ARG_LIST_##N)>											\
	{																						\
	public:																					\
		Function(FunctionImplBase##N<ReturnType, CALL_ARG_LIST_##N>* func,					\
				StorageArgsBase##N<CALL_ARG_LIST_##N>* storage)								\
				:m_func_imp##N(func), m_storage(storage){}									\
		virtual ~Function()																	\
		{																					\
			delete m_func_imp##N;															\
			delete m_storage;																\
		}																					\
		ReturnType operator () (CALL_ARG_PARAM_LIST_##N)									\
		{																					\
			FUNCTION_OPERATOR_ASSIGN_##N													\
			(*m_func_imp##N)(FUNCTION_OPERATOR_GET_ARG_##N);								\
		}																					\
	private:																				\
		FunctionImplBase##N<ReturnType, CALL_ARG_LIST_##N>* m_func_imp##N;					\
		StorageArgsBase##N<CALL_ARG_LIST_##N>*	m_storage;									\
	};																						\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N>								\
	class FunctionImplBase##N																\
	{																						\
	public:																					\
		virtual ReturnType operator () (CALL_ARG_PARAM_LIST_##N) = 0;						\
	};																						\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N>								\
	class FunctionImplFunc##N																\
		:public FunctionImplBase##N<ReturnType, CALL_ARG_LIST_##N>							\
	{																						\
		typedef ReturnType(*CallBack)(CALL_ARG_LIST_##N);									\
	public:																					\
		FunctionImplFunc##N(CallBack& call_back):m_call_back(call_back){}					\
		ReturnType operator () (CALL_ARG_PARAM_LIST_##N)									\
		{return m_call_back(PARAM_LIST_##N);}												\
	private:																				\
		CallBack m_call_back;																\
	};																						\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N,								\
			typename ClassType, typename Obj>												\
	class FunctionImplMethod##N																\
		:public FunctionImplBase##N<ReturnType, CALL_ARG_LIST_##N>							\
	{																						\
		typedef ReturnType(ClassType::*CallBack)(CALL_ARG_LIST_##N);						\
	public:																					\
			FunctionImplMethod##N(Obj& obj, CallBack& call_back)							\
			:m_obj(obj), m_call_back(call_back){}											\
		ReturnType operator () (CALL_ARG_PARAM_LIST_##N)									\
		{																					\
			return (m_obj->*m_call_back)(PARAM_LIST_##N);									\
		}																					\
	private:																				\
		Obj		 m_obj;																		\
		CallBack m_call_back;																\
	};																						\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N, TYPENAME_BIND_ARG_LIST_##N>	\
	Function<ReturnType(CALL_ARG_LIST_##N)> Bind(ReturnType(*CallBack)(CALL_ARG_LIST_##N),	\
			BIND_ARG_PARAM_LIST_##N)														\
	{																						\
		return Function<ReturnType(CALL_ARG_LIST_##N)>(										\
			new FunctionImplFunc##N<ReturnType, CALL_ARG_LIST_##N>(CallBack),				\
			new StorageArgs##N<BIND_ARG_LIST_##N, CALL_ARG_LIST_##N>(BIND_PARAM_LIST_##N));	\
	}																						\
	template<typename ReturnType, TYPENAME_CALL_ARG_LIST_##N, typename ClassType,			\
			typename Obj, TYPENAME_BIND_ARG_LIST_##N>										\
	Function<ReturnType(CALL_ARG_LIST_##N)> Bind(											\
				ReturnType(ClassType::*CallBack)(CALL_ARG_LIST_##N), Obj obj,				\
				BIND_ARG_PARAM_LIST_##N)													\
	{																						\
		return Function<ReturnType(CALL_ARG_LIST_##N)>(										\
			new FunctionImplMethod##N<ReturnType, CALL_ARG_LIST_##N, ClassType, Obj>(obj,	\
				CallBack),																	\
			new StorageArgs##N<BIND_ARG_LIST_##N, CALL_ARG_LIST_##N>(BIND_PARAM_LIST_##N));	\
	}

SUPPORT_FUNC_PARAM_COUNT_MAX(2)
****************************************************************************************/
