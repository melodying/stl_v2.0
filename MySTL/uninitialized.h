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



#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

#include "construct.h"
#include "config.h"
#include "type_traits.h"
#include "algobase.h"

NAMESPACE_BEGIN

template<typename _InputIter, typename _ForwardIter>
_ForwardIter _uninitialized_copy_aux(_InputIter _first, _InputIter _last, 
						_ForwardIter _result, _true_type)
{
	return copy(_first, _last, _result);
}

template<typename _InputIter, typename _ForwardIter>
_ForwardIter _uninitialized_copy_aux(_InputIter _first, _InputIter _last,
						_ForwardIter _result, _false_type)
{
	_ForwardIter _cur = _result;
	for (; _first != _last; ++_first, ++_cur)
		construct(&*_cur, *_first);

	return _cur;
}

template<typename _InputIter, typename _ForwardIter, typename _Tp>
_ForwardIter _uninitialized_copy(_InputIter _first, _InputIter _last,
	_ForwardIter _result, _Tp*)
{
	typedef typename _type_traits<_Tp>::is_POD_type _Is_POD;
	return _uninitialized_copy_aux(_first, _last, _result, _Is_POD());
}

template<typename _InputIter, typename _ForwardIter>
_ForwardIter uninitialized_copy(_InputIter _first, _InputIter _last, _ForwardIter _result)
{
	return _uninitialized_copy(_first, _last, _result, _VALUE_TYPE(_first));
}

inline char* uninitialized_copy(const char *_first, const char *_last, char *_result)
{
	memmove(_result, _first, (_last - _first) * sizeof(char));
	return _result;
}

inline wchar_t* uninitialized_copy(const wchar_t *_first, const wchar_t *_last, wchar_t *_result)
{
	memmove(_result, _first, (_last - _first) * sizeof(wchar_t));
	return _result;
}



template<typename _ForwardIter, typename _Tp>
void _uninitialized_fill_aux(_ForwardIter _first, _ForwardIter _last, const _Tp &_val, _true_type)
{
	fill(_first, _last, _val);
}

template<typename _ForwardIter, typename _Tp>
void _uninitialized_fill_aux(_ForwardIter _first, _ForwardIter _last, const _Tp &_val, _false_type)
{
	_ForwardIter _cur = _first;

	for (; _cur != _last; ++_cur)
		construct(&*_first, _val);
}

template<typename _ForwardIter, typename _Tp, typename _Tp1>
void _uninitialized_fill(_ForwardIter _first, _ForwardIter _last, const _Tp &_val, _Tp1*)
{
	typedef typename _type_traits<_Tp1>::is_POD_type _Is_POD;
	_uninitialized_fill_aux(_first, _last, _val, _Is_POD());
}


template<typename _ForwardIter, typename _Tp>
void uninitialized_fill(_ForwardIter _first, _ForwardIter _last, const _Tp& _val)
{
	_uninitialized_fill(_first, _last, _val, _VALUE_TYPE(_first));
}

template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter
_uninitialized_fill_n_aux(_ForwardIter _first, _Size _n, const _Tp& _x, _true_type)
{
	return fill_n(_first, _n, _x);
}

template <typename _ForwardIter, typename _Size, typename _Tp>
inline _ForwardIter
_uninitialized_fill_n_aux(_ForwardIter _first, _Size _n, const _Tp& _val, _false_type)
{
	_ForwardIter _cur = _first;

	for (; _n > 0; --_n, ++_cur)
		construct(&*_cur, _val);

	return _cur;
}

template <typename _ForwardIter, typename _Size, typename _Tp, typename _Tp1>
inline _ForwardIter
_uninitialized_fill_n(_ForwardIter _first, _Size _n, const _Tp& _val, _Tp1*)
{
	typedef typename _type_traits<_Tp1>::is_POD_type _Is_POD;
	return _uninitialized_fill_n_aux(_first, _n, _val, _Is_POD());
}


template<typename _ForwardIter, typename  _Size, typename _Tp>
_ForwardIter uninitialized_fill_n(_ForwardIter _first, _Size _n, const _Tp& _val)
{
	return _uninitialized_fill_n(_first, _n, _val, _VALUE_TYPE(_first));
}



template <class _InputIter1, class _InputIter2, class _ForwardIter>
_ForwardIter _uninitialized_copy_copy(_InputIter1 _first1, _InputIter1 _last1, 
							_InputIter2 _first2, _InputIter2 _last2, _ForwardIter _result)
{
	_ForwardIter _mid = uninitialized_copy(_first1, _last1, _result);
	return uninitialized_copy(_first2, _last2, _mid);
}

template <class _ForwardIter, class _Tp, class _InputIter>
_ForwardIter _uninitialized_fill_copy(_ForwardIter _result, _ForwardIter _mid, 
					const _Tp &_val, _InputIter _first, _InputIter _last)
{
	uninitialized_fill(_result, _mid, _val);
	return uninitialized_copy(_first, _last, _mid);
}

template <class _ForwardIter, class _Tp, class _InputIter>
void _uninitialized_copy_fill(_InputIter _first1, _InputIter _last1,
	_ForwardIter _first2, _ForwardIter _last2, const _Tp &_val)
{
	_ForwardIter _mid = uninitialized_copy(_first1, _last1, _first2);
	uninitialized_fill(_mid, _last2, _val);
}


NAMESPACE_END

#endif//!_UNINITIALIZED_H_