/*******************************************************************************
* @brief	��boost::bind, boost::function
 * @date	2015-4-3
 * @author	phenix
 * @mark
 ******************************************************************************/
#ifndef PHENIX_BASE_SIMPLEDELEGATE_H
#define PHENIX_BASE_SIMPLEDELEGATE_H

namespace Phenix
{
namespace Base
{
namespace SimpleDelegate
{
	/*
	todo:
	1. ռλ��
	2. ��ʵ�ֶ������
	3. boost::ref
	4. ���ɸ���
	5. ע��
	6. ��Ա�������͵İ�װ������ָ��
	*/
	template<typename U>
	class Function;

	template<typename R, typename A1> class FunctionImplBase;
	template<typename R, typename A1>
	class Function<R(A1)>
	{
		typedef R	ReturnType;
		typedef A1	ArgType1;
	public:
		Function(FunctionImplBase<ReturnType, ArgType1>* func):m_func_impl(func){}
		ReturnType operator () (ArgType1 arg1){m_func_impl->operator ()(arg1);}
	private:
		FunctionImplBase<ReturnType, ArgType1>* m_func_impl;
	};

	template<typename R, typename A1>
	class FunctionImplBase
	{
		typedef R	ReturnType;
		typedef A1	ArgType1;		
	public:
		virtual ReturnType operator () (ArgType1 arg1) = 0;
	};

	// for ȫ�ֺ�������̬����
	template<typename R, typename A1>
	class FunctionImplFunc
		:public FunctionImplBase<R, A1>
	{
		typedef R	ReturnType;
		typedef A1	ArgType1;
		typedef ReturnType(*CallBack)(ArgType1);
	public:
		FunctionImplFunc(CallBack& call_back):m_call_back(call_back){}
		ReturnType operator () (ArgType1 arg1){return m_call_back(arg1);}
	private:
		CallBack m_call_back;
	};

	// for ���Ա����
	template<typename R, typename T, typename A1>
	class FunctionImplMethod
		:public FunctionImplBase<R, A1>
	{
		typedef R	ReturnType;
		typedef T	Obj;
		typedef A1	ArgType1;
		typedef ReturnType(Obj::*CallBack)(ArgType1);
	public:
		FunctionImplMethod(Obj* obj, CallBack& call_back)
			:m_obj(obj), m_call_back(call_back){}
		ReturnType operator () (ArgType1 arg1)
		{
			return (m_obj->*m_call_back)(arg1);
		}
	private:
		Obj*	 m_obj;
		CallBack m_call_back;
	};

	template<typename R, typename A1>
	FunctionImplFunc<R, A1>* Bind(R(*CallBack)(A1))
	{
		return new FunctionImplFunc<R, A1>(CallBack);
	}

	template<typename R, typename T, typename A1>
	FunctionImplMethod<R, T, A1>* Bind(T* obj, R(T::*CallBack)(A1))
	{
		return new FunctionImplMethod<R, T, A1>(obj, CallBack);
	}

} // end SimpleDelegate
} // end namespace Base
} // end namespace Phenix


#endif