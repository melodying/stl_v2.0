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

#ifndef _HEAP_H_
#define _HEAP_H_

#include "config.h"
#include <xlocinfo.h>
#include "iterator_base.h"

NAMESPACE_BEGIN

template<typename _RandomAccessIter, typename _Distance, typename _Tp>
void _push_heap(_RandomAccessIter _first, _Distance _holeInsex, _Distance _topIndex, _Tp _val)
{
	_Distance _parent = (_holeInsex - 1) / 2;
	while (_holeInsex > _topIndex && *(_first + _parent) < _val)
	{
		*(_first + _holeInsex) = *(_first + _parent);
		_holeInsex = _parent;
		_parent = (_holeInsex - 1) / 2;
	}
	*(_first + _holeInsex) = _val;
}

template<typename _RandomAccessIter, typename _Distance, typename _Tp>
void _push_heap_aux(_RandomAccessIter _first, _RandomAccessIter _last, _Distance*, _Tp*)
{
	_push_heap(_first, _Distance((_last - _first) - 1), _Distance(0), _Tp(*(_last - 1)));
}

template<typename _RandomAccessIter>
void push_heap(_RandomAccessIter _first, _RandomAccessIter _last)
{
	_push_heap_aux(_first, _last, _DISTANCE_TYPE(_first), _VALUE_TYPE(_first));
}

template<typename _RandomAccessIter, typename _Distance, typename _Tp, typename _Compare>
void _push_heap(_RandomAccessIter _first, _Distance _holeInsex, _Distance _topIndex,
				_Tp _val, _Compare _comp)
{
	_Distance _parent = (_holeInsex - 1) / 2;
	while (_holeInsex > _topIndex && _comp(*(_first + _parent), _val))
	{
		*(_first + _holeInsex) = *(_first + _parent);
		_holeInsex = _parent;
		_parent = (_holeInsex - 1) / 2;
	}
	*(_first + _holeInsex) = _val;
}

template<typename _RandomAccessIter, typename _Distance, typename _Tp, typename _Compare>
void _push_heap_aux(_RandomAccessIter _first, _RandomAccessIter _last, 
					_Compare _comp, _Distance*, _Tp*)
{
	_push_heap(_first, _Distance((_last - _first) - 1), _Distance(0), _Tp(*(_last - 1)), _comp);
}

template<typename _RandomAccessIter, typename _Compare>
void push_heap(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _comp)
{
	_push_heap_aux(_first, _last, _comp, _DISTANCE_TYPE(_first), _VALUE_TYPE(_first));
}

template <typename _RandomAccessIterator, typename _Distance, typename _Tp>
void _adjust_heap(_RandomAccessIterator _first, _Distance _holeIndex,
				  _Distance _len, _Tp _value)
{
	_Distance _topIndex = _holeIndex;
	_Distance _secondChild = 2 * _holeIndex + 2;
	while (_secondChild < _len)
	{
		if (*(_first + _secondChild) < *(_first + (_secondChild - 1)))
			--_secondChild;

		*(_first + _holeIndex) = *(_first + _secondChild);
		_holeIndex = _secondChild;
		_secondChild = 2 * (_secondChild + 1);
	}
	if (_secondChild == _len) 
	{
		*(_first + _holeIndex) = *(_first + (_secondChild - 1));
		_holeIndex = _secondChild - 1;
	}
	_push_heap(_first, _holeIndex, _topIndex, _value);
}

template <typename _RandomAccessIterator, typename _Tp, typename _Distance>
void _pop_heap(_RandomAccessIterator _first, _RandomAccessIterator _last,
			   _RandomAccessIterator _result, _Tp _value, _Distance*)
{
	*_result = *_first;
	_adjust_heap(_first, _Distance(0), _Distance(_last - _first), _value);
}

template <typename _RandomAccessIterator, typename _Tp>
void _pop_heap_aux(_RandomAccessIterator _first, _RandomAccessIterator _last, _Tp*)
{
	_pop_heap(_first, _last - 1, _last - 1, _Tp(*(_last - 1)), _DISTANCE_TYPE(_first));
}

template <typename _RandomAccessIterator>
void pop_heap(_RandomAccessIterator _first, _RandomAccessIterator _last)
{
	_pop_heap_aux(_first, _last, _VALUE_TYPE(_first));
}

template <typename _RandomAccessIterator, typename _Distance, typename _Tp, typename _Compare>
void _adjust_heap(_RandomAccessIterator _first, _Distance _holeIndex,
				  _Distance _len, _Tp _value, _Compare _comp)
{
	_Distance _topIndex = _holeIndex;
	_Distance _secondChild = 2 * _holeIndex + 2;
	while (_secondChild < _len)
	{
		if (_comp(*(_first + _secondChild), *(_first + (_secondChild - 1))))
			--_secondChild;

		*(_first + _holeIndex) = *(_first + _secondChild);
		_holeIndex = _secondChild;
		_secondChild = 2 * (_secondChild + 1);
	}
	if (_secondChild == _len)
	{
		*(_first + _holeIndex) = *(_first + (_secondChild - 1));
		_holeIndex = _secondChild - 1;
	}
	_push_heap(_first, _holeIndex, _topIndex, _value, _comp);
}

template <typename _RandomAccessIterator, typename _Tp, typename _Distance, typename _Compare>
void _pop_heap(_RandomAccessIterator _first, _RandomAccessIterator _last,
	_RandomAccessIterator _result, _Tp _value, _Compare _comp, _Distance*)
{
	*_result = *_first;
	_adjust_heap(_first, _Distance(0), _Distance(_last - _first), _value, _comp);
}

template <typename _RandomAccessIterator, typename _Tp, typename _Compare>
void _pop_heap_aux(_RandomAccessIterator _first, _RandomAccessIterator _last, _Tp*, _Compare _comp)
{
	_pop_heap(_first, _last - 1, _last - 1, _Tp(*(_last - 1)), _comp, _DISTANCE_TYPE(_first));
}

template <typename _RandomAccessIterator, typename _Compare>
void pop_heap(_RandomAccessIterator _first, _RandomAccessIterator _last, _Compare _comp)
{
	_pop_heap_aux(_first, _last, _VALUE_TYPE(_first), _comp);
}

template <typename _RandomAccessIterator, typename _Tp, typename _Distance>
void _make_heap(_RandomAccessIterator _first, _RandomAccessIterator _last, _Tp*, _Distance*)
{
	if (_last - _first < 2) 
		return;

	_Distance _len = _last - _first;
	_Distance _parent = (_len - 2) / 2;

	while (true) 
	{
		_adjust_heap(_first, _parent, _len, _Tp(*(_first + _parent)));
		if (_parent == 0) 
			return;

		--_parent;
	}
}

template <typename _RandomAccessIterator>
inline void
make_heap(_RandomAccessIterator _first, _RandomAccessIterator _last)
{
	_make_heap(_first, _last, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first));
}

template <typename _RandomAccessIterator, typename _Compare, typename _Tp, typename _Distance>
void _make_heap(_RandomAccessIterator _first, _RandomAccessIterator _last,
				_Compare _comp, _Tp*, _Distance*)
{
	if (_last - _first < 2) 
		return;

	_Distance _len = _last - _first;
	_Distance _parent = (_len - 2) / 2;

	while (true)
	{
		_adjust_heap(_first, _parent, _len, _Tp(*(_first + _parent)), _comp);
		if (_parent == 0)
			return;

		--_parent;
	}
}

template <typename _RandomAccessIterator, typename _Compare>
inline void
make_heap(_RandomAccessIterator _first, _RandomAccessIterator _last, _Compare _comp)
{
	_make_heap(_first, _last, _comp, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first));
}


template <class _RandomAccessIterator>
void sort_heap(_RandomAccessIterator _first, _RandomAccessIterator _last)
{
	while (_last - _first > 1)
		pop_heap(_first, _last--);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator _first, _RandomAccessIterator _last, _Compare _comp)
{
	while (_last - _first > 1)
		pop_heap(_first, _last--, _comp);
}


NAMESPACE_END

#endif//!_Cmpare