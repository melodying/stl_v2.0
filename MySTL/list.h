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



#ifndef _LIST_H_
#define _LIST_H_

#include "config.h"
#include "alloc.h"
#include "construct.h"
#include "iterator.h"

NAMESPACE_BEGIN

struct _List_node_base
{
	_List_node_base *_m_next;
	_List_node_base *_m_prev;
};

template<typename _Tp>
struct _List_node : public _List_node_base
{
	_Tp _m_data;
};

struct _List_iterator_base
{
	typedef size_t						size_type;
	typedef ptrdiff_t					difference_type;
	typedef bidirectional_iterator_tag  iterator_category;

	_List_node_base* _m_node;

	_List_iterator_base(_List_node_base *_x) :_m_node(_x){}
	_List_iterator_base():_m_node(nullptr){}

	void _M_incr()
	{
		_m_node = _m_node->_m_next;
	}

	void _M_decr()
	{
		_m_node = _m_node->_m_prev;
	}

	bool operator==(const _List_iterator_base &_x)const
	{
		return _m_node == _x._m_node;
	}

	bool operator!=(const _List_iterator_base &_x)const
	{
		return _m_node != _x._m_node;
	}
};


template<typename _Tp, typename _Ref, typename _Ptr>
struct _List_iterator : public _List_iterator_base
{
public:
	typedef _List_iterator<_Tp, _Tp&, _Tp*>					iterator;
	typedef _List_iterator<_Tp, const _Tp&, const _Tp*>		const_iterator;
	typedef _List_iterator<_Tp, _Ref, _Ptr>					_Self;

	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
	typedef _List_node<_Tp> _Node;


	_List_iterator(_Node *_x): _List_iterator_base(_x){}
	_List_iterator(){}
	_List_iterator(const iterator& _x):_List_iterator_base(_x._m_node){}

	reference operator*()const 
	{
		return ((_Node*)_m_node)->_m_data;
	}

	pointer operator->()const
	{
		return &(operator*());
	}

	_Self& operator++()
	{
		this->_M_incr();
		return *this;
	}

	_Self operator++(int)
	{
		_Self _tmp = *this;
		this->_M_incr();
		return _tmp;
	}

	_Self &operator--()
	{
		this->_M_decr();
		return *this;
	}
	_Self operator--(int)
	{
		_Self _tmp = *this;
		this->_M_decr();
		return _tmp;
	}
};

template<typename _Tp, typename _Alloc>
class _List_base
{
public:
	typedef _Alloc allocator_type;
	allocator_type get_allocator()const
	{
		return allocator_type();
	}

	_List_base(const allocator_type&)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
	}

	~_List_base()
	{
		clear();
		_M_put_node(_m_head);
	}

	void clear();
protected:
	typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;

	_List_node<_Tp> * _M_get_node()
	{
		return _Alloc_type::allocate(1);
	}
	void _M_put_node(_List_node<_Tp> *_p)
	{
		_Alloc_type::deallocate(_p);
	}

	_List_node<_Tp>* _m_head;
};


template<typename _Tp, typename _Alloc = alloc >
class list : protected _List_base<_Tp, _Alloc>
{
	typedef _List_base<_Tp, _Alloc> _Base;
public:
	typedef _Tp						value_type;
	typedef _Tp*					pointer;
	typedef const _Tp*				const_point;
	typedef _Tp&					reference;
	typedef const _Tp&				const_reference;
	typedef  _List_node<_Tp>		_Node;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

	typedef typename _Base::allocator_type allocator_type;
	allocator_type get_allocator()const
	{
		return allocator_type();
	}
	

	typedef _List_iterator<_Tp, _Tp&, _Tp*>			iterator;
	typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;

	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator>       reverse_iterator;

protected:
	using _Base::_M_put_node;
	using _Base::_M_get_node;
	using _Base::_m_head;


	_Node *_M_create_node(const_reference _x)
	{
		_Node *_p = _M_get_node();
		construct(&_p->_m_data, _x);
		return _p;
	}

	_Node *_M_create_node()
	{
		_Node *_p = _M_get_node();
		construct(&_p->_m_data);
		return _p;
	}

	void _M_transfer(iterator _pos, iterator _first, iterator _last)
	{
		if (_pos != _last)
		{
			// 移除 [first, last)
			_last._m_node->_m_prev->_m_next = _pos._m_node;
			_first._m_node->_m_prev->_m_next = _last._m_node;
			_pos._m_node->_m_prev->_m_next = _first._m_node;

			// 将[first, last) 拼接到pos处
			_List_node_base *_tmp = _pos._m_node->_m_prev;
			_pos._m_node->_m_prev = _last._m_node->_m_prev;
			_last._m_node->_m_prev = _first._m_node->_m_prev;
			_first._m_node->_m_prev = _tmp;
		}
	}

public:
	list(const allocator_type &_a = allocator_type()) : _Base(_a){}

	list(size_type _n, const_reference _value, const allocator_type& _a = allocator_type()):_Base(_a)
	{
		insert(begin(), _n, _value);
	}

	list(size_type _n):_Base(allocator_type())
	{
		insert(begin(), _n, _Tp());
	}

	list(const_point _first, const_point _last, const allocator_type& _a = allocator_type()):_Base(_a)
	{
		insert(begin(), _first, _last);
	}

	list(const_iterator _first, const_iterator _last, const allocator_type& _a = allocator_type()):_Base(_a)
	{
		insert(begin(), _first, _last);
	}

	list(const list<_Tp, _Alloc> &_x):_Base(_x.get_allocator())
	{
		insert(begin(), _x.begin(), _x.end());
	}

	~list(){}

	iterator begin()
	{
		return (_Node*)_m_head->_m_next;
	}
	const_iterator begin()const
	{
		return (_Node*)_m_head->_m_next;
	}
	iterator end()
	{
		return (_Node*)_m_head;
	}
	const_iterator end()const
	{
		return (_Node*)_m_head;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	bool empty()const
	{
		return _m_head == _m_head->_m_next;
	}

	size_type size()
	{
		size_type _result = 0;
		distance(begin(), end(), _result);
		return _result;
	}
	size_type max_size()const
	{
		return (size_type)(-1);
	}

	reference front()
	{
		return *begin();
	}
	const_reference front()const
	{
		return *begin();
	}
	reference back()
	{
		return *(--end());
	}
	const_reference back()const
	{
		return *(--end());
	}

	iterator insert(iterator _pos, const_reference _x)
	{
		_Node *_tmp = _M_create_node(_x);
		_tmp->_m_next = _pos._m_node;
		_tmp->_m_prev = _pos._m_node->_m_prev;
		_pos._m_node->_m_prev->_m_next = _tmp;
		_pos._m_node->_m_prev = _tmp;
		return _tmp;
	}

	iterator insert(iterator _pos)
	{
		return insert(_pos, _Tp());
	}

	void insert(iterator _pos, const_point _first, const_point _last)
	{
		for (; _first != _last; ++_first)
			insert(_pos, *_first);
	}
	void insert(iterator _pos, const_iterator _first, const_iterator _last)
	{
		for (; _first != _last; ++_first)
			insert(_pos, *_first);
	}

	void insert(iterator _pos, size_type _n, const_reference _x)
	{
		for (; _n > 0; --_n)
			insert(_pos, _x);
	}

	void push_front(const_reference _x)
	{
		insert(begin(), _x);
	}
	void push_front()
	{
		insert(begin());
	}
	void push_back(const_reference _x)
	{
		insert(end(), _x);
	}
	void push_back()
	{
		insert(end());
	}

	iterator erase(iterator _pos)
	{
		_List_node_base *_next_node = _pos._m_node->_m_next;
		_List_node_base *_prev_node = _pos._m_node->_m_prev;
		_Node *_tmp = (_Node*)_pos._m_node;
		_prev_node->_m_next = _next_node;
		_next_node->_m_next = _prev_node;
		destroy(&_tmp->_m_data);
		_M_put_node(_tmp);
		return iterator((_Node*)_next_node);
	}
	iterator erase(iterator _first, iterator _last)
	{
		while (_first != _last)
			erase(_first++);
		return _last;
	}
	void clear()
	{
		_Base::clear();
	}
	void pop_front()
	{
		erase(begin());
	}
	void pop_back()
	{
		erase(--end());
	}
	void resize(size_type _new_size, const_reference _x)
	{
		iterator _i = begin();
		size_type _len = 0;
		for (; _i != end() && _len < _new_size; ++_i)
			++_len;

		if (_len == _new_size)// _i != end()
			erase(_i, end());
		else
			insert(_i, _new_size - _len, _x);
	}
	void resize(size_type _new_size)
	{
		resize(_new_size, _Tp());
	}

	list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& _x)
	{
		if(this != &_x)
		{
			iterator _first1 = begin();
			iterator _last1 = end();
			const_iterator _first2 = _x.begin();
			const_iterator _last2 = _x.end();
			while (_first1 != _last1 && _first2 != _last2)
				*_first1++ = *_first2++;

			if (_first2 == _last2)//表示first1中有剩余空间
				erase(_first1, _last1);
			else
				insert(_last1, _first2, _last2);
		}
		return *this;
	}

	void assign(size_type _n, const_reference _val);
	void swap(list<_Tp, _Alloc>& _x)
	{
		_Node *tmp = _m_head;
		 _m_head = _x._m_head;
		 _x._m_head = tmp;
	}

	void remove(const_reference _val)
	{
		iterator _first = begin();
		iterator _last = end();
		while (_first != _last)
		{
			iterator _next = _first;
			++_next;
			if (*_first == _val)
				erase(_first);

			_first = _next;
		}
	}

	void unique()
	{
		iterator _first = begin();
		iterator _last = end();
		if (_first == _last)
			return;

		iterator _next = _first;
		while (++_next != _last)
		{	
			if (*_next == *_first)
				erase(_next);
			else
			{
				_first = _next;
				_next = _first;
			}
		}
	}

	void merge(list<_Tp, _Alloc> &_x)
	{
		iterator _first1 = begin();
		iterator _last1 = end();
		iterator _first2 = _x.begin();
		iterator _last2 = _x.end();

		while (_first1 != _last1 && _first2 != _last2)
		{
			if (*_first2 < *_first1)
			{
				iterator _next = _first2;
				_M_transfer(_first1, _first2, ++_next);
				_first2 = _next;
			}
			else
				++_first1;
		}
		if (_first2 != _last2)
			_M_transfer(_last1, _first2, _last2);
	}

	void splice(iterator _pos, list<_Tp, _Alloc>& _x)
	{
		if (!_x.empty())
			_M_transfer(_pos, _x.begin(), _x.end());
	}

	void splice(iterator _pos, list<_Tp, _Alloc> &, iterator _i)
	{
		iterator _tmp = _i;
		++_tmp;
		if(_pos == _i || _pos == _tmp)
			return;

		_M_transfer(_pos, _i, _tmp);
	}

	void splice(iterator _pos, list&, iterator _first, iterator _last)
	{
		if (_first != _last)
			_M_transfer(_pos, _first, _last);
	}

	void sort()// 归并
	{
		if (!empty() && size() != 1)
		{
			list<_Tp, _Alloc> _carry;
			list<_Tp, _Alloc> _counter[64];
			int _fill = 0;
			while (!empty())
			{
				_carry.splice(_carry.begin(), *this, begin());
				int _i = 0;
				while (_i < _fill && !_counter[_i].empty())
				{
					_counter[_i].merge(_carry);
					_carry.swap(_counter[_i++]);
				}
				_carry.swap(_counter[_i]);
				if (_i == _fill)
					++_fill;
			}
			for (int _i = 1; _i < _fill; ++_i)
				_counter[_i].merge(_counter[_i - 1]);

			swap(_counter[_fill - 1]);
		}
	}

};

template <typename _Tp, typename _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
	_List_node<_Tp> *_cur = (_List_node<_Tp> *)_m_head->_m_next;
	while (_cur != _m_head)
	{
		_List_node<_Tp> *_tmp = _cur;
		_cur = (_List_node<_Tp> *)_cur->_m_next;
		destroy(&_tmp->_m_data);
		_M_put_node(_tmp);
	}
	_m_head->_m_next = _m_head;
	_m_head->_m_prev = _m_head;
}

template <typename _Tp, typename _Alloc>
void list<_Tp, _Alloc>::assign(size_type _n, const_reference _val)
{
	iterator _it = begin();
	for (; _it != end() && _n > 0; ++_it, --_n)
		*_it = _val;

	if (_n > 0)
		insert(end(), _n, _val);
	else
		erase(_it, end());
}


	NAMESPACE_END
#endif//!_LIST_H_