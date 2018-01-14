/*
*
* Copyright (c) 1994
* Hewlett-Packard Company
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Hewlett-Packard Company makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*
* Copyright (c) 1996-1998
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*/


#ifndef _FOUNTION_H_
#define _FOUNTION_H_

#include "config.h"

NAMESPACE_BEGIN


template<typename _Arg, typename _Result>
struct unary_function
{
	typedef _Arg argument_type;
	typedef _Result result_type;
};

template<typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
	typedef _Arg1 first_argument_type;
	typedef _Arg2 second_argument_type;
	typedef _Result result_type;
};

template<typename _Tp>
struct plus:public binary_function<_Tp, _Tp, _Tp>
{
	_Tp operator()(const _Tp&_x, const _Tp& _y)const
	{
		return _x + _y;
	}
};

template<typename _Tp>
struct minus : public binary_function<_Tp, _Tp, _Tp>
{
	_Tp operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x - _y;
	}
};

template<typename _Tp>
struct multiplies : public binary_function<_Tp, _Tp, _Tp>
{
	_Tp operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x * _y;
	}
};

template<typename _Tp>
struct divides : public binary_function<_Tp, _Tp, _Tp>
{
	_Tp operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x / _y;
	}
};

template<typename _Tp>
struct modulus : public binary_function<_Tp, _Tp, _Tp>
{
	_Tp operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x % _y;
	}
};

template<typename _Tp>
struct negate : public unary_function<_Tp, _Tp>
{
	_Tp operator()(const _Tp &_x)const
	{
		return -_x;
	}
};

template<typename _Tp>
struct equal_to : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x == _y;
	}
};

template<typename _Tp>
struct not_equal_to : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x != _y;
	}
};

template<typename _Tp>
struct greater : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x > _y;
	}
};

template <typename _Tp>
struct less : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp& _x, const _Tp& _y) const
	{ 
		return _x < _y; 
	}
};

template<typename _Tp>
struct greater_equal : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x >= _y;
	}
};

template <typename _Tp>
struct less_equal : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp& _x, const _Tp& _y) const
	{
		return _x <= _y;
	}
};

template<typename _Tp>
struct logical_and : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp &_x, const _Tp &_y)const
	{
		return _x && _y;
	}
};

template <typename _Tp>
struct logical_or : public binary_function<_Tp, _Tp, bool>
{
	bool operator()(const _Tp& _x, const _Tp& _y) const
	{
		return _x || _y;
	}
};

template<typename _Tp>
struct logical_not : public unary_function<_Tp, bool>
{
	bool operator()(const _Tp &_x)const
	{
		return !_x;
	}
};

// 一元函数对象的否定
template<typename _Predicate>
struct unary_negate : public unary_function<typename _Predicate::argument_type, bool>
{
protected:
	_Predicate _m_pred;
public:
	explicit unary_negate(const _Predicate& _x) : _m_pred(_x){}
	bool operator()(const typename _Predicate::argument_type &_x)const
	{
		return !_m_pred(_x);
	}
};

template<typename _Predicate>
unary_negate<_Predicate> not1(const _Predicate _pred)
{
	return unary_negate<_Predicate>(_pred);
}

template <typename _Predicate>
class binary_negate : public binary_function<typename _Predicate::first_argument_type,
											 typename _Predicate::second_argument_type, 
											 bool>
{
protected:
	_Predicate _m_pred;
public:
	explicit binary_negate(const _Predicate& __x) : _m_pred(__x) {}
	bool operator()(const typename _Predicate::first_argument_type& _x,
					const typename _Predicate::second_argument_type& _y) const
	{
		return !_m_pred(_x, _y);
	}
};

template <typename _Predicate>
inline binary_negate<_Predicate> not2(const _Predicate& _pred)
{
	return binary_negate<_Predicate>(_pred);
}


template<typename _Operation>
class binder1st:public unary_function<typename _Operation::second_argument_type,
									  typename _Operation::result_type>
{
protected:
	_Operation op;
	typename _Operation::first_argument_type value;
public:
	binder1st(const _Operation &_x, const typename _Operation::first_argument_type &_y)
		:op(_x), value(_y){}

	typename _Operation::result_type 
	operator()(const typename _Operation::second_argument_type &_x)const
	{
		return op(value, _x);
	}
};


template <typename _Operation, typename _Tp>
inline binder1st<_Operation>
bind1st(const _Operation& _fn, const _Tp& _x)
{
	typedef typename _Operation::first_argument_type _Arg1_type;
	return binder1st<_Operation>(_fn, _Arg1_type(_x));
}

template<typename _Operation>
class binder2nd :public unary_function<typename _Operation::first_argument_type,
									   typename _Operation::result_type>
{
protected:
	_Operation op;
	typename _Operation::second_argument_type value;
public:
	binder2nd(const _Operation &_x, const typename _Operation::second_argument_type &_y)
		:op(_x), value(_y) {}

	typename _Operation::result_type
		operator()(const typename _Operation::first_argument_type &_x)const
	{
		return op(_x, value);
	}
};

template <typename _Operation, typename _Tp>
inline binder2nd<_Operation>
bind1st(const _Operation& _fn, const _Tp& _x)
{
	typedef typename _Operation::second_argument_type _Arg2_type;
	return binder2nd<_Operation>(_fn, _Arg2_type(_x));
}

template<typename _Arg, typename _Result>
class pointer_to_unary_function : public unary_function<_Arg, _Result>
{
protected:
	_Result (*_m_ptr)(_Arg);
public:
	pointer_to_unary_function(){}
	explicit pointer_to_unary_function(_Result (*_x)(_Arg)) : _m_ptr(_x){}
	_Result operator()(_Arg _x) const
	{
		return _m_ptr(_x);
	}
};

template<typename _Arg, typename _Result>
pointer_to_unary_function<_Arg, _Result> ptr_fun(_Result (*_x)(_Arg))
{
	return pointer_to_unary_function<_Arg, _Result>(_x);
}


template <typename _Arg1, typename _Arg2, typename _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result>
{
protected:
	_Result(*_m_ptr)(_Arg1, _Arg2);
public:
	pointer_to_binary_function() {}
	explicit pointer_to_binary_function(_Result(*_x)(_Arg1, _Arg2))	: _m_ptr(_x) {}
	_Result operator()(_Arg1 _x, _Arg2 _y) const
	{
		return _m_ptr(_x, _y);
	}
};

template <typename _Arg1, typename _Arg2, typename _Result>
inline pointer_to_binary_function<_Arg1, _Arg2, _Result>
ptr_fun(_Result(*_x)(_Arg1, _Arg2))
{
	return pointer_to_binary_function<_Arg1, _Arg2, _Result>(_x);
}


template<typename _Tp>
struct _Identity : public unary_function<_Tp, _Tp>
{
	const _Tp& operator()(const _Tp& _x)const
	{
		return _x;
	}
};

template<typename _Tp>
struct identity : public _Identity<_Tp> {};

template <typename _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type>
{
	const typename _Pair::first_type& operator()(const _Pair &_x)const
	{
		return _x.first;
	}
};

template <typename _Pair>
struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type>
{
	const typename _Pair::second_type& operator()(const _Pair &_x)const
	{
		return _x.second;
	}
};

template <typename _Pair> struct select1st : public _Select1st<_Pair> {};
template <typename _Pair> struct select2nd : public _Select2nd<_Pair> {};


template <typename _Arg1, typename _Arg2>
struct _Project1st : public binary_function<_Arg1, _Arg2, _Arg1>
{
	_Arg1 operator()(const _Arg1& _x, const _Arg2&) const
	{ 
		return _x; 
	}
};

template <typename _Arg1, typename _Arg2>
struct _Project2nd : public binary_function<_Arg1, _Arg2, _Arg2>
{
	_Arg2 operator()(const _Arg1&, const _Arg2& _y) const
	{ 
		return _y; 
	}
};

template <typename _Arg1, typename _Arg2>
struct project1st : public _Project1st<_Arg1, _Arg2> {};

template <typename _Arg1, typename _Arg2>
struct project2nd : public _Project2nd<_Arg1, _Arg2> {};


template <typename _Result>
struct _Constant_void_fun 
{
	typedef _Result result_type;
	result_type _m_val;

	_Constant_void_fun(const result_type& _v) : _m_val(_v) {}
	const result_type& operator()() const 
	{
		return _m_val; 
	}
};

template <typename _Result, typename _Argument>
struct _Constant_unary_fun 
{
	typedef _Argument argument_type;
	typedef  _Result  result_type;
	result_type _m_val;

	_Constant_unary_fun(const result_type& _v) : _m_val(_v) {}
	const result_type& operator()(const _Argument&) const 
	{
		return _m_val; 
	}
};

template <typename _Result, typename _Arg1, typename _Arg2>
struct _Constant_binary_fun 
{
	typedef  _Arg1   first_argument_type;
	typedef  _Arg2   second_argument_type;
	typedef  _Result result_type;
	_Result _m_val;

	_Constant_binary_fun(const _Result& _v) : _m_val(_v) {}
	const result_type& operator()(const _Arg1&, const _Arg2&) const 
	{
		return _m_val;
	}
};

template <typename _Result>
struct constant_void_fun : public _Constant_void_fun<_Result> 
{
	constant_void_fun(const _Result& _v) : _Constant_void_fun<_Result>(_v) {}
};


template <typename _Result, typename _Argument = _Result>
struct constant_unary_fun : public _Constant_unary_fun<_Result, _Argument>
{
	constant_unary_fun(const _Result& _v): _Constant_unary_fun<_Result, _Argument>(_v) {}
};

template <typename _Result, typename _Arg1 = _Result, typename _Arg2 = _Arg1>
struct constant_binary_fun
	: public _Constant_binary_fun<_Result, _Arg1, _Arg2>
{
	constant_binary_fun(const _Result& _v)
		: _Constant_binary_fun<_Result, _Arg1, _Arg2>(_v) {}
};

template <typename _Result>
inline constant_void_fun<_Result> constant0(const _Result& _val)
{
	return constant_void_fun<_Result>(_val);
}

template <typename _Result>
inline constant_unary_fun<_Result, _Result> constant1(const _Result& _val)
{
	return constant_unary_fun<_Result, _Result>(_val);
}

template <typename _Result>
inline constant_binary_fun<_Result, _Result, _Result>
constant2(const _Result& _val)
{
	return constant_binary_fun<_Result, _Result, _Result>(_val);
}



template <typename _Ret, typename _Tp>
class mem_fun_t : public unary_function<_Tp*, _Ret>
{
public:
	explicit mem_fun_t(_Ret(_Tp::*_pf)()) : _m_f(_pf) {}
	_Ret operator()(_Tp* _p) const 
	{
		return (_p->*_m_f)(); 
	}
private:
	_Ret(_Tp::*_m_f)();
};

template <typename _Ret, typename _Tp>
class const_mem_fun_t : public unary_function<const _Tp*, _Ret> 
{
public:
	explicit const_mem_fun_t(_Ret(_Tp::*_pf)() const) : _m_f(_pf) {}
	_Ret operator()(const _Tp* _p) const
	{ 
		return (_p->*_m_f)();
	}
private:
	_Ret(_Tp::*_m_f)() const;
};


template <typename _Ret, typename _Tp>
class mem_fun_ref_t : public unary_function<_Tp, _Ret> 
{
public:
	explicit mem_fun_ref_t(_Ret(_Tp::*_pf)()) : _m_f(_pf) {}
	_Ret operator()(_Tp& _r) const 
	{ 
		return (_r.*_m_f)(); 
	}
private:
	_Ret(_Tp::*_m_f)();
};

template <typename _Ret, typename _Tp>
class const_mem_fun_ref_t : public unary_function<_Tp, _Ret>
{
public:
	explicit const_mem_fun_ref_t(_Ret(_Tp::*_pf)() const) : _m_f(_pf) {}
	_Ret operator()(const _Tp& _r) const 
	{ 
		return (_r.*_m_f)(); 
	}
private:
	_Ret(_Tp::*_m_f)() const;
};

template <typename _Ret, typename _Tp, typename _Arg>
class mem_fun1_t : public binary_function<_Tp*, _Arg, _Ret>
{
public:
	explicit mem_fun1_t(_Ret(_Tp::*_pf)(_Arg)) : _m_f(_pf) {}
	_Ret operator()(_Tp* _p, _Arg _x) const 
	{ 
		return (_p->*_m_f)(_x); 
	}
private:
	_Ret(_Tp::*_m_f)(_Arg);
};

template <typename _Ret, typename _Tp, typename _Arg>
class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret>
{
public:
	explicit const_mem_fun1_t(_Ret(_Tp::*_pf)(_Arg) const) : _m_f(_pf) {}
	_Ret operator()(const _Tp* _p, _Arg _x) const
	{
		return (_p->*_m_f)(_x);
	}
private:
	_Ret(_Tp::*_m_f)(_Arg) const;
};

template <typename _Ret, typename _Tp, typename _Arg>
class mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret>
{
public:
	explicit mem_fun1_ref_t(_Ret(_Tp::*_pf)(_Arg)) : _m_f(_pf) {}
	_Ret operator()(_Tp& _r, _Arg _x) const
	{ 
		return (_r.*_m_f)(_x);
	}
private:
	_Ret(_Tp::*_m_f)(_Arg);
};

template <typename _Ret, typename _Tp, typename _Arg>
class const_mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret> 
{
public:
	explicit const_mem_fun1_ref_t(_Ret(_Tp::*_pf)(_Arg) const) : _m_f(_pf) {}
	_Ret operator()(const _Tp& _r, _Arg _x) const
	{ 
		return (_r.*_m_f)(_x); 
	}
private:
	_Ret(_Tp::*_m_f)(_Arg) const;
};

template <typename _Tp>
class mem_fun_t<void, _Tp> : public unary_function<_Tp*, void> 
{
public:
	explicit mem_fun_t(void (_Tp::*_pf)()) : _m_f(_pf) {}
	void operator()(_Tp* _p) const 
	{ 
		(_p->*_m_f)();
	}
private:
	void (_Tp::*_m_f)();
};

template <typename _Tp>
class const_mem_fun_t<void, _Tp> : public unary_function<const _Tp*, void> 
{
public:
	explicit const_mem_fun_t(void (_Tp::*_pf)() const) : _m_f(_pf) {}
	void operator()(const _Tp* _p) const 
	{ 
		(_p->*_m_f)(); 
	}
private:
	void (_Tp::*_m_f)() const;
};

template <typename _Tp>
class mem_fun_ref_t<void, _Tp> : public unary_function<_Tp, void> 
{
public:
	explicit mem_fun_ref_t(void (_Tp::*_pf)()) : _m_f(_pf) {}
	void operator()(_Tp& _r) const { (_r.*_m_f)(); }
private:
	void (_Tp::*_m_f)();
};

template <typename _Tp>
class const_mem_fun_ref_t<void, _Tp> : public unary_function<_Tp, void>
{
public:
	explicit const_mem_fun_ref_t(void (_Tp::*_pf)() const) : _m_f(_pf) {}
	void operator()(const _Tp& _r) const { (_r.*_m_f)(); }
private:
	void (_Tp::*_m_f)() const;
};

template <typename _Tp, typename _Arg>
class mem_fun1_t<void, _Tp, _Arg> : public binary_function<_Tp*, _Arg, void> 
{
public:
	explicit mem_fun1_t(void (_Tp::*_pf)(_Arg)) : _m_f(_pf) {}
	void operator()(_Tp* _p, _Arg _x) const { (_p->*_m_f)(_x); }
private:
	void (_Tp::*_m_f)(_Arg);
};

template <typename _Tp, typename _Arg>
class const_mem_fun1_t<void, _Tp, _Arg>
	: public binary_function<const _Tp*, _Arg, void>
{
public:
	explicit const_mem_fun1_t(void (_Tp::*_pf)(_Arg) const) : _m_f(_pf) {}
	void operator()(const _Tp* _p, _Arg _x) const { (_p->*_m_f)(_x); }
private:
	void (_Tp::*_m_f)(_Arg) const;
};

template <typename _Tp, typename _Arg>
class mem_fun1_ref_t<void, _Tp, _Arg>
	: public binary_function<_Tp, _Arg, void> 
{
public:
	explicit mem_fun1_ref_t(void (_Tp::*_pf)(_Arg)) : _m_f(_pf) {}
	void operator()(_Tp& _r, _Arg _x) const { (_r.*_m_f)(_x); }
private:
	void (_Tp::*_m_f)(_Arg);
};

template <typename _Tp, typename _Arg>
class const_mem_fun1_ref_t<void, _Tp, _Arg>
	: public binary_function<_Tp, _Arg, void> 
{
public:
	explicit const_mem_fun1_ref_t(void (_Tp::*_pf)(_Arg) const) : _m_f(_pf) {}
	void operator()(const _Tp& _r, _Arg _x) const { (_r.*_m_f)(_x); }
private:
	void (_Tp::*_m_f)(_Arg) const;
};

template <typename _Ret, typename _Tp>
inline mem_fun_t<_Ret, _Tp> mem_fun(_Ret(_Tp::*_f)())
{
	return mem_fun_t<_Ret, _Tp>(_f);
}

template <typename _Ret, typename _Tp>
inline const_mem_fun_t<_Ret, _Tp> mem_fun(_Ret(_Tp::*_f)() const)
{
	return const_mem_fun_t<_Ret, _Tp>(_f);
}

template <typename _Ret, typename _Tp>
inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret(_Tp::*_f)())
{
	return mem_fun_ref_t<_Ret, _Tp>(_f);
}

template <typename _Ret, typename _Tp>
inline const_mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret(_Tp::*_f)() const)
{
	return const_mem_fun_ref_t<_Ret, _Tp>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline mem_fun1_t<_Ret, _Tp, _Arg> mem_fun(_Ret(_Tp::*_f)(_Arg))
{
	return mem_fun1_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline const_mem_fun1_t<_Ret, _Tp, _Arg> mem_fun(_Ret(_Tp::*_f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline mem_fun1_ref_t<_Ret, _Tp, _Arg> mem_fun_ref(_Ret(_Tp::*_f)(_Arg))
{
	return mem_fun1_ref_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline const_mem_fun1_ref_t<_Ret, _Tp, _Arg>
mem_fun_ref(_Ret(_Tp::*_f)(_Arg) const)
{
	return const_mem_fun1_ref_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline mem_fun1_t<_Ret, _Tp, _Arg> mem_fun1(_Ret(_Tp::*_f)(_Arg))
{
	return mem_fun1_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline const_mem_fun1_t<_Ret, _Tp, _Arg> mem_fun1(_Ret(_Tp::*_f)(_Arg) const)
{
	return const_mem_fun1_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline mem_fun1_ref_t<_Ret, _Tp, _Arg> mem_fun1_ref(_Ret(_Tp::*_f)(_Arg))
{
	return mem_fun1_ref_t<_Ret, _Tp, _Arg>(_f);
}

template <typename _Ret, typename _Tp, typename _Arg>
inline const_mem_fun1_ref_t<_Ret, _Tp, _Arg>
mem_fun1_ref(_Ret(_Tp::*_f)(_Arg) const)
{
	return const_mem_fun1_ref_t<_Ret, _Tp, _Arg>(_f);
}



NAMESPACE_END

#endif//!_FOUNTION_H_