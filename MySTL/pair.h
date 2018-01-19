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


#ifndef _PAIR_H_
#define _PAIR_H_

#include "config.h"

NAMESPACE_BEGIN


template<typename _T1, typename _T2>
struct pair
{
	typedef _T1 first_type;
	typedef _T2 second_type;

	_T1 first;
	_T2 second;

	pair():first(_T1()), second(_T2()){}
	pair(const _T1& _a, const _T2 &_b):first(_a), second(_b){}

	// 提供转换构造, 比如讲U1转换成const U1
	template<typename U1, typename U2>
	pair(const pair<U1, U2> &_p) : first(_p.first), second(_p.second){}
};

template<typename _T1, typename _T2>
bool operator==(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _x.first == _y.first && _x.second == _y.second;
}

template<typename _T1, typename _T2>
bool operator<(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _x.first < _y.first || (!(_y.first < _x.first) && _x.second < _y.second);
}

template<typename _T1, typename _T2>
bool operator!=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_x == _y);
}

template<typename _T1, typename _T2>
bool operator>(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _y < _x;
}

template<typename _T1, typename _T2>
bool operator>=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_x < _y);
}

template<typename _T1, typename _T2>
bool operator<=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_y < _x);
}

template<typename _T1, typename _T2>
pair<_T1, _T2> make_pair(const _T1 &_x, const _T2 &_y)
{
	return pair<_T1, _T2>(_x, _y);
}


NAMESPACE_END

#endif//!_PAIR_H_