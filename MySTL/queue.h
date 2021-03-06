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


#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "config.h"
#include "deque.h"
#include "function.h"
#include "heap.h"

NAMESPACE_BEGIN

template<typename _Tp, typename _Sequence = deque<_Tp> >
class queue
{
public:
	typedef typename _Sequence::value_type      value_type;
	typedef typename _Sequence::size_type       size_type;
	typedef          _Sequence                  container_type;

	typedef typename _Sequence::reference       reference;
	typedef typename _Sequence::const_reference const_reference;
protected:
	_Sequence c;
public:
	queue() :c() {}
	queue(const _Sequence &_c) :c(_c) {}
	bool empty()const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference front()
	{
		return c.front();
	}

	const_reference front()const
	{
		return c.front();
	}

	reference back()
	{
		return c.back();
	}

	const_reference back() const
	{
		return c.back();
	}

	void push(const value_type& _val)
	{
		c.push_back(_val);
	}
	void pop()
	{
		return c.pop_front();
	}

private:
	template<typename _Tp1, typename _Seq1>
	friend bool operator==(const queue<_Tp1, _Seq1>&, const queue<_Tp1, _Seq1>&);

	template<typename _Tp1, typename _Seq1>
	friend bool operator<(const queue<_Tp1, _Seq1>&, const queue<_Tp1, _Seq1>&);
};

template<typename _Tp, typename _Sequence>
bool operator==(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _x.c == _y.c;
}

template<typename _Tp, typename _Sequence>
bool operator<(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _x.c < _y.c;
}

template <typename _Tp, typename _Sequence>
bool operator!=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_x == _y);
}

template <typename _Tp, typename _Sequence>
bool operator>(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _y < _x;
}

template <typename _Tp, typename _Sequence>
bool operator<=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_y < _x);
}

template <typename _Tp, typename _Sequence>
bool operator>=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_x < _y);
}



template<typename _Tp, typename _Sequence = vector<_Tp>, typename _Compare = less<_Tp> >
class priority_queue
{
public:
	typedef typename _Sequence::value_type      value_type;
	typedef typename _Sequence::size_type       size_type;
	typedef          _Sequence                  container_type;

	typedef typename _Sequence::reference       reference;
	typedef typename _Sequence::const_reference const_reference;

protected:
	_Sequence c;
	_Compare comp;
public:
	priority_queue() : c() {}
	explicit priority_queue(const _Compare& __x) : c(), comp(__x) {}

	priority_queue(const _Compare& _x, const _Sequence& _s) : c(_s), comp(_x)
	{
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(const value_type* _first, const value_type* _last) : c(_first, _last) 
	{
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(const value_type* _first, const value_type* _last,
		const _Compare& _x) : c(_first, _last), comp(_x)
	{
		make_heap(c.begin(), c.end(), comp);
	}

	priority_queue(const value_type* _first, const value_type* _last,
		const _Compare& _x, const _Sequence& _c) : c(_c), comp(_x)
	{
		c.insert(c.end(), _first, _last);
		make_heap(c.begin(), c.end(), comp);
	}

	bool empty() const
	{
		return c.empty();
	}
	size_type size() const
	{
		return c.size();
	}
	const_reference top() const
	{
		return c.front();
	}
	void push(const value_type& _x)
	{
		c.push_back(_x);
		push_heap(c.begin(), c.end(), comp);
	}
	void pop()
	{
		pop_heap(c.begin(), c.end(), comp);
		c.pop_back();	
	}
};





NAMESPACE_END

#endif//!_QUEUE_H_


