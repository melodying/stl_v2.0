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


#ifndef _ALGOBASE_H_
#define _ALGOBASE_H_

// TODO: 以下是对一些简单算法的封装, 这些算法皆是对不需要进行构造的空间的操作


#include "config.h"
#include "iterator_base.h"
#include "type_traits.h"
#include "pair.h"
NAMESPACE_BEGIN


template <typename _ForwardIter1, typename _ForwardIter2, typename _Tp>
inline void _iter_swap(_ForwardIter1 _a, _ForwardIter2 _b, _Tp*)
{
	_Tp _tmp = *_a;
	*_a = *_b;
	*_b = _tmp;
}

template <typename _ForwardIter1, typename _ForwardIter2>
inline void iter_swap(_ForwardIter1 _a, _ForwardIter2 _b)
{
	_iter_swap(_a, _b, _VALUE_TYPE(_a));
}

template<typename _Tp>
void swap(_Tp &_x, _Tp _y)
{
	_Tp _tmp = _x;
	_x = _y;
	_y = _tmp;
}

template<typename _Tp>
const _Tp& min(const _Tp &_a, const _Tp &_b)
{
	return _b < _a ? _b : _a; // op<()
}

template<typename _Tp>
const _Tp& max(const _Tp &_a, const _Tp &_b)
{
	return _a < _b ? _b : _a;
}

template<typename _Tp, typename _Compare>
const _Tp& min(const _Tp &_a, const _Tp &_b, _Compare _comp)
{
	return _comp(_b, _a) ? _b : _a;
}

template<typename _Tp, typename _Compare>
const _Tp& max(const _Tp &_a, const _Tp &_b, _Compare _comp)
{
	return _comp(_a, _b) ? _b : _a;
}

// copy
template <typename _InputIter, typename _OutputIter, typename _Distance>
inline _OutputIter _copy(_InputIter _first, _InputIter _last,
	_OutputIter _result, input_iterator_tag, _Distance*)
{
	for (; _first != _last; ++_first, ++_result)
	{
		*_result = _first;
	}
	return _result;
}

template <typename _RandomAccessIter, typename _OutputIter, typename _Distance>
inline _OutputIter _copy(_RandomAccessIter _first, _RandomAccessIter _last,
	_OutputIter _result, random_access_iterator_tag, _Distance*)
{
	for (_Distance _n = _last - _first; _n > 0; --_n)
	{
		*_result = *_first;
		++_first;
		++_result;
	}
	return _result;
}

template <typename _Tp>
inline _Tp*
_copy_trivial(const _Tp* _first, const _Tp* _last, _Tp* _result)
{
	memmove(_result, _first, sizeof(_Tp) * (_last - _first));
	return _result + (_last - _first);
}

template <typename _InputIter, typename _OutputIter, typename _BoolType>
struct _copy_dispatch 
{
	static _OutputIter copy(_InputIter _first, _InputIter _last,
		_OutputIter _result)
	{
		typedef typename iterator_traits<_InputIter>::iterator_category _Category;
		typedef typename iterator_traits<_InputIter>::difference_type _Distance;
		return _copy(_first, _last, _result, _Category(), (_Distance*)0);
	}
};

template <typename _Tp>
struct _copy_dispatch<_Tp*, _Tp*, _true_type>
{
	static _Tp* copy(const _Tp* _first, const _Tp* _last, _Tp* _result)
	{
		return _copy_trivial(_first, _last, _result);
	}
};

template <typename _Tp>
struct _copy_dispatch<const _Tp*, _Tp*, _true_type>
{
	static _Tp* copy(const _Tp* _first, const _Tp* _last, _Tp* _result) 
	{
		return _copy_trivial(_first, _last, _result);
	}
};

template <typename _InputIter, typename _OutputIter>
inline _OutputIter copy(_InputIter _first, _InputIter _last,
	_OutputIter _result) 
{
	typedef typename iterator_traits<_InputIter>::value_type _Tp;
	typedef typename _type_traits<_Tp>::has_trivial_assignment_operator _Trivial;

	return _copy_dispatch<_InputIter, _OutputIter, _Trivial>::copy(_first, _last, _result);
}
//template<typename _InputIter, typename _OutputIter>
//_OutputIter copy(_InputIter _first, _InputIter _last, _OutputIter _result)
//{
//	for (; _first != _last; ++_first, ++_result)
//	{
//		*_result = *_first;
//	}
//	return _result;
//}



// copy_backward

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _Distance>
inline _BidirectionalIter2 _copy_backward(
	_BidirectionalIter1 _first,
	_BidirectionalIter1 _last,
	_BidirectionalIter2 _result,
	bidirectional_iterator_tag,
	_Distance*)
{
	while (_first != _last)
		*--_result = *--_last; //op--()
	return _result;
}

template <typename _RandomAccessIter, typename _BidirectionalIter, typename _Distance>
inline _BidirectionalIter _copy_backward(
	_RandomAccessIter _first,
	_RandomAccessIter _last,
	_BidirectionalIter _result,
	random_access_iterator_tag,
	_Distance*)
{
	for (_Distance _n = _last - _first; _n > 0; --_n)
		*--_result = *--_last;
	return _result;
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _BoolType>
struct _copy_backward_dispatch
{
	typedef typename iterator_traits<_BidirectionalIter1>::iterator_category
		_Category;
	typedef typename iterator_traits<_BidirectionalIter1>::difference_type
		_Distance;

	static _BidirectionalIter2 copy(_BidirectionalIter1 _first,
		_BidirectionalIter1 _last,
		_BidirectionalIter2 _result) 
	{
		return _copy_backward(_first, _last, _result, _Category(), (_Distance*)0);
	}
};

template <typename _Tp>
struct _copy_backward_dispatch<_Tp*, _Tp*, _true_type>
{
	static _Tp* copy(const _Tp* _first, const _Tp* _last, _Tp* _result)
	{
		const ptrdiff_t _Num = _last - _first;
		memmove(_result - _Num, _first, sizeof(_Tp) * _Num);
		return _result - _Num;
	}
};

template <typename _Tp>
struct _copy_backward_dispatch<const _Tp*, _Tp*, _true_type>
{
	static _Tp* copy(const _Tp* _first, const _Tp* _last, _Tp* _result)
	{
		return  _copy_backward_dispatch<_Tp*, _Tp*, _true_type>
			::copy(_first, _last, _result);
	}
};

template <typename _BidirectionalIter1, typename _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(
	_BidirectionalIter1 _first,
	_BidirectionalIter1 _last,
	_BidirectionalIter2 _result)
{
	typedef typename _type_traits<typename iterator_traits<_BidirectionalIter2>::value_type>
		::has_trivial_assignment_operator _Trivial;

	return _copy_backward_dispatch<_BidirectionalIter1, _BidirectionalIter2, _Trivial>
		::copy(_first, _last, _result);
}

//template<typename _BidirectionalIter1, typename _BidirectionalIter2>
//_BidirectionalIter2 copy_backward(_BidirectionalIter1 _first, _BidirectionalIter1 _last,
//	_BidirectionalIter2 _result)
//{
//	while (_first != _last)
//	{
//		*(--_result) = *(--_last);
//	}
//	return _result;
//}



//copy_n

template <typename _InputIter, typename _Size, typename _OutputIter>
pair<_InputIter, _OutputIter> _copy_n(_InputIter _first, _Size _count,
	_OutputIter _result,
	input_iterator_tag)
{
	for (; _count > 0; --_count) 
	{
		*_result = *_first;
		++_first;
		++_result;
	}
	return pair<_InputIter, _OutputIter>(_first, _result);
}

template <typename _RandomAccessIter, typename _Size, typename _OutputIter>
inline pair<_RandomAccessIter, _OutputIter>
_copy_n(_RandomAccessIter _first, _Size _count,
	_OutputIter _result,
	random_access_iterator_tag) 
{
	_RandomAccessIter _last = _first + _count;
	return pair<_RandomAccessIter, _OutputIter>(_last, copy(_first, _last, _result));
}

template <typename _InputIter, typename _Size, typename _OutputIter>
inline pair<_InputIter, _OutputIter>
_copy_n(_InputIter _first, _Size _count, _OutputIter _result)
{
	return _copy_n(_first, _count, _result, _ITERATOR_CATEGORY(_first));
}

template <typename _InputIter, typename _Size, typename _OutputIter>
inline pair<_InputIter, _OutputIter>
copy_n(_InputIter _first, _Size _count, _OutputIter _result) 
{
	return _copy_n(_first, _count, _result);
}


// fill and fill_n
template<typename  _ForwardIter, typename _Tp>
void fill(_ForwardIter _first, _ForwardIter _last, const _Tp& _val)
{
	for (; _first != _last; ++_first)
		*_first = _val;
}

template<typename _OutputIter, typename _Size, typename _Tp>
_OutputIter fill_n(_OutputIter _first, _Size _n, const _Tp &_val)
{
	for (; _n > 0; --_n, ++_first)
	{
		*_first = _val;
	}
	return _first;
}

// Specialization
inline void fill(unsigned char* _first, unsigned char* _last,
	const unsigned char& _c) 
{
	unsigned char _tmp = _c;
	memset(_first, _tmp, _last - _first);
}

inline void fill(signed char* _first, signed char* _last,
	const signed char& _c)
{
	signed char _tmp = _c;
	memset(_first, static_cast<unsigned char>(_tmp), _last - _first);
}

inline void fill(char* _first, char* _last, const char& _c)
{
	char _tmp = _c;
	memset(_first, static_cast<unsigned char>(_tmp), _last - _first);
}


// equal and mismatch

// 返回第一个不匹配的位置
template <typename _InputIter1, typename _InputIter2>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 _first1,
	_InputIter1 _last1,
	_InputIter2 _first2)
{
	while (_first1 != _last1 && *_first1 == *_first2)
	{
		++_first1;
		++_first2;
	}
	return pair<_InputIter1, _InputIter2>(_first1, _first2);
}

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
pair<_InputIter1, _InputIter2> mismatch(_InputIter1 _first1,
	_InputIter1 _last1,
	_InputIter2 _first2,
	_BinaryPredicate _binary_pred)
{
	while (_first1 != _last1 && _binary_pred(*_first1, *_first2))
	{
		++_first1;
		++_first2;
	}
	return pair<_InputIter1, _InputIter2>(_first1, _first2);
}

template <typename _InputIter1, typename _InputIter2>
inline bool equal(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2)
{
	for (; _first1 != _last1; ++_first1, ++_first2)
	{
		if (*_first1 != *_first2)
			return false;
	}
	return true;
}

template <typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
inline bool equal(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _BinaryPredicate _binary_pred)
{
	for (; _first1 != _last1; ++_first1, ++_first2)
	{
		if (!_binary_pred(*_first1, *_first2))
			return false;
	}
	return true;
}


// 按词典的顺序比较 first < last
template <typename _InputIter1, typename _InputIter2>
bool lexicographical_compare(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _InputIter2 _last2) 
{
	for (; _first1 != _last1 && _first2 != _last2 ; ++_first1, ++_first2)
	{
		if (*_first1 < *_first2)
			return true;
		if (*_first2 < *_first1)
			return false;
	}
	return _first1 == _last1 && _first2 != _last2;
}

template <typename _InputIter1, typename _InputIter2, typename _Compare>
bool lexicographical_compare(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _InputIter2 _last2,
	_Compare _comp)
{
	for (; _first1 != _last1 && _first2 != _last2; ++_first1, ++_first2)
	{
		if (_comp(*_first1, *_first2))
			return true;
		if (_comp(*_first2, *_first1))
			return false;
	}
	return _first1 == _last1 && _first2 != _last2;
}

inline bool
lexicographical_compare(const unsigned char* _first1,
	const unsigned char* _last1,
	const unsigned char* _first2,
	const unsigned char* _last2)
{
	const size_t __len1 = _last1 - _first1;
	const size_t __len2 = _last2 - _first2;
	const int _result = memcmp(_first1, _first2, min(__len1, __len2));
	return _result != 0 ? _result < 0 : __len1 < __len2;
}


NAMESPACE_END
#endif//~_ALGOBASE_H_