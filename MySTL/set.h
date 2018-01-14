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


#ifndef _SET_H_
#define _SET_H_

#include "config.h"
#include "function.h"
#include "alloc.h"
#include "tree.h"


NAMESPACE_BEGIN

template<typename _Key, typename _Compare = less<_Key>, typename _Alloc = alloc>
class set;

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y);

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y);

template<typename _Key, typename _Compare, typename _Alloc>
class set
{
public:
	typedef _Key key_type;
	typedef _Key value_type;
	typedef _Compare key_compare;
	typedef _Compare value_compare;

private:
	typedef _Rb_tree<key_type, value_type, _Identity<value_type>, key_compare, _Alloc> _Rep_type;
	_Rep_type _m_tree;

	typedef typename _Rep_type::iterator _Rep_iterator;
public:
	// 定义成const防止用户进行任意写入
	typedef typename _Rep_type::const_pointer pointer;
	typedef typename _Rep_type::const_pointer const_pointer;
	typedef typename _Rep_type::const_reference reference;
	typedef typename _Rep_type::const_reference const_reference;
	typedef typename _Rep_type::const_iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;
	typedef typename _Rep_type::size_type size_type;
	typedef typename _Rep_type::difference_type difference_type;


	set():_m_tree(_Compare()){}
	explicit set(const _Compare &_comp) : _m_tree(_comp){}
	set(const value_type *_first, const value_type *_last) :_m_tree(_Compare())
	{
		_m_tree.insert_unique(_first, _last);
	}
	set(const value_type *_first, const value_type *_last, const _Compare &_comp):_m_tree(_comp)
	{
		_m_tree.insert_unique(_first, _last);
	}
	set(const_iterator _first, const_iterator _last) :_m_tree(_Compare())
	{
		_m_tree.insert_unique(_first, _last);
	}
	set(const_iterator _first, const_iterator _last, const _Compare &_comp) :_m_tree(_comp)
	{
		_m_tree.insert_unique(_first, _last);
	}
	set(const set<_Key, _Compare, _Alloc> &_x):_m_tree(_x._m_tree){}
	set<_Key,_Compare,_Alloc>& operator=(const set<_Key,_Compare, _Alloc> &_x)
	{
		_m_tree = _x._m_tree;
		return *this;
	}
public:
	key_compare key_comp()const
	{
		return _m_tree.key_comp();
	}

	value_compare value_comp()const
	{
		return _m_tree.key_comp();
	}

	iterator begin()const
	{
		return _m_tree.begin();
	}

	iterator end()const
	{
		return _m_tree.end();
	}

	bool empty()const
	{
		return _m_tree.empty();
	}

	size_type size()const
	{
		return _m_tree.size();
	}

	size_type max_size()const
	{
		return _m_tree.max_size();
	}

	void swap(set<_Key, _Compare, _Alloc> &_x)
	{
		_m_tree.swap(_x._m_tree);
	}

	pair<iterator, bool> insert(const value_type &_val)
	{
		pair<typename _Rep_type::iterator, bool> _p = _m_tree.insert_unique(_val);
		return pair<iterator, bool>(_p.first, _p.second);
	}

	iterator insert(iterator _position, const value_type &_x)
	{
		return _m_tree.insert_unique((_Rep_iterator&)_position, _x);
	}

	void insert(const_iterator _first, const_iterator _last)
	{
		_m_tree.insert_unique(_first, _last);
	}
	void insert(const value_type *_first, const value_type *_last)
	{
		_m_tree.insert_unique(_first, _last);
	}

	void erase(iterator _position)
	{
		_m_tree.erase((_Rep_iterator&)_position);
	}
	size_type erase(const key_type& _x)
	{
		return _m_tree.erase(_x);
	}

	void erase(iterator _first, iterator _last)
	{
		_m_tree.erase((_Rep_iterator&)_first, (_Rep_iterator&)_last);
	}

	void clear()
	{
		_m_tree.clear();
	}

	iterator find(const key_type& _x)const
	{
		return _m_tree.find(_x);
	}

	size_type count(const key_type &_x)const
	{
		return _m_tree.find(_x) == _m_tree.end() ? 0 : 1;
	}

	iterator lower_bound(const key_type &_x)const
	{
		return _m_tree.lower_bound(_x);
	}

	iterator upper_bound(const key_type &_x)const
	{
		return _m_tree.upper_bound(_x);
	}

	pair<iterator, iterator> equal_range(const key_type & _x)const
	{
		return _m_tree.equal_range(_x);
	}

	friend bool operator==(const set&, const set&);
	friend bool operator<(const set&, const set&);
};

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y)
{
	return _x._m_tree == _y._m_tree;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y)
{
	return _x._m_tree < _y._m_tree;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator!=(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y) {
	return !(_x == _y);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y) {
	return _y < _x;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<=(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y) {
	return !(_y < _x);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>=(const set<_Key, _Compare, _Alloc>& _x,
	const set<_Key, _Compare, _Alloc>& _y) {
	return !(_x < _y);
}


NAMESPACE_END

#endif//!_SET_H_