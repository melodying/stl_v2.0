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
* Copyright (c) 1996
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

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "config.h"
#include "iterator.h"
#include "algobase.h"
#include "heap.h"
#include "tempbuf.h"

NAMESPACE_BEGIN

// _median 三个数取中间值
template <typename _Tp>
const _Tp& _median(const _Tp& _a, const _Tp& _b, const _Tp& _c)
{
	if (_a < _b)
	{
		if (_b < _c)
			return _b;
		else if (_a < _c)
			return _c;
		else
			return _a;
	}
	else if (_a < _c)
		return _a;
	else if (_b < _c)
		return _c;
	else
		return _b;
}

template <typename _Tp, class _Compare>
const _Tp& _median(const _Tp& _a, const _Tp& _b, const _Tp& _c, _Compare _comp)
{	
	if (_comp(_a, _b))
	{
		if (_comp(_b, _c))
			return _b;
		else if (_comp(_a, _c))
			return _c;
		else
			return _a;
	}
	else if (_comp(_a, _c))
		return _a;
	else if (_comp(_b, _c))
		return _c;
	else
		return _b;
}


// all_of, any_of, none_of遍历容器判断是否满足某种条件
template<typename _InputIter, typename _UnaryPred>
bool all_of(_InputIter _first, _InputIter _last, _UnaryPred _pred)
{
	while (_first != _last)
	{
		if(!_pred(*_first))
			return false;

		++_first;
	}
	return true;
}

template<typename _InputIter, typename _UnaryPred>
bool any_of(_InputIter _first, _InputIter _last, _UnaryPred _pred)
{
	while (_first != _last)
	{
		if (_pred(*_first))
			return true;

		++_first;
	}
	return false;
}

template<typename _InputIter, typename _UnaryPred>
bool none_of(_InputIter _first, _InputIter _last, _UnaryPred _pred)
{
	while (_first != _last)
	{
		if (_pred(*_first))
			return false;

		++_first;
	}
	return true;
}

// for_each 通过某种方法对容器进行遍历
template<typename _InputIter, typename _Function>
_Function for_each(_InputIter _first, _InputIter _last, _Function _fun)
{
	for (; _first != _last; ++_first)
		_fun(*_first);

	return _fun;
}

// find, find_if, find_if_not 查找容器中某元素所在的位置, 或满足某种条件的元素所在位置
template<typename _InputIter, typename _Tp>
_InputIter find(_InputIter _first, _InputIter _last, const _Tp &_val)
{
	while (_first != _last)
	{
		if (*_first == _val)
			return _first;

		++_first;
	}
	return _last;
}

template<typename _InputIter, typename _UnaryPred>
_InputIter find_if(_InputIter _first, _InputIter _last, const _UnaryPred &_pred)
{
	while (_first != _last)
	{
		if (_pred(*_first))
			return _first;

		++_first;
	}
	return _last;
}

template<typename _InputIter, typename _UnaryPred>
_InputIter find_if_not(_InputIter _first, _InputIter _last, const _UnaryPred &_pred)
{
	while (_first != _last)
	{
		if (!_pred(*_first))
			return _first;

		++_first;
	}
	return _last;
}


// adjacent_find 查找相邻元素值相同的第一个元素的位置
template<typename _ForwardIter>
_ForwardIter adjacent_find(_ForwardIter _first, _ForwardIter _last)
{
	if (_first == _last)
		return _last;

	_ForwardIter _next = _first;
	while (++_next != _last)
	{
		if (*_first == *_next)
			return _first;

		_first = _next;
	}
	return _last;
}

template <typename _ForwardIter, typename _BinaryPredicate>
_ForwardIter adjacent_find(_ForwardIter _first, _ForwardIter _last,
	_BinaryPredicate _binary_pred)
{
	if (_first == _last)
		return _last;

	_ForwardIter _next = _first;
	while (++_next != _last)
	{
		if (_binary_pred(*_first, *_next))
			return _first;

		_first = _next;
	}
	return _last;
}

// count, count_if 对容器的某个元素计数
template<typename _InputIter, typename _Tp, typename _Size>
void count(_InputIter _first, _InputIter _last, const _Tp &_val, _Size &_n)
{
	for (; _first != _last; ++_first)
	{
		if (*_first == _val)
			++_n;
	}
}

template<typename _InputIter, typename _Predicate, typename _Size>
void count_if(_InputIter _first, _InputIter _last, _Predicate _pred, _Size &_n)
{
	for (; _first != _last; ++_first)
	{
		if (_pred(*_first))
			++_n;
	}
}

template<typename _InputIter, typename _Tp>
typename iterator_traits<_InputIter>::difference_type
count(_InputIter _first, _InputIter _last, const _Tp &_val)
{
	typename iterator_traits<_InputIter>::difference_type _ret = 0;
	for (; _first != _last; ++_first)
	{
		if (*_first == _val)
			++_ret;
	}
	return _ret;
}

template<typename _InputIter, typename _Predicate>
typename iterator_traits<_InputIter>::difference_type
count_if(_InputIter _first, _InputIter _last, const _Predicate _pred)
{
	typename iterator_traits<_InputIter>::difference_type _ret = 0;
	for (; _first != _last; ++_first)
	{
		if (_pred(*_first))
			++_ret;
	}
	return _ret;
}

// search 判断第一个容器是否存在元素和第二个容器完全匹配
// 如果有, 返回匹配的首元素所在第一个容器的位置
template<typename _ForwardIter1, typename _ForwardIter2>
_ForwardIter1 search(_ForwardIter1 _first1, _ForwardIter1 _last1, 
					 _ForwardIter2 _first2, _ForwardIter2 _last2)
{
	// 空容器
	if (_first1 == _last1 || _first2 == _last2)
		return _first1;

	// 第二个容器只有一个元素
	_ForwardIter2 _tmp(_first2);
	++_tmp;
	if (_tmp == _last2)
		return find(_first1, _last1, *_first2);

	// 一般情况
	_ForwardIter2 _p, _p1;
	_p1 = _first2;
	++_p1;

	_ForwardIter1 _current = _first1;
	while (_first1 != _last1)
	{
		_first1 = find(_first1, _last1, *_first2);	// 逐渐缩小查找范围
		_p = _p1;
		_current = _first1;

		if (++_current == _last1)
			return _last1;

		while (*_current == *_p)
		{
			if (++_p == _last2)
				return _first1;
			if (++_current == _last1)
				return _last1;
		}

		++_first1;
	}
	return _first1;
}

template<typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPred>
_ForwardIter1 search(_ForwardIter1 _first1, _ForwardIter1 _last1,
					 _ForwardIter2 _first2, _ForwardIter2 _last2,
					 _BinaryPred _pred)
{
	if (_first1 == _last1 || _first2 == _last2)
		return _first1;

	_ForwardIter2 _tmp(_first2);
	++_tmp;
	if (_tmp == _last2)
	{
		while (_first1 != _last1 && !_pred(*_first1, *_first2))
			++_first1;

		return _first1;
	}

	// 此段代码相较上面的代码来说，因为要不停的调用迭代器的构造，效率较低
	while (_first1 != _last1)
	{
		_ForwardIter1 _it1 = _first1;
		_ForwardIter2 _it2 = _first2;

		while (_pred(*_it1, *_it2))
		{
			if (_it2 == _last2)
				return _first1;
			if (_it1 == _last1)
				return _last1;
			++_it1;
			++_it2;
		}
		
		++_first1;
	}
	return _first1;
}

// search_n 在容器中查找出现n个val的首元素位置
template<typename _ForwardIter, typename _Integer, typename _Tp>
_ForwardIter search_n(_ForwardIter _first, _ForwardIter _last, _Integer _count, const _Tp& _val)
{
	if (_count <= 0)
		return _first;
	else
	{
		_first = find(_first, _last, _val);
		while (_first != _last)
		{
			_Integer _n = _count - 1;
			_ForwardIter _it = _first;
			++_it;
			while (_it != _last && _n != 0 && *_it == _val)
			{
				++_it;
				--_n;
			}
			if (_n == 0)
				return _first;
			else
				_first = find(_it, _last, _val);
		}
		return _last;
	}
}

template<typename _ForwardIter, typename _Integer, typename _Tp, typename _BinaryPred>
_ForwardIter search_n(_ForwardIter _first, _ForwardIter _last, 
					  _Integer _count, const _Tp& _val, _BinaryPred _pred)
{
	if (_count <= 0)
		return _first;
	else
	{
		while (_first != _last)
		{
			if(_pred(*_first, _val))
				break;
			++_first;
		}
		while (_first != _last)
		{
			_Integer _n = _count - 1;
			_ForwardIter _it = _first;
			++_it;
			while (_it != _last && _n != 0 && _pred(*_it, _val))
			{
				++_it;
				--_n;
			}
			if (_n == 0)
				return _first;
			else
			{
				while (_it != _last)
				{
					if (_pred(*_first, _val))
						break;
					++_it;
				}
				_first = _it;
			}
		}
		return _last;
	}
}

// swap_range 将两个容器的元素互换
template<typename _ForwardIter1, typename _ForwardIter2>
_ForwardIter2 swap_ranges(_ForwardIter1 _first1, _ForwardIter1 _last1,
						 _ForwardIter2 _first2, _ForwardIter2 _last2)
{
	for (; _first1 != _last1; ++_first1, ++_first2)
		iterator_swap(_first1, _first2);

	return _first2;
}

// transform 将一容器的所有元素通过某一操作后放到第二个容器中, 原容器不变
template<typename _InputIter, typename _OutputIter, typename _UnaryOperation>
_OutputIter transform(_InputIter _first, _InputIter _last,
					  _OutputIter _result, _UnaryOperation _oper)
{
	for (; _first != _last; ++_first, ++_result)
		*_result = _oper(*_first);

	return _result;
}

template<typename _InputIter1, typename _InputIter2, 
		 typename _OutputIter, typename _BinaryOperation>
_OutputIter transform(_InputIter1 _first1, _InputIter1 _last1,
					  _InputIter2 _first2, _OutputIter _result, _BinaryOperation _oper)
{
	for (; _first1 != _last1; ++_first1, ++_first2, ++_result)
		*_result = _oper(*_first1, *_first2);

	return _result;
}

// replace, replace_if 对容器进行元素替换
template<typename _ForwardIter, typename _Tp>
void replace(_ForwardIter _first, _ForwardIter _last, 
			const _Tp &_old_val, const _Tp &_new_val)
{
	for (; _first != _last; ++_first)
	{
		if (*_first == _old_val)
			*_first = _new_val;
	}
}

template<typename _ForwardIter, typename _Predicate, typename _Tp>
void replace_if(_ForwardIter _first, _ForwardIter _last,
			    _Predicate _pred, const _Tp &_new_val)
{
	for (; _first != _last; ++_first)
	{
		if (_pred(*_first))
			*_first = _new_val;
	}
}

// replace_copy, replace_copy_if 将容器的值拷贝到新的容器中, 并将old_val进行替换。不会改变原容器的值
template<typename _InputIter, typename _OutputIter, typename _Tp>
_OutputIter replace_copy(_InputIter _first, _InputIter _last, _OutputIter _result,
						 const _Tp& _old_val, const _Tp& _new_val)
{
	for (; _first != _last; ++_first, ++_result)
		*_result = (*_first == _old_val ? _new_val : *_first);

	return _result;
}

template<typename _InputIter, typename _OutputIter, typename _Predicate, typename _Tp>
_OutputIter replace_copy(_InputIter _first, _InputIter _last, _OutputIter _result,
						 _Predicate _pred, const _Tp& _new_val)
{
	for (; _first != _last; ++_first, ++_result)
		*_result = (_pred(*_first) ? _new_val : *_first);

	return _result;
}

// generate, generate_n 元素生成器
template<typename _ForwardIter, typename _Generator>
void generate(_ForwardIter _first, _ForwardIter _last, _Generator _gen)
{
	for (; _first != _last; ++_first)
		*_first = _gen();
}

template<typename _OutputIter, typename _Size, typename _Generator>
_OutputIter generate_n(_OutputIter _first, _Size _n, _Generator _gen)
{
	for (; _n > 0; --_n, ++_first)
		*_first = _gen();

	return _first;
}

// remove_copy, remove_copy_if 将值不为val或不满足某条件的元素拷贝到新的容器中, 原容器不变
template<typename _InputIter, typename _OutputIter, typename _Tp>
_OutputIter remove_copy(_InputIter _first, _InputIter _last, 
						_OutputIter _result, const _Tp &_val)
{
	for (; _first != _last; ++_first)
	{
		if (!(*_first == _val))
		{
			*_result = *_first;
			++_result;
		}
	}
	return _result;
}

template<typename _InputIter, typename _OutputIter, typename _Predicate>
_OutputIter remove_copy_if(_InputIter _first, _InputIter _last,
	_OutputIter _result, _Predicate &_pred)
{
	for (; _first != _last; ++_first)
	{
		if (!_pred(*_first))
		{
			*_result = *_first;
			++_result;
		}
	}
	return _result;
}

// remove, remove_if 在容器中移除元素, 此方法会使外部的迭代器失效. 其返回值指向更新后的容器的end()位置
template<typename _ForwardIter, typename _Tp>
_ForwardIter remove(_ForwardIter _first, _ForwardIter _last, const _Tp &_val)
{
	_first = find(_first, _last, _val);
	_ForwardIter _it = _first;
	return _first == _last ? _first : remove_copy(++_it, _last, _first, _val);
}

template<typename _ForwardIter, typename _Predicate>
_ForwardIter remove_if(_ForwardIter _first, _ForwardIter _last, _Predicate _pred)
{
	_first = find_if(_first, _last, _pred);
	_ForwardIter _it = _first;
	return _first == _last ? _first : remove_copy_if(++_it, _last, _first, _pred);
}

// unique_copy 将容器中的元素不重复的拷贝到第二个容器中
// 对原生指针和前向迭代器进行了重载
template<typename _InputIter, typename _OutputIter, typename _Tp>
_OutputIter _unique_copy(_InputIter _first, _InputIter _last, _OutputIter _result, _Tp*)
{
	_Tp _val = *_first;
	*_result = _val;
	while (++_first != _last)
	{
		if(!(_val == *_first))
		{
			_val = *_first;
			*++_result = _val;
		}
	}
	return ++_result;
}

template<typename _InputIter, typename _OutputIter>
_OutputIter _unique_copy(_InputIter _first, _InputIter _last,
						 _OutputIter _result, output_iterator_tag)
{
	return _unique_copy(_first, _last, _result, _VALUE_TYPE(_first));
}

template<typename _InputIter, typename _ForwardIter>
_ForwardIter _unique_copy(_InputIter _first, _InputIter _last,
						  _ForwardIter _result, forward_iterator_tag)
{
	*_result = *_first;
	while (++_first != _last)
	{
		if (!(*_result == *_first))
			*++_result = *_first;
	}
	return ++_result;
}

template<typename _InputIter, typename _OutputIter>
_OutputIter unique_copy(_InputIter _first, _InputIter _last, _OutputIter _result)
{
	if (_first == _last)
		return _result;
	return _unique_copy(_first, _last, _result, _ITERATOR_CATEGORY(_result));
}

template <typename _InputIter, typename _OutputIter, 
		  typename _BinaryPredicate, typename _Tp>
_OutputIter _unique_copy(_InputIter _first, _InputIter _last,
						 _OutputIter _result, _BinaryPredicate _pred, _Tp*)
{
	_Tp _value = *_first;
	*_result = _value;
	while (++_first != _last)
	{
		if (!_pred(_value, *_first))
		{
			_value = *_first;
			*++_result = _value;
		}
	}
	return ++_result;
}

template <typename _InputIter, typename _OutputIter, typename _BinaryPredicate>
_OutputIter _unique_copy(_InputIter _first, _InputIter _last,
		_OutputIter _result, _BinaryPredicate _pred, output_iterator_tag)
{
	return _unique_copy(_first, _last, _result, _pred, _VALUE_TYPE(_first));
}

template <typename _InputIter, typename _ForwardIter, typename _BinaryPredicate>
_ForwardIter _unique_copy(_InputIter _first, _InputIter _last,
	_ForwardIter _result, _BinaryPredicate _pred, forward_iterator_tag)
{
	*_result = *_first;
	while (++_first != _last)
	{
		if (!_pred(*_result, *_first))
			*++_result = *_first;
	}
	return ++_result;
}

template <typename _InputIter, typename _OutputIter, typename _BinaryPredicate>
_OutputIter unique_copy(_InputIter _first, _InputIter _last,
	_OutputIter _result, _BinaryPredicate _pred)
{
	if (_first == _last)
		return _result;

	return _unique_copy(_first, _last, _result, _pred, _ITERATOR_CATEGORY(_result));
}

// unique 移除连续相同元素的后部分
template<typename _ForwardIter>
_ForwardIter unique(_ForwardIter _first, _ForwardIter _last)
{
	_first = adjacent_find(_first, _last);
	return unique_copy(_first, _last, _first);
}

template<typename _ForwardIter, typename _BinaryPredicate>
_ForwardIter unique(_ForwardIter _first, _ForwardIter _last, _BinaryPredicate _pred)
{
	_first = adjacent_find(_first, _last, _pred);
	return unique_copy(_first, _last, _first, _pred);
}

// reverse 元素反向

template<typename _BidirectionalIter>
void _reverse(_BidirectionalIter _first, _BidirectionalIter _last, bidirectional_iterator_tag)
{
	while (true)
	{
		if (_first == _last || _first == --_last)
			return;
		else
			iter_swap(_first++, --_last);
	}
}

template<typename _RandomAccessIter>
void _reverse(_RandomAccessIter _first, _RandomAccessIter _last, random_access_iterator_tag)
{
	while (_first < _last)
		iter_swap(_first++, --_last);
}

template<typename _BidirectionalIter>
void reverse(_BidirectionalIter _first, _BidirectionalIter _last)
{
	_reverse(_first, _last, _ITERATOR_CATEGORY(_first));
}

template<typename _BidirectionalIter, typename _OutputIter>
_OutputIter reverse_copy(_BidirectionalIter _first, _BidirectionalIter _last,
						 _OutputIter _result)
{
	while (_first != _last)
	{
		--_last;
		*_result = *_last;
		++_result;
	}
	return _result;
}

template<typename _EuclideanRingElement>
_EuclideanRingElement _gcb(_EuclideanRingElement _m, _EuclideanRingElement _n)
{
	while(_n != 0)
	{
		_EuclideanRingElement _tmp = _m % _n;
		_m = _n;
		_n = _tmp;
	}
	return _m;
}

// rotate 以middle为中心进行旋转, 将两边的值互换. 
// 若成功进行旋转, 则返回值为原容器的首元素现在所在的位置
template<typename _ForwardIter, typename _Distance>
_ForwardIter _rotate(_ForwardIter _first, _ForwardIter _middle,
					 _ForwardIter _last, _Distance *, forward_iterator_tag)
{
	if (_first == _middle)
		return _last;
	if (_last == _middle)
		return _first;

	_ForwardIter _first2 = _middle;
	do
	{
		swap(*_first++, *_first2++);
		if (_first == _middle)
			_middle = _first2;
	}
	while (_first2 != _last);

	_ForwardIter _new_middle = _first;
	_first2 = _middle;
	while (_first2 != _last)
	{
		swap(*_first++, *_first2++);
		if (_first == _middle)
			_middle = _first2;
		else if (_first2 == _last)
			_first2 = _middle;
	}

	return _new_middle;
}

template <typename _BidirectionalIter, typename _Distance>
_BidirectionalIter _rotate(_BidirectionalIter _first, _BidirectionalIter _middle,
						   _BidirectionalIter _last, _Distance*, bidirectional_iterator_tag)
{
	if (_first == _middle)
		return _last;
	if (_last == _middle)
		return _first;

	_reverse(_first, _middle, bidirectional_iterator_tag());
	_reverse(_middle, _last, bidirectional_iterator_tag());

	while (_first != _middle && _middle != _last)
		swap(*_first++, *--_last);

	if (_first == _middle)
	{
		_reverse(_middle, _last, bidirectional_iterator_tag());
		return _last;
	}
	else
	{
		_reverse(_first, _middle, bidirectional_iterator_tag());
		return _first;
	}
}

template <typename _RandomAccessIter, typename _Distance, typename _Tp>
_RandomAccessIter _rotate(_RandomAccessIter _first,
						  _RandomAccessIter _middle,
						  _RandomAccessIter _last,
						  _Distance *, _Tp *)
{
	_Distance _len = _last - _first;
	_Distance _front_len = _middle - _first;
	_Distance _other_len = _len - _front_len;
	_RandomAccessIter _result = _first + (_last - _middle);

	if (_front_len == 0)
		return _last;
	else if (_front_len == _other_len) 
	{
		swap_ranges(_first, _middle, _middle);
		return _result;
	}

	_Distance _d = _gcd(_len, _front_len);

	for (_Distance _i = 0; _i < _d; ++_i)
	{
		_Tp _tmp = *_first;
		_RandomAccessIter _p = _first;

		if (_front_len < _other_len)
		{
			for (_Distance _j = 0; _j < _other_len / _d; ++_j)
			{
				if (_p > _first + _other_len)
				{
					*_p = *(_p - _other_len);
					_p -= _other_len;
				}

				*_p = *(_p + _front_len);
				_p += _front_len;
			}
		}

		else
		{
			for (_Distance _j = 0; _j < _front_len / _d - 1; ++_j)
			{
				if (_p < _last - _front_len)
				{
					*_p = *(_p + _front_len);
					_p += _front_len;
				}

				*_p = *(_p - _other_len);
				_p -= _other_len;
			}
		}

		*_p = _tmp;
		++_first;
	}

	return _result;
}

template <typename _ForwardIter>
_ForwardIter rotate(_ForwardIter _first, _ForwardIter _middle,
					_ForwardIter _last)
{
	return _rotate(_first, _middle, _last, _DISTANCE_TYPE(_first), _ITERATOR_CATEGORY(_first));
}

template <typename _ForwardIter, typename _OutputIter>
_OutputIter rotate_copy(_ForwardIter _first, _ForwardIter _middle, 
						_ForwardIter _last, _OutputIter _result)
{
	return copy(_first, _middle, copy(_middle, _last, _result)); 
}


template<typename _Distance>
_Distance _random_number(_Distance _n)
{
	return rand() % _n;
}

template<typename _RandomAccessIter>
void random_shuffle(_RandomAccessIter _first, _RandomAccessIter _last)
{
	if (_first == _last)
		return;

	for (_RandomAccessIter _it = _first + 1; _it != _last; ++_it)
		iter_swap(_it, _first + _random_number((_it - _first) + 1));
}

template<typename _RandomAccessIter, typename _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter _first, _RandomAccessIter _last, _RandomNumberGenerator &_rand)
{
	if (_first == _last)
		return;

	for (_RandomAccessIter _it = _first + 1; _it != _last; ++_it)
		iter_swap(_it, _first + _rand((_it - _first) + 1));
}

// partition 将符合条件的元素划分到一起。返回最后一个符合条件的元素的后一个位置
template<typename _ForwardIter, typename _Predicate>
_ForwardIter _partition(_ForwardIter _first, _ForwardIter _last,
						_Predicate _pred, forward_iterator_tag)
{
	if (_first == _last)
		return _first;

	while (_pred(*_first))
	{
		if (++_first == _last)
			return _first;
	}

	_ForwardIter _next = _first;
	while (++_next != _last)
	{
		if(_pred(*_next))
		{
			swap(*_first, *_next);
			++_first;
		}
	}
	return _first;
}

template<typename _BidirectionalIter, typename _Predicate>
_BidirectionalIter _partition(_BidirectionalIter _first, _BidirectionalIter _last, 
							  _Predicate _pred, bidirectional_iterator_tag)
{
	while (true)
	{
		while (true)
		{
			if (_first == _last)
				return _first;
			else if (_pred(*_first))
				++_first;
			else
				break;
		}
		--_last;

		while (true)
		{
			if (_first == _last)
				return _first;
			else if (!_pred(*_last))
				--_last;
			else
				break;
		}
		iter_swap(_first, _last);
		++_first;
	}	
}

template<typename _ForwardIter, typename _Predicate>
_ForwardIter partition(_ForwardIter _first, _ForwardIter _last,
	_Predicate _pred)
{
	return _partition(_first, _last, _pred, _ITERATOR_CATEGORY(_first));
}


template <typename _RandomAccessIter, typename _Tp>
_RandomAccessIter _unguarded_partition(_RandomAccessIter _first, _RandomAccessIter _last,
									   _Tp _pivot)
{
	while (true) 
	{
		while (*_first < _pivot)
			++_first;

		--_last;
		while (_pivot < *_last)
			--_last;

		if (!(_first < _last))
			return _first;

		iter_swap(_first, _last);
		++_first;
	}
}

template <typename _ForwardIter, typename _Predicate, typename _Distance>
_ForwardIter _inplace_stable_partition(_ForwardIter _first,
	_ForwardIter _last, _Predicate _pred, _Distance _len)
{
	if (_len == 1)
		return _pred(*_first) ? _last : _first;

	_ForwardIter _middle = _first;
	advance(_middle, _len / 2);
	return rotate(_inplace_stable_partition(_first, _middle, _pred, _len / 2), _middle,
		_inplace_stable_partition(_middle, _last, _pred, _len - _len / 2));
}

template <typename _ForwardIter, typename _Pointer, typename _Predicate, typename _Distance>
_ForwardIter _stable_partition_adaptive(_ForwardIter _first, _ForwardIter _last,
		_Predicate _pred, _Distance _len,
		_Pointer _buffer, _Distance _buffer_size)
{
	if (_len <= _buffer_size)
	{
		_ForwardIter _result1 = _first;
		_Pointer _result2 = _buffer;
		for (; _first != _last; ++_first)
		{
			if (_pred(*_first))
			{
				*_result1 = *_first;
				++_result1;
			}
			else
			{
				*_result2 = *_first;
				++_result2;
			}
		}
		copy(_buffer, _result2, _result1);
		return _result1;
	}
	else 
	{
		_ForwardIter _middle = _first;
		advance(_middle, _len / 2);

		return rotate(_stable_partition_adaptive( _first, _middle, _pred,
			_len / 2, _buffer, _buffer_size), _middle,
			_stable_partition_adaptive( _middle, _last, _pred,
				_len - _len / 2, _buffer, _buffer_size));
	}
}

template <typename _ForwardIter, typename _Predicate, typename _Tp, typename _Distance>
inline _ForwardIter
_stable_partition_aux(_ForwardIter _first, _ForwardIter _last,
	_Predicate _pred, _Tp*, _Distance*)
{
	_Temporary_buffer<_ForwardIter, _Tp> _buf(_first, _last);

	if (_buf.size() > 0)
		return _stable_partition_adaptive(_first, _last, _pred,
			_Distance(_buf.requested_size()), _buf.begin(), _buf.size());
	else
		return _inplace_stable_partition(_first, _last, _pred, _Distance(_buf.requested_size()));
}

template <typename _ForwardIter, typename _Predicate>
inline _ForwardIter stable_partition(_ForwardIter _first,
	_ForwardIter _last,
	_Predicate _pred)
{
	if (_first == _last)
		return _first;
	else
		return _stable_partition_aux(_first, _last, _pred,
			_VALUE_TYPE(_first), _DISTANCE_TYPE(_first));
}

template <typename _RandomAccessIter, typename _Tp, typename _Compare>
_RandomAccessIter _unguarded_partition(_RandomAccessIter _first,
	_RandomAccessIter _last, _Tp _pivot, _Compare _comp)
{
	while (true)
	{
		while (_comp(*_first, _pivot))
			++_first;

		--_last;
		while (_comp(_pivot, *_last))
			--_last;

		if (!(_first < _last))
			return _first;

		iter_swap(_first, _last);
		++_first;
	}
}

// sort 及其辅助函数

const int _stl_threshold = 16;// 通过这个基数与(_last - first)的大小比较觉得其排序的方式

template<typename _RandomAccessIter, typename _Tp>
void _unguarded_linear_insert(_RandomAccessIter _last, _Tp _val)
{
	_RandomAccessIter _next = _last;
	--_next;
	while (_val < *_next)
	{
		*_last = *_next;
		_last = _next;
		--_next;
	}
	*_last = _val;
}


template<typename _RandomAccessIter, typename _Tp, typename _Compare>
void _unguarded_linear_insert(_RandomAccessIter _last, _Tp _val, _Compare _com)
{
	_RandomAccessIter _next = _last;
	--_next;
	while (_com(_val, *_next))
	{
		*_last = *_next;
		_last = _next;
		--_next;
	}
	*_last = _val;
}

template<typename _RandomAccessIter, typename _Tp>
void _linear_insert(_RandomAccessIter _first, _RandomAccessIter _last, _Tp*)
{
	_Tp _val = *_last;
	if (_val < *_first)
	{
		copy_backward(_first, _last, _last + 1);
		*_first = _val;
	}
	else
		_unguarded_linear_insert(_last, _val);
}

template<typename _RandomAccessIter, typename _Tp, typename _Compare>
void _linear_insert(_RandomAccessIter _first, _RandomAccessIter _last, _Tp*, _Compare _com)
{
	_Tp _val = *_last;
	if (_com(_val, *_first))
	{
		copy_backward(_first, _last, _last + 1);
		*_first = _val;
	}
	else
		_unguarded_linear_insert(_last, _val, _com);
}

template<typename _RandomAccessIter>
void _insertion_sort(_RandomAccessIter _fitst, _RandomAccessIter _last)
{
	if (_fitst == _last)
		return;

	for (_RandomAccessIter _it = _fitst + 1; _it != _last; ++_it)
		_linear_insert(_fitst, _it, _VALUE_TYPE(_fitst));
}

template<typename _RandomAccessIter, typename _Compare>
void _insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _com)
{
	if(_first == _last)
		return;

	for (_RandomAccessIter _it = _first + 1; _it != _last; ++_it)
		_linear_insert(_first, _it, _VALUE_TYPE(_first), _com);
}

template<typename _RandomAccessIter, typename _Tp>
void _unguarded_insertion_sort_aux(_RandomAccessIter _first, _RandomAccessIter _last, _Tp*)
{
	for (_RandomAccessIter _it = _first; _it != _last; ++_it)
		_unguarded_linear_insert(_it, _Tp(*_it));
}

template<typename _RandomAccessIter>
void _unguarded_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last)
{
	_unguarded_insertion_sort_aux(_first, _last, _VALUE_TYPE(_first));
}

template<typename _RandomAccessIter, typename _Tp, typename _Compare>
void _unguarded_insertion_sort_aux(_RandomAccessIter _first, _RandomAccessIter _last, 
								   _Tp*, _Compare _com)
{
	for (_RandomAccessIter _it = *_first; _it != _last; ++_it)
		_unguarded_linear_insert(_it, _Tp(*_it), _com);
}

template<typename _RandomAccessIter, typename _Compare>
void _unguarded_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _com)
{
	_unguarded_insertion_sort_aux(_first, _last, _VALUE_TYPE(_first), _com);
}

template<typename _RandomAccessIter>
void _final_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last)
{
	if (_last - _first > _stl_threshold)
	{
		_insertion_sort(_first, _first + _stl_threshold);
		_unguarded_insertion_sort(_first + _stl_threshold, _last);
	}
	else
		_insertion_sort(_first, _last);
}

template<typename _RandomAccessIter, typename _Compare>
void _final_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _com)
{
	if (_last - _first > _stl_threshold)
	{
		_insertion_sort(_first, _first + _stl_threshold, _com);
		_unguarded_insertion_sort(_first + _stl_threshold, _last, _com);
	}
	else
		_insertion_sort(_first, _last, _com);
}

template<typename _Size>
_Size _lg(_Size _n)
{
	_Size _ret;
	for (_ret = 0; _n != 1; _n >>= 1)
		++_ret;

	return _ret;
}



template<typename _RandomAccessIter, typename _Tp, typename _Size>
void _introsort_loop(_RandomAccessIter _first, _RandomAccessIter _last, 
					 _Tp*, _Size _depth_limit)
{
	while (_last - _first > _stl_threshold)
	{
		if(_depth_limit == 0)
		{
			partial_sort(_first, _last, _last);
			return;
		}
		--_depth_limit;
		_RandomAccessIter _cut = _unguarded_partition(_first, _last, _Tp(_median(*_first,
			*(_first + (_last - _first) / 2), *(_last - 1))) );
		_introsort_loop(_cut, _last, (_Tp*)0, _depth_limit);
		_last = _cut;
	}
}

template<typename _RandomAccessIter, typename _Tp, typename _Size, typename _Compare>
void _introsort_loop(_RandomAccessIter _first, _RandomAccessIter _last,
					 _Tp*, _Size _depth_limit, _Compare _comp)
{
	while (_last - _first > _stl_threshold)
	{
		if (_depth_limit == 0)
		{
			partial_sort(_first, _last, _last, _comp);
			return;
		}
		--_depth_limit;
		_RandomAccessIter _cut = _unguarded_partition(_first, _last, _Tp(_median(*_first,
			*(_first + (_last - _first) / 2), *(_last - 1), _comp)), _comp);

		_introsort_loop(_cut, _last, (_Tp*)0, _depth_limit, _comp);
		_last = _cut;
	}
}

template<typename _RandomAccessIter>
void sort(_RandomAccessIter _first, _RandomAccessIter _last)
{
	if(_first != _last)
	{
		_introsort_loop(_first, _last, _VALUE_TYPE(_first), _lg(_last - _first) * 2);
		_final_insertion_sort(_first, _last);
	}
}

template<typename _RandomAccessIter, typename _Compare>
void sort(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _comp)
{
	if (_first != _last)
	{
		_introsort_loop(_first, _last, _VALUE_TYPE(_first), _lg(_last - _first) * 2, _comp);
		_final_insertion_sort(_first, _last, _comp);
	}
}

template<typename _RandomAccessIter>
void _inplace_stable_sort(_RandomAccessIter _first, _RandomAccessIter _last)
{
	if (_last - _first < _stl_threshold - 1)
	{
		_insertion_sort(_first, _last);
		return;
	}

	_RandomAccessIter _middle = _first + (_last - _first) / 2;
	_insertion_sort(_first, _middle);
	_insertion_sort(_middle, _last);
	_merge_without_buffer(_first, _middle, _last, _middle - _first, _last - _middle);
}

template <typename _RandomAccessIter, typename _Compare>
void _inplace_stable_sort(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _comp)
{
	if (_last - _first < 15)
	{
		_insertion_sort(_first, _last, _comp);
		return;
	}
	_RandomAccessIter _middle = _first + (_last - _first) / 2;
	_inplace_stable_sort(_first, _middle, _comp);
	_inplace_stable_sort(_middle, _last, _comp);
	_merge_without_buffer(_first, _middle, _last,
		_middle - _first, _last - _middle, _comp);
}

template <typename _RandomAccessIter1, typename _RandomAccessIter2, typename _Distance>
void _merge_sort_loop(_RandomAccessIter1 _first, _RandomAccessIter1 _last,
					  _RandomAccessIter2 _result, _Distance _step_size)
{
	_Distance _two_step = 2 * _step_size;

	while (_last - _first >= _two_step)
	{
		_result = merge(_first, _first + _step_size,
			_first + _step_size, _first + _two_step, _result);
		_first += _two_step;
	}

	_step_size = min(_Distance(_last - _first), _step_size);
	merge(_first, _first + _step_size, _first + _step_size, _last, _result);
}

template <typename _RandomAccessIter1, typename _RandomAccessIter2,
	typename _Distance, typename _Compare>
void _merge_sort_loop(_RandomAccessIter1 _first, _RandomAccessIter1 _last,
					  _RandomAccessIter2 _result, _Distance _step_size, _Compare _comp)
{
	_Distance _two_step = 2 * _step_size;

	while (_last - _first >= _two_step)
	{
		_result = merge(_first, _first + _step_size,
			_first + _step_size, _first + _two_step,
			_result, _comp);
		_first += _two_step;
	}
	_step_size = min(_Distance(_last - _first), _step_size);

	merge(_first, _first + _step_size,
		_first + _step_size, _last,
		_result, _comp);
}

const int _stl_chunk_size = 7;

template <typename _RandomAccessIter, typename _Distance>
void _chunk_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last, _Distance _chunk_size)
{
	while (_last - _first >= _chunk_size)
	{
		_insertion_sort(_first, _first + _chunk_size);
		_first += _chunk_size;
	}
	_insertion_sort(_first, _last);
}

template <typename _RandomAccessIter, typename _Distance, typename _Compare>
void _chunk_insertion_sort(_RandomAccessIter _first, _RandomAccessIter _last,
						   _Distance _chunk_size, _Compare _comp)
{
	while (_last - _first >= _chunk_size)
	{
		_insertion_sort(_first, _first + _chunk_size, _comp);
		_first += _chunk_size;
	}
	_insertion_sort(_first, _last, _comp);
}

template <typename _RandomAccessIter, typename _Pointer, typename _Distance>
void _merge_sort_with_buffer(_RandomAccessIter _first, _RandomAccessIter _last,
							 _Pointer _buffer, _Distance*)
{
	_Distance _len = _last - _first;
	_Pointer _buffer_last = _buffer + _len;

	_Distance _step_size = _stl_chunk_size;
	_chunk_insertion_sort(_first, _last, _step_size);

	while (_step_size < _len)
	{
		_merge_sort_loop(_first, _last, _buffer, _step_size);
		_step_size *= 2;
		_merge_sort_loop(_buffer, _buffer_last, _first, _step_size);
		_step_size *= 2;
	}
}

template <typename _RandomAccessIter, typename _Pointer, typename _Distance, typename _Compare>
void _merge_sort_with_buffer(_RandomAccessIter _first,
		_RandomAccessIter _last, _Pointer _buffer,
		_Distance*, _Compare _comp)
{
	_Distance _len = _last - _first;
	_Pointer _buffer_last = _buffer + _len;

	_Distance _step_size = _stl_chunk_size;
	_chunk_insertion_sort(_first, _last, _step_size, _comp);

	while (_step_size < _len)
	{
		_merge_sort_loop(_first, _last, _buffer, _step_size, _comp);
		_step_size *= 2;
		_merge_sort_loop(_buffer, _buffer_last, _first, _step_size, _comp);
		_step_size *= 2;
	}
}

template <typename _RandomAccessIter, typename _Pointer, typename _Distance>
void _stable_sort_adaptive(_RandomAccessIter _first, _RandomAccessIter _last,
	_Pointer _buffer, _Distance _buffer_size)
{
	_Distance _len = (_last - _first + 1) / 2;
	_RandomAccessIter _middle = _first + _len;
	if (_len > _buffer_size) 
	{
		_stable_sort_adaptive(_first, _middle, _buffer, _buffer_size);
		_stable_sort_adaptive(_middle, _last, _buffer, _buffer_size);
	}
	else
	{
		_merge_sort_with_buffer(_first, _middle, _buffer, (_Distance*)0);
		_merge_sort_with_buffer(_middle, _last, _buffer, (_Distance*)0);
	}
	_merge_adaptive(_first, _middle, _last, _Distance(_middle - _first),
		_Distance(_last - _middle), _buffer, _buffer_size);
}

template <typename _RandomAccessIter, typename _Pointer, typename _Distance, typename _Compare>
void _stable_sort_adaptive(_RandomAccessIter _first,
		_RandomAccessIter _last, _Pointer _buffer,
		_Distance _buffer_size, _Compare _comp)
{
	_Distance _len = (_last - _first + 1) / 2;
	_RandomAccessIter _middle = _first + _len;
	if (_len > _buffer_size)
	{
		_stable_sort_adaptive(_first, _middle, _buffer, _buffer_size, _comp);
		_stable_sort_adaptive(_middle, _last, _buffer, _buffer_size, _comp);
	}
	else
	{
		_merge_sort_with_buffer(_first, _middle, _buffer, (_Distance*)0, _comp);
		_merge_sort_with_buffer(_middle, _last, _buffer, (_Distance*)0, _comp);
	}
	_merge_adaptive(_first, _middle, _last, _Distance(_middle - _first),
		_Distance(_last - _middle), _buffer, _buffer_size, _comp);
}

template <typename _RandomAccessIter, typename _Tp, typename _Distance>
inline void _stable_sort_aux(_RandomAccessIter _first,
	_RandomAccessIter _last, _Tp*, _Distance*)
{
	_Temporary_buffer<_RandomAccessIter, _Tp> _buf(_first, _last);
	if (_buf.begin() == 0)
		_inplace_stable_sort(_first, _last);
	else
		_stable_sort_adaptive(_first, _last, _buf.begin(), _Distance(_buf.size()));
}

template <typename _RandomAccessIter, typename _Tp, typename _Distance, typename _Compare>
inline void _stable_sort_aux(_RandomAccessIter _first,
	_RandomAccessIter _last, _Tp*, _Distance*, _Compare _comp)
{
	_Temporary_buffer<_RandomAccessIter, _Tp> _buf(_first, _last);
	if (_buf.begin() == 0)
		_inplace_stable_sort(_first, _last, _comp);
	else
		_stable_sort_adaptive(_first, _last, _buf.begin(),
			_Distance(_buf.size()), _comp);
}

template <typename _RandomAccessIter>
inline void stable_sort(_RandomAccessIter _first,
	_RandomAccessIter _last) 
{
	_stable_sort_aux(_first, _last, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first));
}

template <typename _RandomAccessIter, typename _Compare>
inline void stable_sort(_RandomAccessIter _first,
	_RandomAccessIter _last, _Compare _comp)
{	
	_stable_sort_aux(_first, _last, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first), _comp);
}

// partial_sort

template <typename _RandomAccessIter, typename _Tp>
void _partial_sort(_RandomAccessIter _first, _RandomAccessIter _middle,
	_RandomAccessIter _last, _Tp*)
{
	make_heap(_first, _middle);
	for (_RandomAccessIter _it = _middle; _it < _last; ++_it)
	{
		if (*_it < *_first)
			_pop_heap(_first, _middle, _it, _Tp(*_it), _DISTANCE_TYPE(_first));
	}
	sort_heap(_first, _middle);
}

template <typename _RandomAccessIter>
void partial_sort(_RandomAccessIter _first,
	_RandomAccessIter _middle,
	_RandomAccessIter _last)
{
	_partial_sort(_first, _middle, _last, _VALUE_TYPE(_first));
}

template <typename _RandomAccessIter, typename _Tp, typename _Compare>
void _partial_sort(_RandomAccessIter _first, _RandomAccessIter _middle,
	_RandomAccessIter _last, _Tp*, _Compare _comp)
{
	make_heap(_first, _middle, _comp);
	for (_RandomAccessIter _it = _middle; _it < _last; ++_it)
	{
		if (_comp(*_it, *_first))
			_pop_heap(_first, _middle, _it, _Tp(*_it), _comp, _DISTANCE_TYPE(_first));
	}
	sort_heap(_first, _middle, _comp);
}

template <typename _RandomAccessIter, typename _Compare>
inline void partial_sort(_RandomAccessIter _first, _RandomAccessIter _middle,
	_RandomAccessIter _last, _Compare _comp)
{
	_partial_sort(_first, _middle, _last, _VALUE_TYPE(_first), _comp);
}

template <typename _InputIter, typename _RandomAccessIter, typename _Distance, typename _Tp>
_RandomAccessIter _partial_sort_copy(_InputIter _first, _InputIter _last,
		_RandomAccessIter _result_first, _RandomAccessIter _result_last,
		_Distance*, _Tp*) 
{
	if (_result_first == _result_last) 
		return _result_last;

	_RandomAccessIter _result_real_last = _result_first;
	while (_first != _last && _result_real_last != _result_last)
	{
		*_result_real_last = *_first;
		++_result_real_last;
		++_first;
	}
	make_heap(_result_first, _result_real_last);
	while (_first != _last) 
	{
		if (*_first < *_result_first)
		{
			_adjust_heap(_result_first, _Distance(0),
				_Distance(_result_real_last - _result_first),
				_Tp(*_first));
		}
		++_first;
	}
	sort_heap(_result_first, _result_real_last);
	return _result_real_last;
}

template <typename _InputIter, typename _RandomAccessIter>
inline _RandomAccessIter
partial_sort_copy(_InputIter _first, _InputIter _last,
	_RandomAccessIter _result_first,
	_RandomAccessIter _result_last)
{
	return _partial_sort_copy(_first, _last, _result_first, _result_last,
		_DISTANCE_TYPE(_result_first), _VALUE_TYPE(_first));
}

template <typename _InputIter, typename _RandomAccessIter, typename _Compare,
	typename _Distance, typename _Tp>
_RandomAccessIter _partial_sort_copy(_InputIter _first,
		_InputIter _last,
		_RandomAccessIter _result_first,
		_RandomAccessIter _result_last,
		_Compare _comp, _Distance*, _Tp*)
{
	if (_result_first == _result_last) 
		return _result_last;

	_RandomAccessIter _result_real_last = _result_first;
	while (_first != _last && _result_real_last != _result_last)
	{
		*_result_real_last = *_first;
		++_result_real_last;
		++_first;
	}
	make_heap(_result_first, _result_real_last, _comp);
	while (_first != _last)
	{
		if (_comp(*_first, *_result_first))
		{
			_adjust_heap(_result_first, _Distance(0),
				_Distance(_result_real_last - _result_first), _Tp(*_first), _comp);
		}
		++_first;
	}
	sort_heap(_result_first, _result_real_last, _comp);
	return _result_real_last;
}

template <typename _InputIter, typename _RandomAccessIter, typename _Compare>
inline _RandomAccessIter
partial_sort_copy(_InputIter _first, _InputIter _last,
	_RandomAccessIter _result_first, _RandomAccessIter _result_last, _Compare _comp) 
{
	
	return _partial_sort_copy(_first, _last, _result_first, _result_last,
		_comp, _DISTANCE_TYPE(_result_first), _VALUE_TYPE(_first));
}


template <typename _RandomAccessIter, typename _Tp>
void _nth_element(_RandomAccessIter _first, _RandomAccessIter _nth,
	_RandomAccessIter _last, _Tp*) 
{
	while (_last - _first > 3) 
	{
		_RandomAccessIter __cut =
			_unguarded_partition(_first, _last, _Tp(__median(*_first,
					*(_first + (_last - _first) / 2), *(_last - 1))));

		if (__cut <= _nth)
			_first = __cut;
		else
			_last = __cut;
	}
	_insertion_sort(_first, _last);
}

template <typename _RandomAccessIter>
inline void nth_element(_RandomAccessIter _first, _RandomAccessIter _nth,
	_RandomAccessIter _last) 
{
	_nth_element(_first, _nth, _last, _VALUE_TYPE(_first));
}

template <typename _RandomAccessIter, typename _Tp, typename _Compare>
void _nth_element(_RandomAccessIter _first, _RandomAccessIter _nth,
	_RandomAccessIter _last, _Tp*, _Compare _comp)
{
	while (_last - _first > 3) 
	{
		_RandomAccessIter __cut =
			_unguarded_partition(_first, _last,
				_Tp(__median(*_first, *(_first + (_last - _first) / 2),
					*(_last - 1), _comp)), _comp);

		if (__cut <= _nth)
			_first = __cut;
		else
			_last = __cut;
	}
	_insertion_sort(_first, _last, _comp);
}

template <typename _RandomAccessIter, typename _Compare>
inline void nth_element(_RandomAccessIter _first, _RandomAccessIter _nth,
	_RandomAccessIter _last, _Compare _comp) 
{
	_nth_element(_first, _nth, _last, _VALUE_TYPE(_first), _comp);
}

template <typename _ForwardIter, typename _Tp, typename _Distance>
_ForwardIter _lower_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle;

	while (_len > 0) 
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (*_middle < _val) 
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
		else
			_len = _half;
	}
	return _first;
}

template <typename _ForwardIter, typename _Tp>
inline _ForwardIter lower_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val) 
{
	return _lower_bound(_first, _last, _val, _DISTANCE_TYPE(_first));
}

template <typename _ForwardIter, typename _Tp, typename _Compare, typename _Distance>
_ForwardIter _lower_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Compare _comp, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle;

	while (_len > 0) 
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (_comp(*_middle, _val))
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
		else
			_len = _half;
	}
	return _first;
}

template <typename _ForwardIter, typename _Tp, typename _Compare>
inline _ForwardIter lower_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Compare _comp) 
{
	return _lower_bound(_first, _last, _val, _comp,
		_DISTANCE_TYPE(_first));
}

template <typename _ForwardIter, typename _Tp, typename _Distance>
_ForwardIter _upper_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle;

	while (_len > 0) 
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (_val < *_middle)
			_len = _half;
		else 
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
	}
	return _first;
}

template <typename _ForwardIter, typename _Tp>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
	const _Tp& __val)
{
	return _upper_bound(__first, __last, __val, _DISTANCE_TYPE(__first));
}

template <typename _ForwardIter, typename _Tp, typename _Compare, typename _Distance>
_ForwardIter _upper_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Compare _comp, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle;

	while (_len > 0)
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (_comp(_val, *_middle))
			_len = _half;
		else
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
	}
	return _first;
}

template <typename _ForwardIter, typename _Tp, typename _Compare>
inline _ForwardIter upper_bound(_ForwardIter _first, _ForwardIter _last,
	const _Tp& _val, _Compare _comp)
{
	return _upper_bound(_first, _last, _val, _comp, _DISTANCE_TYPE(_first));
}

template<typename _ForwardIter, typename _Tp, typename _Distance>
pair<_ForwardIter, _ForwardIter> _equal_range(
					_ForwardIter _first, _ForwardIter _last,
					const _Tp &_val, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle, _left, _right;

	while (_len > 0)
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (*_middle < _val)
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
		else if (_val < *_middle)
			_len = _half;
		else
		{
			_left = lower_bound(_first, _middle, _val);
			advance(_first, _len);
			_right = upper_bound(++_middle, _first, _val);
			return pair<_ForwardIter, _ForwardIter>(_left, _right);
		}
	}
	return pair<_ForwardIter, _ForwardIter>(_first, _first);
}

template <typename _ForwardIter, typename _Tp>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter _first, _ForwardIter _last, const _Tp& _val)
{
	return _equal_range(_first, _last, _val, _DISTANCE_TYPE(_first));
}

template<typename _ForwardIter, typename _Tp, typename _Compare, typename _Distance>
pair<_ForwardIter, _ForwardIter> _equal_range(
	_ForwardIter _first, _ForwardIter _last,
	const _Tp &_val, _Compare _comp, _Distance*)
{
	_Distance _len = 0;
	distance(_first, _last, _len);
	_Distance _half;
	_ForwardIter _middle, _left, _right;

	while (_len > 0)
	{
		_half = _len >> 1;
		_middle = _first;
		advance(_middle, _half);
		if (_comp(*_middle, _val))
		{
			_first = _middle;
			++_first;
			_len = _len - _half - 1;
		}
		else if (_comp(_val, *_middle))
			_len = _half;
		else
		{
			_left = lower_bound(_first, _middle, _val, _comp);
			advance(_first, _len);
			_right = upper_bound(++_middle, _first, _val, _comp);
			return pair<_ForwardIter, _ForwardIter>(_left, _right);
		}
	}
	return pair<_ForwardIter, _ForwardIter>(_first, _first);
}

template <typename _ForwardIter, typename _Tp, typename _Compare>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter _first, _ForwardIter _last, const _Tp& _val, _Compare _comp)
{
	return _equal_range(_first, _last, _val, _comp, _DISTANCE_TYPE(_first));
}

template<typename _ForwardIter, typename _Tp>
bool binary_search(_ForwardIter _first, _ForwardIter _last, const _Tp &_val)
{
	_ForwardIter _it = lower_bound(_first, _last, _val);
	return _it != _last && !(_val < *_it);
}

template<typename _ForwardIter, typename _Tp, typename _Compare>
bool binary_search(_ForwardIter _first, _ForwardIter _last, const _Tp &_val, _Compare _comp)
{
	_ForwardIter _it = lower_bound(_first, _last, _val, _comp);
	return _it != _last && !_comp(_val, *_it);
}

template<typename _InputIter1, typename _InputIter2, typename _OutputIter>
_OutputIter merge(_InputIter1 _first1, _InputIter1 _last1,
				  _InputIter2 _first2, _InputIter2 _last2,
				  _OutputIter _result)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if(*_first2 < *_first1)
		{
			*_result = *_first2;
			++_first2;
		}
		else
		{
			*_result = *_first1;
			++_first1;
		}
		++_result;
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}

template<typename _InputIter1, typename _InputIter2, 
		 typename _OutputIter, typename _Compare>
_OutputIter merge(_InputIter1 _first1, _InputIter1 _last1,
				  _InputIter2 _first2, _InputIter2 _last2,
				  _OutputIter _result, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp(*_first2, *_first1))
		{
			*_result = *_first2;
			++_first2;
		}
		else
		{
			*_result = *_first1;
			++_first1;
		}
		++_result;
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}

template <typename _BidirectionalIter, typename _Distance>
void _merge_without_buffer(_BidirectionalIter _first, _BidirectionalIter _middle,
						   _BidirectionalIter _last, _Distance _len1, _Distance _len2)
{
	if (_len1 == 0 || _len2 == 0)
		return;
	if (_len1 + _len2 == 2) {
		if (*_middle < *_first)
			iter_swap(_first, _middle);
		return;
	}
	_BidirectionalIter _first_cut = _first;
	_BidirectionalIter _second_cut = _middle;
	_Distance _len11 = 0;
	_Distance _len22 = 0;
	if (_len1 > _len2) 
	{
		_len11 = _len1 / 2;
		advance(_first_cut, _len11);
		_second_cut = lower_bound(_middle, _last, *_first_cut);
		distance(_middle, _second_cut, _len22);
	}
	else
	{
		_len22 = _len2 / 2;
		advance(_second_cut, _len22);
		_first_cut = upper_bound(_first, _middle, *_second_cut);
		distance(_first, _first_cut, _len11);
	}
	_BidirectionalIter __new_middle = rotate(_first_cut, _middle, _second_cut);
	_merge_without_buffer(_first, _first_cut, __new_middle, _len11, _len22);
	_merge_without_buffer(__new_middle, _second_cut, _last, _len1 - _len11, _len2 - _len22);
}

template <typename _BidirectionalIter, typename _Distance, typename _Compare>
void _merge_without_buffer(_BidirectionalIter _first, _BidirectionalIter _middle,
	_BidirectionalIter _last, _Distance _len1, _Distance _len2,
	_Compare _comp)
{
	if (_len1 == 0 || _len2 == 0)
		return;
	if (_len1 + _len2 == 2) {
		if (_comp(*_middle, *_first))
			iter_swap(_first, _middle);
		return;
	}
	_BidirectionalIter _first_cut = _first;
	_BidirectionalIter _second_cut = _middle;
	_Distance _len11 = 0;
	_Distance _len22 = 0;
	if (_len1 > _len2)
	{
		_len11 = _len1 / 2;
		advance(_first_cut, _len11);
		_second_cut = lower_bound(_middle, _last, *_first_cut, _comp);
		distance(_middle, _second_cut, _len22);
	}
	else
	{
		_len22 = _len2 / 2;
		advance(_second_cut, _len22);
		_first_cut = upper_bound(_first, _middle, *_second_cut, _comp);
		distance(_first, _first_cut, _len11);
	}
	_BidirectionalIter _new_middle = rotate(_first_cut, _middle, _second_cut);
	_merge_without_buffer(_first, _first_cut, _new_middle, _len11, _len22, _comp);
	_merge_without_buffer(_new_middle, _second_cut, _last, _len1 - _len11, _len2 - _len22, _comp);
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _Distance>
_BidirectionalIter1 _rotate_adaptive(
		_BidirectionalIter1 _first, _BidirectionalIter1 _middle,
		_BidirectionalIter1 _last, _Distance _len1, _Distance _len2,
		_BidirectionalIter2 _buffer, _Distance _buffer_size)
{
	_BidirectionalIter2 _buffer_end;
	if (_len1 > _len2 && _len2 <= _buffer_size)
	{
		_buffer_end = copy(_middle, _last, _buffer);
		copy_backward(_first, _middle, _last);
		return copy(_buffer, _buffer_end, _first);
	}
	else if (_len1 <= _buffer_size)
	{
		_buffer_end = copy(_first, _middle, _buffer);
		copy(_middle, _last, _first);
		return copy_backward(_buffer, _buffer_end, _last);
	}
	else
		return rotate(_first, _middle, _last);
}


template <typename _BidirectionalIter1, typename _BidirectionalIter2,
		  typename _BidirectionalIter3>
_BidirectionalIter3 _merge_backward(
		_BidirectionalIter1 _first1, _BidirectionalIter1 _last1,
		_BidirectionalIter2 _first2, _BidirectionalIter2 _last2,
		_BidirectionalIter3 _result)
{
	if (_first1 == _last1)
		return copy_backward(_first2, _last2, _result);
	if (_first2 == _last2)
		return copy_backward(_first1, _last1, _result);

	--_last1;
	--_last2;
	while (true) 
	{
		if (*_last2 < *_last1) 
		{
			*--_result = *_last1;
			if (_first1 == _last1)
				return copy_backward(_first2, ++_last2, _result);

			--_last1;
		}
		else 
		{
			*--_result = *_last2;
			if (_first2 == _last2)
				return copy_backward(_first1, ++_last1, _result);

			--_last2;
		}
	}
}


template <typename _BidirectionalIter1, typename _BidirectionalIter2,
		  typename _BidirectionalIter3, typename _Compare>
	_BidirectionalIter3 _merge_backward(
		_BidirectionalIter1 _first1, _BidirectionalIter1 _last1,
		_BidirectionalIter2 _first2, _BidirectionalIter2 _last2,
		_BidirectionalIter3 _result, _Compare _comp)
{
	if (_first1 == _last1)
		return copy_backward(_first2, _last2, _result);
	if (_first2 == _last2)
		return copy_backward(_first1, _last1, _result);

	--_last1;
	--_last2;
	while (true)
	{
		if (_comp(*_last2, *_last1))
		{
			*--_result = *_last1;
			if (_first1 == _last1)
				return copy_backward(_first2, ++_last2, _result);

			--_last1;
		}
		else
		{
			*--_result = *_last2;
			if (_first2 == _last2)
				return copy_backward(_first1, ++_last1, _result);

			--_last2;
		}
	}
}


template<typename _BidirectionalIter, typename _Distance, typename _Pointer>
void _merge_adaptive(_BidirectionalIter _first, _BidirectionalIter _middle,
			_BidirectionalIter _last, _Distance _len1, _Distance _len2,
			_Pointer _buffer, _Distance _buffer_size)
{
	if (_len1 <= _len2 && _len1 <= _buffer_size)
	{
		_Pointer _buffer_end = copy(_first, _middle, _buffer);
		merge(_buffer, _buffer_end, _middle, _last, _first);
	}
	else if (_len2 <= _buffer_size)
	{
		_Pointer _buffer_end = copy(_middle, _last, _buffer);
		_merge_backward(_first, _middle, _buffer, _buffer_end, _last);
	}
	else 
	{
		_BidirectionalIter _first_cut = _first;
		_BidirectionalIter _second_cut = _middle;
		_Distance _len11 = 0;
		_Distance _len22 = 0;
		if (_len1 > _len2) 
		{
			_len11 = _len1 / 2;
			advance(_first_cut, _len11);
			_second_cut = lower_bound(_middle, _last, *_first_cut);
			distance(_middle, _second_cut, _len22);
		}
		else 
		{
			_len22 = _len2 / 2;
			advance(_second_cut, _len22);
			_first_cut = upper_bound(_first, _middle, *_second_cut);
			distance(_first, _first_cut, _len11);
		}
		_BidirectionalIter _new_middle =
			_rotate_adaptive(_first_cut, _middle, _second_cut, _len1 - _len11,
				_len22, _buffer, _buffer_size);

		_merge_adaptive(_first, _first_cut, _new_middle, _len11,
			_len22, _buffer, _buffer_size);

		_merge_adaptive(_new_middle, _second_cut, _last, _len1 - _len11,
			_len2 - _len22, _buffer, _buffer_size);
	}
}

template<typename _BidirectionalIter, typename _Distance, 
		 typename _Pointer, typename _Compare>
void _merge_adaptive(_BidirectionalIter _first, _BidirectionalIter _middle,
	_BidirectionalIter _last, _Distance _len1, _Distance _len2,
	_Pointer _buffer, _Distance _buffer_size, _Compare _comp)
{
	if (_len1 <= _len2 && _len1 <= _buffer_size)
	{
		_Pointer _buffer_end = copy(_first, _middle, _buffer);
		merge(_buffer, _buffer_end, _middle, _last, _first, _comp);
	}
	else if (_len2 <= _buffer_size)
	{
		_Pointer _buffer_end = copy(_middle, _last, _buffer);
		_merge_backward(_first, _middle, _buffer, _buffer_end, _last, _comp);
	}
	else
	{
		_BidirectionalIter _first_cut = _first;
		_BidirectionalIter _second_cut = _middle;
		_Distance _len11 = 0;
		_Distance _len22 = 0;
		if (_len1 > _len2)
		{
			_len11 = _len1 / 2;
			advance(_first_cut, _len11);
			_second_cut = lower_bound(_middle, _last, *_first_cut, _comp);
			distance(_middle, _second_cut, _len22);
		}
		else
		{
			_len22 = _len2 / 2;
			advance(_second_cut, _len22);
			_first_cut = upper_bound(_first, _middle, *_second_cut, _comp);
			distance(_first, _first_cut, _len11);
		}
		_BidirectionalIter _new_middle =
			_rotate_adaptive(_first_cut, _middle, _second_cut, _len1 - _len11,
				_len22, _buffer, _buffer_size);

		_merge_adaptive(_first, _first_cut, _new_middle, _len11,
			_len22, _buffer, _buffer_size, _comp);

		_merge_adaptive(_new_middle, _second_cut, _last, _len1 - _len11,
			_len2 - _len22, _buffer, _buffer_size, _comp);
	}
}

template<typename _BidirectionalIter, typename _Tp, typename _Distance>
void _inplace_merge_aux(_BidirectionalIter _first, _BidirectionalIter _middle,
						_BidirectionalIter _last, _Tp*, _Distance*)
{
	_Distance _len1 = 0;
	distance(_first, _middle, _len1);
	_Distance _len2 = 0;
	distance(_middle, _last, _len2);

	_Temporary_buffer<_BidirectionalIter, _Tp> _buf(_first, _last);
	if (!_buf.begin())
		_merge_without_buffer(_first, _middle, _last, _len1, _len2);
	else
		_merge_adaptive(_first, _middle, _last, _len1, _len2, _buf.begin(), _Distance(_buf.size()));
}

template<typename _BidirectionalIter, typename _Tp, typename _Distance, typename _Compare>
void _inplace_merge_aux(_BidirectionalIter _first, _BidirectionalIter _middle,
				_BidirectionalIter _last, _Tp*, _Distance*, _Compare _comp)
{
	_Distance _len1 = 0;
	distance(_first, _middle, _len1);
	_Distance _len2 = 0;
	distance(_middle, _last, _len2);

	_Temporary_buffer<_BidirectionalIter, _Tp> _buf(_first, _last);
	if (!_buf.begin())
		_merge_without_buffer(_first, _middle, _last, _len1, _len2, _comp);
	else
		_merge_adaptive(_first, _middle, _last, _len1, _len2, _buf.begin(), _Distance(_buf.size()), _comp);
}

template<typename _BidirectionalIter, typename _Tp, typename _Distance>
void inplace_merge(_BidirectionalIter _first, _BidirectionalIter _middle,
	_BidirectionalIter _last)
{
	if (_first == _middle || _middle == _last)
		return;

	_inplace_merge_aux(_first, _middle, _last, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first));
}

template<typename _BidirectionalIter, typename _Tp, typename _Distance, typename _Compare>
void inplace_merge(_BidirectionalIter _first, _BidirectionalIter _middle,
	_BidirectionalIter _last, _Compare _comp)
{
	if (_first == _middle || _middle == _last)
		return;

	_inplace_merge_aux(_first, _middle, _last, _VALUE_TYPE(_first), _DISTANCE_TYPE(_first), _comp);
}

template <typename _InputIter1, typename _InputIter2>
bool includes(_InputIter1 _first1, _InputIter1 _last1,
			  _InputIter2 _first2, _InputIter2 _last2)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (*_first2 < *_first1)
			return false;
		else if (*_first1 < *_first2)
			++_first1;
		else
			++_first1, ++_first2;
	}

	return _first2 == _last2;
}

template <typename _InputIter1, typename _InputIter2, typename _Compare>
bool includes(_InputIter1 _first1, _InputIter1 _last1,
			  _InputIter2 _first2, _InputIter2 _last2, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp(*_first2, *_first1))
			return false;
		else if (_comp(*_first1, *_first2))
			++_first1;
		else
			++_first1, ++_first2;
	}

	return _first2 == _last2;
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter>
_OutputIter set_union(_InputIter1 _first1, _InputIter1 _last1,
					  _InputIter2 _first2, _InputIter2 _last2,
					  _OutputIter _result)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (*_first1 < *_first2)
		{
			*_result = *_first1;
			++_first1;
		}
		else if (*_first2 < *_first1) 
		{
			*_result = *_first2;
			++_first2;
		}
		else 
		{
			*_result = *_first1;
			++_first1;
			++_first2;
		}
		++_result;
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}

template <typename _InputIter1, typename _InputIter2, 
		  typename _OutputIter, typename _Compare>
_OutputIter set_union(_InputIter1 _first1, _InputIter1 _last1,
					_InputIter2 _first2, _InputIter2 _last2,
					_OutputIter _result, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp(*_first1, *_first2))
		{
			*_result = *_first1;
			++_first1;
		}
		else if (_comp(*_first2, *_first1))
		{
			*_result = *_first2;
			++_first2;
		}
		else
		{
			*_result = *_first1;
			++_first1;
			++_first2;
		}
		++_result;
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter>
_OutputIter set_intersection(_InputIter1 _first1, _InputIter1 _last1,
							_InputIter2 _first2, _InputIter2 _last2,
							_OutputIter _result)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (*_first1 < *_first2)
			++_first1;
		else if (*_first2 < *_first1)
			++_first2;
		else 
		{
			*_result = *_first1;
			++_first1;
			++_first2;
			++_result;
		}
	}
	return _result;
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter, typename _Compare>
_OutputIter set_intersection(_InputIter1 _first1, _InputIter1 _last1,
						_InputIter2 _first2, _InputIter2 _last2,
						_OutputIter _result, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp(*_first1, *_first2))
			++_first1;
		else if (_comp(*_first2, *_first1))
			++_first2;
		else
		{
			*_result = *_first1;
			++_first1;
			++_first2;
			++_result;
		}
	}
	return _result;
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter>
_OutputIter set_difference(_InputIter1 _first1, _InputIter1 _last1,
						_InputIter2 _first2, _InputIter2 _last2,
						_OutputIter _result)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (*_first1 < *_first2)
		{
			*_result = *_first1;
			++_first1;
			++_result;
		}
		else if (*_first2 < *_first1)
			++_first2;
		else
		{
			++_first1;
			++_first2;
		}
	}
	return copy(_first1, _last1, _result);
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter, typename _Compare>
_OutputIter set_difference(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _InputIter2 _last2,
	_OutputIter _result, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp(*_first1, *_first2))
		{
			*_result = *_first1;
			++_first1;
			++_result;
		}
		else if (_comp(*_first2, *_first1))
			++_first2;
		else
		{
			++_first1;
			++_first2;
		}
	}
	return copy(_first1, _last1, _result);
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter>
_OutputIter
set_symmetric_difference(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _InputIter2 _last2,
	_OutputIter _result)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (*_first1 < *_first2)
		{
			*_result = *_first1;
			++_first1;
			++_result;
		}
		else if (*_first2 < *_first1)
		{
			*_result = *_first2;
			++_first2;
			++_result;
		}
		else 
		{
			++_first1;
			++_first2;
		}
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}

template <typename _InputIter1, typename _InputIter2, typename _OutputIter, typename _Compare>
_OutputIter
set_symmetric_difference(_InputIter1 _first1, _InputIter1 _last1,
	_InputIter2 _first2, _InputIter2 _last2,
	_OutputIter _result, _Compare _comp)
{
	while (_first1 != _last1 && _first2 != _last2)
	{
		if (_comp*_first1, *_first2)
		{
			*_result = *_first1;
			++_first1;
			++_result;
		}
		else if (_comp(*_first2, *_first1))
		{
			*_result = *_first2;
			++_first2;
			++_result;
		}
		else
		{
			++_first1;
			++_first2;
		}
	}
	return copy(_first2, _last2, copy(_first1, _last1, _result));
}


template <typename _ForwardIter>
_ForwardIter max_element(_ForwardIter _first, _ForwardIter _last)
{
	if (_first == _last) 
		return _first;

	_ForwardIter _result = _first;
	while (++_first != _last)
	{
		if (*_result < *_first)
			_result = _first;
	}
	return _result;
}

template <typename _ForwardIter, typename _Compare>
_ForwardIter max_element(_ForwardIter _first, _ForwardIter _last, _Compare _comp)
{
	if (_first == _last)
		return _first;

	_ForwardIter _result = _first;
	while (++_first != _last)
	{
		if (_comp(*_result, *_first))
			_result = _first;
	}
	return _result;
}

template <typename _ForwardIter>
_ForwardIter min_element(_ForwardIter _first, _ForwardIter _last)
{
	if (_first == _last)
		return _first;

	_ForwardIter _result = _first;
	while (++_first != _last)
	{
		if (*_first < *_result)
			_result = _first;
	}
	return _result;
}

template <typename _ForwardIter, typename _Compare>
_ForwardIter min_element(_ForwardIter _first, _ForwardIter _last, _Compare _comp)
{
	if (_first == _last)
		return _first;

	_ForwardIter _result = _first;
	while (++_first != _last)
	{
		if (_comp(*_first, *_result))
			_result = _first;
	}
	return _result;
}


template <typename _BidirectionalIter>
bool next_permutation(_BidirectionalIter _first, _BidirectionalIter _last)
{
	
	if (_first == _last)
		return false;

	_BidirectionalIter _it = _first;
	++_it;
	if (_it == _last)
		return false;

	_it = _last;
	--_it;

	for (;;) 
	{
		_BidirectionalIter _ii = _it;
		--_it;
		if (*_it < *_ii) 
		{
			_BidirectionalIter _j = _last;
			while (!(*_it < *--_j))
			{
			}
			iter_swap(_it, _j);
			reverse(_ii, _last);
			return true;
		}
		if (_it == _first)
		{
			reverse(_first, _last);
			return false;
		}
	}
}

template <typename _BidirectionalIter, typename _Compare>
bool next_permutation(_BidirectionalIter _first, _BidirectionalIter _last, _Compare _comp)
{
	if (_first == _last)
		return false;

	_BidirectionalIter _it = _first;
	++_it;
	if (_it == _last)
		return false;

	_it = _last;
	--_it;

	for (;;)
	{
		_BidirectionalIter _ii = _it;
		--_it;
		if (_comp(*_it, *_ii))
		{
			_BidirectionalIter _j = _last;
			while (!_comp(*_it, *--_j))
			{
			}
			iter_swap(_it, _j);
			reverse(_ii, _last);
			return true;
		}
		if (_it == _first)
		{
			reverse(_first, _last);
			return false;
		}
	}
}


template <typename _BidirectionalIter>
bool prev_permutation(_BidirectionalIter _first, _BidirectionalIter _last)
{	
	if (_first == _last)
		return false;

	_BidirectionalIter _i = _first;
	++_i;
	if (_i == _last)
		return false;

	_i = _last;
	--_i;

	for (;;) 
	{
		_BidirectionalIter _ii = _i;
		--_i;
		if (*_ii < *_i)
		{
			_BidirectionalIter _j = _last;
			while (!(*--_j < *_i))
			{
			}
			iter_swap(_i, _j);
			reverse(_ii, _last);
			return true;
		}
		if (_i == _first) 
		{
			reverse(_first, _last);
			return false;
		}
	}
}

template <typename _BidirectionalIter, typename _Compare>
bool prev_permutation(_BidirectionalIter _first, _BidirectionalIter _last, _Compare _comp)
{
	if (_first == _last)
		return false;

	_BidirectionalIter _i = _first;
	++_i;
	if (_i == _last)
		return false;

	_i = _last;
	--_i;

	for (;;)
	{
		_BidirectionalIter _ii = _i;
		--_i;
		if (_comp(*_ii, *_i))
		{
			_BidirectionalIter _j = _last;
			while (!_comp(*--_j, *_i))
			{
			}
			iter_swap(_i, _j);
			reverse(_ii, _last);
			return true;
		}
		if (_i == _first)
		{
			reverse(_first, _last);
			return false;
		}
	}
}

template <typename _InputIter, typename _ForwardIter>
_InputIter find_first_of(_InputIter _first1, _InputIter _last1,
	_ForwardIter _first2, _ForwardIter _last2)
{
	for (; _first1 != _last1; ++_first1)
	{
		for (_ForwardIter _it = _first2; _it != _last2; ++_it)
			if (*_first1 == *_it)
				return _first1;
	}
	return _last1;
}

template <typename _InputIter, typename _ForwardIter, typename _BinaryPred>
_InputIter find_first_of(_InputIter _first1, _InputIter _last1,
	_ForwardIter _first2, _ForwardIter _last2, _BinaryPred _pred)
{
	for (; _first1 != _last1; ++_first1)
	{
		for (_ForwardIter _it = _first2; _it != _last2; ++_it)
			if (_pred(*_first1, *_it))
				return _first1;
	}
	return _last1;
}

template <typename _ForwardIter1, typename _ForwardIter2>
_ForwardIter1 _find_end(_ForwardIter1 _first1, _ForwardIter1 _last1,
	_ForwardIter2 _first2, _ForwardIter2 _last2,
	forward_iterator_tag, forward_iterator_tag)
{
	if (_first2 == _last2)
		return _last1;
	else
	{
		_ForwardIter1 _result = _last1;
		while (true)
		{
			_ForwardIter1 _new_result = search(_first1, _last1, _first2, _last2);
			if (_new_result == _last1)
				return _result;
			else
			{
				_result = _new_result;
				_first1 = _new_result;
				++_first1;
			}
		}
	}
}

template <typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPred>
_ForwardIter1 _find_end(_ForwardIter1 _first1, _ForwardIter1 _last1,
	_ForwardIter2 _first2, _ForwardIter2 _last2,
	forward_iterator_tag, forward_iterator_tag, _BinaryPred _comp)
{
	if (_first2 == _last2)
		return _last1;
	else
	{
		_ForwardIter1 _result = _last1;
		while (true)
		{
			_ForwardIter1 _new_result = search(_first1, _last1, _first2, _last2, _comp);
			if (_new_result == _last1)
				return _result;
			else
			{
				_result = _new_result;
				_first1 = _new_result;
				++_first1;
			}
		}
	}
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2>
_BidirectionalIter1
_find_end(_BidirectionalIter1 _first1, _BidirectionalIter1 _last1,
	_BidirectionalIter2 _first2, _BidirectionalIter2 _last2,
	bidirectional_iterator_tag, bidirectional_iterator_tag)
{

	typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
	typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

	_RevIter1 _rlast1(_first1);
	_RevIter2 _rlast2(_first2);
	_RevIter1 _rresult = search(_RevIter1(_last1), _rlast1, _RevIter2(_last2), _rlast2);

	if (_rresult == _rlast1)
		return _last1;
	else 
	{
		_BidirectionalIter1 _result = _rresult.base();
		advance(_result, -distance(_first2, _last2));
		return _result;
	}
}

template <typename _BidirectionalIter1, typename _BidirectionalIter2, typename _BinaryPred>
_BidirectionalIter1
_find_end(_BidirectionalIter1 _first1, _BidirectionalIter1 _last1,
	_BidirectionalIter2 _first2, _BidirectionalIter2 _last2,
	bidirectional_iterator_tag, bidirectional_iterator_tag, _BinaryPred _comp)
{

	typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
	typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

	_RevIter1 _rlast1(_first1);
	_RevIter2 _rlast2(_first2);
	_RevIter1 _rresult = search(_RevIter1(_last1), _rlast1, _RevIter2(_last2), _rlast2, _comp);

	if (_rresult == _rlast1)
		return _last1;
	else
	{
		_BidirectionalIter1 _result = _rresult.base();
		advance(_result, -distance(_first2, _last2));
		return _result;
	}
}



template <typename _ForwardIter1, typename _ForwardIter2>
inline _ForwardIter1
find_end(_ForwardIter1 _first1, _ForwardIter1 _last1,
	_ForwardIter2 _first2, _ForwardIter2 _last2)
{
	return _find_end(_first1, _last1, _first2, _last2,
		_ITERATOR_CATEGORY(_first1), _ITERATOR_CATEGORY(_first2));
}

template <typename _ForwardIter1, typename _ForwardIter2, typename _BinaryPred>
inline _ForwardIter1
find_end(_ForwardIter1 _first1, _ForwardIter1 _last1,
	_ForwardIter2 _first2, _ForwardIter2 _last2, _BinaryPred _comp)
{
	return _find_end(_first1, _last1, _first2, _last2,
		_ITERATOR_CATEGORY(_first1), _ITERATOR_CATEGORY(_first2), _comp);
}

template <typename _RandomAccessIter, typename _Distance>
bool _is_heap(_RandomAccessIter _first, _Distance _n)
{
	_Distance _parent = 0;
	for (_Distance _child = 1; _child < _n; ++_child)
	{
		if (_first[_parent] < _first[_child])
			return false;

		if ((_child & 1) == 0)
			++_parent;
	}
	return true;
}

template <typename _RandomAccessIter, typename _Distance, typename _StrictWeakOrdering>
bool _is_heap(_RandomAccessIter _first, _StrictWeakOrdering _comp,
	_Distance _n)
{
	_Distance _parent = 0;
	for (_Distance _child = 1; _child < _n; ++_child)
	{
		if (_comp(_first[_parent], _first[_child]))
			return false;

		if ((_child & 1) == 0)
			++_parent;
	}
	return true;
}

template <typename _RandomAccessIter>
inline bool is_heap(_RandomAccessIter _first, _RandomAccessIter _last)
{
	return _is_heap(_first, _last - _first);
}


template <typename _RandomAccessIter, typename _StrictWeakOrdering>
inline bool is_heap(_RandomAccessIter _first, _RandomAccessIter _last,
	_StrictWeakOrdering _comp)
{
	return _is_heap(_first, _comp, _last - _first);
}

template <typename _ForwardIter>
bool is_sorted(_ForwardIter _first, _ForwardIter _last)
{
	if (_first == _last)
		return true;

	_ForwardIter _next = _first;
	for (++_next; _next != _last; _first = _next, ++_next)
	{
		if (*_next < *_first)
			return false;
	}

	return true;
}

template <typename _ForwardIter, typename _StrictWeakOrdering>
bool is_sorted(_ForwardIter _first, _ForwardIter _last,
	_StrictWeakOrdering _comp)
{
	if (_first == _last)
		return true;

	_ForwardIter _next = _first;
	for (++_next; _next != _last; _first = _next, ++_next)
	{
		if (_comp(*_next, *_first))
			return false;
	}

	return true;
}




NAMESPACE_END


#endif//!_ALGORITHM_H_