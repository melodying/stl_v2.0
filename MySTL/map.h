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


#ifndef _MAP_H_
#define _MAP_H_

#include "config.h"
#include "function.h"
#include "alloc.h"
#include "pair.h"
#include "tree.h"

NAMESPACE_BEGIN

template<typename _Key, typename _Tp, typename _Compare = less<_Key>, typename _Alloc = alloc>
class map;

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y);

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y);


template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
class map
{
public:
	typedef _Key key_type;
	typedef _Tp data_type;
	typedef _Tp mapped_type;
	typedef pair<const _Key, _Tp> value_type;
	typedef _Compare key_compare;

	class value_compare
	{
		friend class map<_Key, _Tp, _Compare, _Alloc>;
	protected:
		_Compare _comp;
		value_compare(_Compare _c) :_comp(_c){}
	public:
		bool operator()(const value_type &_x, const value_type &_y)const
		{
			return _comp(_x.first, _y.first);
		}
	};

private:
	typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
	_Rep_type _m_tree;

public:
	typedef typename _Rep_type::pointer pointer;
	typedef typename _Rep_type::const_pointer const_pointer;
	typedef typename _Rep_type::reference reference;
	typedef typename _Rep_type::const_reference const_reference;
	typedef typename _Rep_type::iterator iterator;
	typedef typename _Rep_type::const_iterator const_iterator;
	typedef typename _Rep_type::size_type size_type;
	typedef typename _Rep_type::difference_type difference_type;

	map():_m_tree(_Compare()){}
	explicit map(const _Compare &_comp):_m_tree(_comp){}
	map(const value_type *_first, const value_type *_last):_m_tree(_Compare())
	{
		_m_tree.insert_unique(_first, _last);
	}
	map(const value_type *_first, const value_type *_last, _Compare &_comp) :_m_tree(_comp)
	{
		_m_tree.insert_unique(_first, _last);
	}

	map(const_iterator _first, const_iterator _last) :_m_tree(_Compare())
	{
		_m_tree.insert_unique(_first, _last);
	}
	map(const_iterator _first, const_iterator _last, _Compare &_comp) :_m_tree(_comp)
	{
		_m_tree.insert_unique(_first, _last);
	}

	map(const map<_Key, _Tp, _Compare, _Alloc> &_x):_m_tree(_x._m_tree){}
	map<_Key, _Tp, _Compare, _Alloc> &operator=(const map<_Key, _Tp, _Compare, _Alloc> &_x)
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
		return value_compare(_m_tree.key_comp());
	}

	iterator begin()
	{
		return _m_tree.begin();
	}
	const_iterator begin()const
	{
		return _m_tree.begin();
	}

	iterator end()
	{
		return _m_tree.end();
	}
	const_iterator end()const
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

	_Tp& operator[](const key_type &_key)
	{
		iterator _it = lower_bound(_key);
		if (_it == end() || key_comp()(_key, (*_it).first))
		{
			_it = insert(_it, value_type(_key, _Tp()));
		}
		return (*_it).second;
	}

	pair<iterator, bool> insert(const value_type &_val)
	{
		return _m_tree.insert_unique(_val);
	}

	iterator insert(iterator _position, const value_type &_val)
	{
		return _m_tree.insert_unique(_position, _val);
	}

	void insert(const value_type *_first, const value_type *_last)
	{
		_m_tree.insert_unique(_first, _last);
	}

	void insert(const_iterator _first, const_iterator _last)
	{
		_m_tree.insert_unique(_first, _last);
	}

	void erase(iterator _first, iterator _last)
	{
		_m_tree.erase(_first, _last);
	}

	void clear()
	{
		_m_tree.clear();
	}

	iterator find(const key_type &_key)
	{
		return _m_tree.find(_key);
	}
	const_iterator find(const key_type &_key)const
	{
		return _m_tree.find(_key);
	}

	size_type count(const key_type &_key)
	{
		return _m_tree.find(_key) == _m_tree.end() ? 0 : 1;
	}

	iterator lower_bound(const key_type &_key)
	{
		return  _m_tree.lower_bound(_key);
	}
	const_iterator lower_bound(const key_type &_key)const
	{
		return _m_tree.lower_bound(_key);
	}

	iterator upper_bound(const key_type &_key)
	{
		return  _m_tree.upper_bound(_key);
	}
	const_iterator upper_bound(const key_type &_key)const
	{
		return _m_tree.upper_bound(_key);
	}

	pair<iterator, iterator> equal_range(const key_type &_key)
	{
		return _m_tree.equal_range(_key);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type &_key)const
	{
		return _m_tree.equal_range(_key);
	}

	friend bool operator==(const map&, const map&);
	friend bool operator<(const map&, const map&);
};


template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y)
{
	return _x._m_tree == _y._m_tree;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y)
{
	return _x._m_tree < _y._m_tree;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator!=(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y) 
{
	return !(_x == _y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y) 
{
	return _y < _x;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<=(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y) 
{
	return !(_y < _x);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>=(const map<_Key, _Tp, _Compare, _Alloc>& _x,
	const map<_Key, _Tp, _Compare, _Alloc>& _y) 
{
	return !(_x < _y);
}




NAMESPACE_END

#endif//!_MAP_H_