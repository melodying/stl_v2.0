
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

#ifndef _MULTISET_H_
#define _MULTISET_H_

#include "config.h"
#include "function.h"
#include "tree.h"

NAMESPACE_BEGIN


template<typename _Key, typename _Compare = less<_Key>, typename _Alloc = alloc>
class multiset;

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const multiset<_Key, _Compare, _Alloc>& _x,
					   const multiset<_Key, _Compare, _Alloc>& _y);

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const multiset<_Key, _Compare, _Alloc>& _x,
					  const multiset<_Key, _Compare, _Alloc>& _y);


template<typename _Key, typename _Compare, typename _Alloc>
class multiset
{
public:
	typedef _Key key_type;
	typedef _Key value_type;
	typedef _Compare key_compare;
	typedef _Compare value_compare;
private:
	typedef _Rb_tree<key_type, value_type, 
		_Identity<value_type>, key_compare, _Alloc> _Rep_type;

	_Rep_type _m_tree;
	typedef typename _Rep_type::iterator _Rep_iterator;
public:
	typedef typename _Rep_type::const_pointer pointer;
	typedef typename _Rep_type::const_pointer const_pointer;
	typedef typename _Rep_type::const_reference reference;
	typedef typename _Rep_type::const_reference const_reference;
	typedef typename _Rep_type::const_iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;
	typedef typename _Rep_type::const_reverse_iterator reverse_iterator;
	typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename _Rep_type::size_type size_type;
	typedef typename _Rep_type::difference_type difference_type;
	typedef typename _Rep_type::allocator_type allocator_type;

	allocator_type get_allocator()const
	{
		return _m_tree.get_allocator();
	}

	multiset() :_m_tree(_Compare(), allocator_type()) {}
	explicit multiset(const _Compare &_comp, const allocator_type &_a = allocator_type()) 
				: _m_tree(_comp, _a) {}

	multiset(const value_type *_first, const value_type *_last) :_m_tree(_Compare(), allocator_type())
	{
		_m_tree.insert_equal(_first, _last);
	}
	multiset(const value_type *_first, const value_type *_last, const _Compare &_comp,
		const allocator_type&_a = allocator_type()) :_m_tree(_comp, _a)
	{
		_m_tree.insert_equal(_first, _last);
	}
	multiset(const_iterator _first, const_iterator _last) :_m_tree(_Compare(), allocator_type())
	{
		_m_tree.insert_equal(_first, _last);
	}
	multiset(const_iterator _first, const_iterator _last, const _Compare &_comp,
		const allocator_type &_a = allocator_type()) :_m_tree(_comp, _a)
	{
		_m_tree.insert_equal(_first, _last);
	}
	multiset(const multiset<_Key, _Compare, _Alloc> &_x) :_m_tree(_x._m_tree) {}

	multiset<_Key, _Compare, _Alloc>& operator=(const multiset<_Key, _Compare, _Alloc> &_x)
	{
		_m_tree = _x._m_tree;
		return *this;
	}

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

	void swap(multiset<_Key, _Compare, _Alloc> &_x)
	{
		_m_tree.swap(_x._m_tree);
	}

	iterator insert(const value_type &_val)
	{
		return _m_tree.insert_equal(_val);
	}

	iterator insert(iterator _position, const value_type &_val)
	{
		return _m_tree.insert_equal((_Rep_iterator&)_position, _val);
	}

	void insert(const value_type *_first, const value_type *_last)
	{
		_m_tree.insert_equal(_first, _last);
	}

	void insert(const_iterator _first, const_iterator _last)
	{
		_m_tree.insert_equal(_first, _last);
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

	friend bool operator==(const multiset&, const multiset&);
	friend bool operator<(const multiset&, const multiset&);
};

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y)
{
	return _x._m_tree == _y._m_tree;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y)
{
	return _x._m_tree < _y._m_tree;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator!=(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y) {
	return !(_x == _y);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y) {
	return _y < _x;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<=(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y) {
	return !(_y < _x);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>=(const multiset<_Key, _Compare, _Alloc>& _x,
	const multiset<_Key, _Compare, _Alloc>& _y) {
	return !(_x < _y);
}




NAMESPACE_END

#endif//_MULTISET_H_