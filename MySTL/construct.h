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
* Copyright (c) 1997
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



#ifndef  _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new.h>


#include "config.h"
#include "type_traits.h"
#include "iterator_base.h"

NAMESPACE_BEGIN

template<typename _T1, typename _T2>
void _Construct(_T1 *_p, const _T2 &_val)
{
	new ((void*)_p) _T1(_val);
}

template<typename _T1>
void _Construct(_T1 *_p)
{
	new ((void*)_p) _T1();
}

template <class _Tp>
void _Destroy(_Tp* _pointer)
{
	_pointer->~_Tp();
}

template<typename _ForwardIter>
void _destroy_aux(_ForwardIter _first, _ForwardIter _last, _false_type)
{
	for (; _first != _last; ++_first)
		destroy(&*_first);//op*()
}

template<typename _ForwardIter>
void _destroy_aux(_ForwardIter _first, _ForwardIter _last, _true_type){}

template <class _ForwardIterator, class _Tp>
inline void _destroy(_ForwardIterator _first, _ForwardIterator _last, _Tp*)
{
	typedef typename _type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
	_destroy_aux(_first, _last, _Trivial_destructor());
}

template <class _ForwardIterator>
void _Destroy(_ForwardIterator _first, _ForwardIterator _last)
{
	_destroy(_first, _last, _VALUE_TYPE(_first));
}

// 对原生类型进行重载, 这些类型不需要析构
inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}
inline void _Destroy(wchar_t*, wchar_t*) {}


template <typename _T1, typename _T2>
void construct(_T1 *_p, const _T2 _val)
{
	_Construct(_p, _val);
}

template <typename _T1>
void construct(_T1 *_p)
{
	_Construct(_p);
}

template <typename _Tp>
void destroy(_Tp* _pointer)
{
	_Destroy(_pointer);
}

template <typename _ForwardIterator>
void destroy(_ForwardIterator _first, _ForwardIterator _last)
{
	_Destroy(_first, _last);
}

NAMESPACE_END
#endif//!_CONSTRUCT_H_