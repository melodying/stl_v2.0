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

#ifndef _ITERATOR_BASE_H_
#define _ITERATOR_BASE_H_

#include "config.h"


NAMESPACE_BEGIN


struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template <typename _Tp, typename _Distance>
struct input_iterator
{
	typedef input_iterator_tag iterator_category;
	typedef _Tp                value_type;
	typedef _Distance          difference_type;
	typedef _Tp*               pointer;
	typedef _Tp&               reference;
};

struct output_iterator
{
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;
};

template <typename _Tp, typename _Distance>
struct forward_iterator
{
	typedef forward_iterator_tag iterator_category;
	typedef _Tp                  value_type;
	typedef _Distance            difference_type;
	typedef _Tp*                 pointer;
	typedef _Tp&                 reference;
};

template <typename _Tp, typename _Distance>
struct bidirectional_iterator
{
	typedef bidirectional_iterator_tag iterator_category;
	typedef _Tp                        value_type;
	typedef _Distance                  difference_type;
	typedef _Tp*                       pointer;
	typedef _Tp&                       reference;
};

template <typename _Tp, typename _Distance>
struct random_access_iterator 
{
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp                        value_type;
	typedef _Distance                  difference_type;
	typedef _Tp*                       pointer;
	typedef _Tp&                       reference;
};


template<typename _Iterator>
struct iterator_traits
{
	typedef typename _Iterator::iterator_category iterator_category;
	typedef typename _Iterator::value_type        value_type;
	typedef typename _Iterator::difference_type   difference_type;
	typedef typename _Iterator::pointer           pointer;
	typedef typename _Iterator::reference         reference;
};

template <typename _Tp>
struct iterator_traits<_Tp*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp                         value_type;
	typedef ptrdiff_t                   difference_type;
	typedef _Tp*                        pointer;
	typedef _Tp&                        reference;
};

// 类型萃取机
template <typename _Tp>
struct iterator_traits<const _Tp*> 
{
	typedef random_access_iterator_tag iterator_category;
	typedef _Tp                         value_type;
	typedef ptrdiff_t                   difference_type;
	typedef const _Tp*                  pointer;
	typedef const _Tp&                  reference;
};

template <typename _Iter>
inline typename iterator_traits<_Iter>::iterator_category
_iterator_category(const _Iter&)
{
	typedef typename iterator_traits<_Iter>::iterator_category _Category;
	return _Category();
}

template <typename _Iter>
inline typename iterator_traits<_Iter>::difference_type*
_distance_type(const _Iter&)
{
	return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
}

template <typename _Iter>
inline typename iterator_traits<_Iter>::value_type*
_value_type(const _Iter&)
{
	return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
}

template <typename _Iter>
inline typename iterator_traits<_Iter>::iterator_category
iterator_category(const _Iter& __i)
{
	return _iterator_category(__i);
}


template <typename _Iter>
inline typename iterator_traits<_Iter>::difference_type*
distance_type(const _Iter& __i)
{
	return _distance_type(__i);
}

template <typename _Iter>
inline typename iterator_traits<_Iter>::value_type*
value_type(const _Iter& __i) 
{
	return _value_type(__i);
}


#define _ITERATOR_CATEGORY(__i) _iterator_category(__i)
#define _DISTANCE_TYPE(__i)     _distance_type(__i)
#define _VALUE_TYPE(__i)        _value_type(__i)


template <typename _InputIterator, typename _Distance>
inline void _distance(_InputIterator _first, _InputIterator _last,
	_Distance& _n, input_iterator_tag)
{
	while (_first != _last)
	{
		++_first; 
		++_n;
	}
}

template <typename _RandomAccessIterator, typename _Distance>
inline void _distance(_RandomAccessIterator _first,
	_RandomAccessIterator _last,
	_Distance& _n, random_access_iterator_tag)
{
	_n += _last - _first;
}

template <typename _InputIterator, typename _Distance>
inline void distance(_InputIterator _first,
	_InputIterator _last, _Distance& _n)
{
	_distance(_first, _last, _n, iterator_category(_first));
}

template <typename _InputIter, typename _Distance>
inline void _advance(_InputIter& _i, _Distance _n, input_iterator_tag) 
{
	while (_n--) 
		++_i;
}

template <typename _BidirectionalIterator, typename _Distance>
inline void _advance(_BidirectionalIterator& _i, _Distance _n,
	bidirectional_iterator_tag)
{
	if (_n >= 0)
	{
		while (_n--)
			++_i;
	}
	else
	{
		while (_n++)
			--_i;
	}
}

template <typename _RandomAccessIterator, typename _Distance>
inline void _advance(_RandomAccessIterator& _i, _Distance _n,
	random_access_iterator_tag) 
{
	_i += _n;
}

template <typename _InputIterator, typename _Distance>
inline void advance(_InputIterator& _i, _Distance _n)
{
	_advance(_i, _n, iterator_category(_i));
}




NAMESPACE_END

#endif//!_ITERATOR_BASE_H_