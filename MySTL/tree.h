/*
*
* Copyright (c) 1996,1997
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
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
*/


#ifndef _TREE_H_
#define _TREE_H_

#include "config.h"
#include "alloc.h"
#include "construct.h"
#include "algobase.h"
#include "pair.h"
#include "iterator_base.h"


NAMESPACE_BEGIN

typedef bool _Rb_tree_color_type;
const _Rb_tree_color_type _S_rb_tree_red = false;
const _Rb_tree_color_type _S_rb_tree_black = true;

struct _Rb_tree_node_base
{
	typedef _Rb_tree_color_type		_Color_type;
	typedef _Rb_tree_node_base*		_Base_ptr;

	_Color_type _m_color;
	_Base_ptr _m_parent;
	_Base_ptr _m_left;
	_Base_ptr _m_right;

	// 最小值为根节点往左子树走到底
	static _Base_ptr _S_minimum(_Base_ptr _x)
	{
		while (_x->_m_left != nullptr)
			_x = _x->_m_left;

		return _x;
	}

	// 最大值为根节点往右子数走到底
	static _Base_ptr _S_maximum(_Base_ptr _x)
	{
		while (_x->_m_right)
			_x = _x->_m_right;

		return _x;
	}
};

template<typename _Value>
struct _Rb_tree_node : public _Rb_tree_node_base
{
	typedef _Rb_tree_node<_Value>* _Link_type;
	_Value _m_value_field;				// 节点的值
};

struct _Rb_tree_base_iterator
{
	typedef _Rb_tree_node_base::_Base_ptr   _Base_ptr;
	typedef bidirectional_iterator_tag      iterator_category;
	typedef ptrdiff_t difference_type;

	_Base_ptr _m_node;

	// 只被迭代器的op++所调用, 其作用主要用来遍历元素. 
	// 对于相同的元素根据传入tree中的比较方式不同, 得到的结果也不相同
	// 如传递less方法. op++相当于从小到大的访问节点
	// 同理, 传递greater方法, 则是从大到小的访问节点
	// 因为不同的比较方式, 生成的树形是不一样的
	void _M_increment()
	{
		if(_m_node->_m_right)
		{
			_m_node = _m_node->_m_right;
			while (_m_node->_m_left)
			{
				_m_node = _m_node->_m_left;
			}
		}
		else
		{
			_Base_ptr _tmp = _m_node->_m_parent;
			while (_m_node == _tmp->_m_right)
			{
				_m_node = _tmp;
				_tmp = _tmp->_m_parent;
			}
			if(_m_node->_m_right != _tmp)
			{
				_m_node = _tmp;
			}
		}
	}

	void _M_decrement()
	{
		if (_m_node->_m_color == _S_rb_tree_red &&
			_m_node->_m_parent->_m_parent == _m_node)
		{
			_m_node = _m_node->_m_right;
		}
		else if(_m_node->_m_left)
		{
			_Base_ptr _tmp = _m_node->_m_left;
			while (_tmp->_m_right)
			{
				_tmp = _tmp->_m_right;
			}
			_m_node = _tmp;
		}
		else
		{
			_Base_ptr _tmp = _m_node->_m_parent;
			while (_m_node == _tmp->_m_left)
			{
				_m_node = _tmp;
				_tmp = _tmp->_m_parent;
			}
			_m_node = _tmp;
		}
	}
};

template<typename _Value, typename _Ref, typename _Ptr>
struct _Rb_tree_iterator:public _Rb_tree_base_iterator
{
	typedef _Value value_type;
	typedef _Ref reference;
	typedef _Ptr pointer;

	typedef _Rb_tree_iterator<_Value, _Value&, _Value*> iterator;
	typedef _Rb_tree_iterator<_Value, const _Value&, const _Value*> const_iterator;
	typedef _Rb_tree_iterator<_Value, _Ref, _Ptr> _Self;
	typedef _Rb_tree_node<_Value>* _Link_type;

	_Rb_tree_iterator(){}
	_Rb_tree_iterator(_Link_type _x)
	{
		_m_node = _x;
	}
	_Rb_tree_iterator(const iterator& _x)
	{
		_m_node = _x._m_node;
	}

	reference operator*()const
	{
		return _Link_type(_m_node)->_m_value_field;
	}

	pointer operator->()const
	{
		return &(operator*());
	}

	_Self &operator++()
	{
		_M_increment();
		return *this;
	}

	_Self operator++(int)
	{
		_Self _tmp = *this;
		_M_increment();
		return _tmp;
	}

	_Self &operator--()
	{
		_M_decrement();
		return *this;
	}

	_Self operator--(int)
	{
		_Self _tmp = *this;
		_M_decrement();
		return _tmp;
	}

};

inline bool operator==(const _Rb_tree_base_iterator& _x,
					   const _Rb_tree_base_iterator& _y)
{
	return _x._m_node == _y._m_node;
}

inline bool operator!=(const _Rb_tree_base_iterator& _x,
					   const _Rb_tree_base_iterator& _y)
{
	return _x._m_node != _y._m_node;
}

inline void _Rb_tree_rotate_left(_Rb_tree_node_base *_x, _Rb_tree_node_base *&_root)
{
	_Rb_tree_node_base *_y = _x->_m_right;
	_x->_m_right = _y->_m_left;
	if(_y->_m_left != nullptr)
	{
		_y->_m_left->_m_parent = _x;
	}
	_y->_m_parent = _x->_m_parent;

	if (_x == _root)
		_root = _y;
	else if (_x == _x->_m_parent->_m_left)
		_x->_m_parent->_m_left = _y;
	else
		_x->_m_parent->_m_right = _y;


	_y->_m_left = _x;
	_x->_m_parent = _y;
}

inline void _Rb_tree_rotate_right(_Rb_tree_node_base *_x, _Rb_tree_node_base *&_root)
{
	_Rb_tree_node_base *_y = _x->_m_left;
	_x->_m_left = _y->_m_right;
	if (_y->_m_right != nullptr)
	{
		_y->_m_right->_m_parent = _x;
	}
	_y->_m_parent = _x->_m_parent;

	if (_x == _root)
		_root = _y;
	else if (_x == _x->_m_parent->_m_right)
		_x->_m_parent->_m_right = _y;
	else
		_x->_m_parent->_m_left = _y;


	_y->_m_right = _x;
	_x->_m_parent = _y;
}

inline void _Rb_tree_rebalance(_Rb_tree_node_base *_x, _Rb_tree_node_base *&_root)
{
	_x->_m_color = _S_rb_tree_red;
	while (_x != _root && _x->_m_parent->_m_color == _S_rb_tree_red)
	{
		if(_x->_m_parent == _x->_m_parent->_m_parent->_m_left)
		{
			_Rb_tree_node_base *_y = _x->_m_parent->_m_parent->_m_right;
			if(_y && _y->_m_color == _S_rb_tree_red)
			{
				_x->_m_parent->_m_color = _S_rb_tree_black;
				_y->_m_color = _S_rb_tree_black;
				_x->_m_parent->_m_parent->_m_color = _S_rb_tree_red;
				_x = _x->_m_parent->_m_parent;
			}
			else
			{
				if(_x == _x->_m_parent->_m_right)
				{
					_x = _x->_m_parent;
					_Rb_tree_rotate_left(_x, _root);
				}

				_x->_m_parent->_m_color = _S_rb_tree_black;
				_x->_m_parent->_m_parent->_m_color = _S_rb_tree_red;
				_Rb_tree_rotate_right(_x->_m_parent->_m_parent, _root);
			}
		}
		else
		{
			_Rb_tree_node_base *_y = _x->_m_parent->_m_parent->_m_left;
			if(_y && _y->_m_color == _S_rb_tree_red)
			{
				_x->_m_parent->_m_color = _S_rb_tree_black;
				_y->_m_color = _S_rb_tree_black;
				_x->_m_parent->_m_parent->_m_color = _S_rb_tree_red;
				_x = _x->_m_parent->_m_parent;
			}
			else
			{
				if(_x == _x->_m_parent->_m_left)
				{
					_x = _x->_m_parent;
					_Rb_tree_rotate_right(_x, _root);
				}

				_x->_m_parent->_m_color = _S_rb_tree_black;
				_x->_m_parent->_m_parent->_m_color = _S_rb_tree_red;
				_Rb_tree_rotate_left(_x->_m_parent->_m_parent, _root);
			}
		}
	}
	_root->_m_color = _S_rb_tree_black;
}


inline _Rb_tree_node_base* 
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base * _z, 
							 _Rb_tree_node_base *&_root,
							 _Rb_tree_node_base *_leftmost, 
							 _Rb_tree_node_base *_rightmost)
{
	_Rb_tree_node_base *_y = _z;
	_Rb_tree_node_base *_x = nullptr;
	_Rb_tree_node_base *_x_parent = nullptr;
	if (_y->_m_left == nullptr)
		_x = _y->_m_right;	
	else
	{
		if (_y->_m_right == nullptr)
			_x = _y->_m_right;
		else
		{
			_y = _y->_m_right;
			while (_y->_m_left != nullptr)
			{
				_y = _y->_m_left;
			}
			_x = _y->_m_right;
		}
	}

	if (_y != _z)
	{
		_z->_m_left->_m_parent = _y;
		_y->_m_left = _z->_m_left;
		if(_y != _z->_m_right)
		{
			_x_parent = _y->_m_parent;
			if (_x)
				_x->_m_parent = _y->_m_parent;

			_y->_m_parent->_m_left = _x;
			_y->_m_right = _z->_m_right;
			_z->_m_right->_m_parent = _y;
		}
		else
		{
			_x_parent = _y; 
		}
		if (_root == _z)
			_root = _y;
		else if (_z->_m_parent->_m_left == _z)
			_z->_m_parent->_m_left = _y;
		else
			_z->_m_parent->_m_right = _y;

		_y->_m_parent = _z->_m_parent;
		swap(_y->_m_color, _z->_m_color);
		_y = _z;
	}
	else
	{
		_x_parent = _y->_m_parent;
		if (_x)
			_x->_m_parent = _y->_m_parent;

		if (_root == _z)
			_root = _x;
		else
		{
			if (_z->_m_parent->_m_left == _z)
				_z->_m_parent->_m_left = _x;
			else
				_z->_m_parent->_m_right = _x;
		}

		if (_leftmost == _z)
		{
			if (_z->_m_right == nullptr)
				_leftmost = _z->_m_parent;
			else
				_leftmost = _Rb_tree_node_base::_S_minimum(_x);
		}

		if (_rightmost == _z)
		{
			if (_z->_m_left == nullptr)
				_rightmost = _z->_m_parent;
			else
				_rightmost = _Rb_tree_node_base::_S_maximum(_x);
		}
	}

	if(_y->_m_color != _S_rb_tree_red)
	{
		while (_x != _root && (_x == nullptr || _x->_m_color == _S_rb_tree_black))
		{
			if(_x == _x_parent->_m_left)
			{
				_Rb_tree_node_base* _w = _x_parent->_m_right;	
				if (_w->_m_color == _S_rb_tree_red)
				{
					_w->_m_color = _S_rb_tree_black;
					_x_parent->_m_color = _S_rb_tree_red;
					_Rb_tree_rotate_left(_x_parent, _root);
					_w = _x_parent->_m_right;
				}

				if ((_w->_m_left == nullptr || _w->_m_left->_m_color == _S_rb_tree_black) &&
					(_w->_m_right == nullptr || _w->_m_right->_m_color == _S_rb_tree_black))
				{
					_w->_m_color = _S_rb_tree_red;
					_x = _x_parent;
					_x_parent = _x_parent->_m_parent;
				}
				else
				{
					if(_w->_m_right == nullptr || _w->_m_right->_m_color == _S_rb_tree_black)
					{
						if (_w->_m_left)
							_w->_m_left->_m_color = _S_rb_tree_black;

						_w->_m_color = _S_rb_tree_red;
						_Rb_tree_rotate_right(_w, _root);
						_w = _x_parent->_m_right;
					}

					_w->_m_color = _x_parent->_m_color;
					_x_parent->_m_color = _S_rb_tree_black;
					if (_w->_m_right)
						_w->_m_right->_m_color = _S_rb_tree_black;

					_Rb_tree_rotate_left(_x_parent, _root);
					break;
				}
			}
			else
			{
				_Rb_tree_node_base* _w = _x_parent->_m_left;
				if (_w->_m_color == _S_rb_tree_red) 
				{
					_w->_m_color = _S_rb_tree_black;
					_x_parent->_m_color = _S_rb_tree_red;
					_Rb_tree_rotate_right(_x_parent, _root);
					_w = _x_parent->_m_left;
				}
				if ((_w->_m_right == nullptr || _w->_m_right->_m_color == _S_rb_tree_black) &&
					(_w->_m_left == nullptr || _w->_m_left->_m_color == _S_rb_tree_black))
				{
					_w->_m_color = _S_rb_tree_red;
					_x = _x_parent;
					_x_parent = _x_parent->_m_parent;
				}
				else
				{
					if (_w->_m_left == nullptr || _w->_m_left->_m_color == _S_rb_tree_black)
					{
						if (_w->_m_right) 
							_w->_m_right->_m_color = _S_rb_tree_black;

						_w->_m_color = _S_rb_tree_red;
						_Rb_tree_rotate_left(_w, _root);
						_w = _x_parent->_m_left;
					}

					_w->_m_color = _x_parent->_m_color;
					_x_parent->_m_color = _S_rb_tree_black;

					if (_w->_m_left)
						_w->_m_left->_m_color = _S_rb_tree_black;

					_Rb_tree_rotate_right(_x_parent, _root);
					break;
				}
			}
		}
		if (_x)
			_x->_m_color = _S_rb_tree_black;
	}
	return _y;
}

template <typename _Tp, typename _Alloc>
struct _Rb_tree_base
{
	typedef _Alloc allocator_type;
	allocator_type get_allocator()const
	{
		return allocator_type();
	}

	_Rb_tree_base(const allocator_type&):_m_header(nullptr)
	{
		_m_header = _M_get_node();
	}
	~_Rb_tree_base()
	{
		_M_put_node(_m_header);
	}

protected:
	_Rb_tree_node<_Tp> *_m_header;

	typedef simple_alloc<_Rb_tree_node<_Tp>, _Alloc> _Alloc_type;

	_Rb_tree_node<_Tp>* _M_get_node()
	{
		return _Alloc_type::allocate(1);
	}

	void _M_put_node(_Rb_tree_node<_Tp> *_p)
	{
		_Alloc_type::deallocate(_p);
	}
};

template<typename _Key, typename _Value, typename _KeyOfValue,
		 typename _Compare, typename _Alloc = alloc>
class _Rb_tree : protected _Rb_tree_base<_Value, _Alloc>
{
	typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
	typedef _Rb_tree_node_base* _Base_ptr;
	typedef _Rb_tree_node<_Value> _Rb_tree_node;
	typedef _Rb_tree_color_type  _Color_type;
public:
	typedef _Key					key_type;
	typedef _Value					value_type;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef _Rb_tree_node*			_Link_type;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;
	
	typedef typename _Base::allocator_type allocator_type;
	allocator_type get_allocator()const
	{
		return allocator_type();
	}

protected:
	_Link_type _M_create_node(const value_type &_val)
	{
		_Link_type _tmp = _M_get_node();
		construct(&_tmp->_m_value_field, _val);
		return _tmp;
	}

	// 拷贝节点的值和颜色
	_Link_type _M_clone_node(_Link_type _x)
	{
		_Link_type _tmp = _M_create_node(_x->_m_value_field);
		_tmp->_m_color = _x->_m_color;
		_tmp->_m_left = nullptr;
		_tmp->_m_right = nullptr;
		return _tmp;
	}

	void destroy_node(_Link_type _p)
	{
		destroy(&_p->_m_value_field);
		_M_put_node(_p);
	}

protected:
	using _Base::_M_put_node;
	using _Base::_M_get_node;

	// 红黑树的header节点不等同于它的root节点, 设计该节点主要是为了处理边界情况
	// 该节点的父节点会指向root节点, 左子树节点指向leftmost(不一定是最小值所在的位置, 视传入的比较方法决定)
	// 右子树节点指向rightmost
	// [begin, end) --> [leftmost, header)
	using _Base::_m_header;

	size_type _m_node_count;
	_Compare _m_key_compare;

protected:
	_Link_type& _M_root()const
	{
		return (_Link_type&)_m_header->_m_parent;
	}
	_Link_type& _M_leftmost()const
	{
		return (_Link_type&)_m_header->_m_left;
	}
	_Link_type& _M_rightmost()const
	{
		return (_Link_type&)_m_header->_m_right;
	}

	static _Link_type& _S_left(_Link_type _x)
	{
		return (_Link_type&)(_x->_m_left);
	}
	static _Link_type& _S_right(_Link_type _x)
	{
		return (_Link_type&)(_x->_m_right);
	}
	static _Link_type& _S_parent(_Link_type _x)
	{
		return (_Link_type&)(_x->_m_parent);
	}
	static reference _S_value(_Link_type _x)
	{
		return _x->_m_value_field;
	}
	static const _Key& _S_key(_Link_type _x)
	{
		return _KeyOfValue()(_S_value(_x));
	}
	static _Color_type& _S_color(_Link_type _x)
	{
		return (_Color_type&)(_x->_m_color);
	}

	static _Link_type& _S_left(_Base_ptr _x)
	{
		return (_Link_type&)(_x->_m_left);
	}
	static _Link_type& _S_right(_Base_ptr _x)
	{
		return (_Link_type&)(_x->_m_right);
	}
	static _Link_type& _S_parent(_Base_ptr _x)
	{
		return (_Link_type&)(_x->_m_parent);
	}
	static reference _S_value(_Base_ptr _x)
	{
		return ((_Link_type)_x)->_m_value_field;
	}
	static const _Key& _S_key(_Base_ptr _x)
	{
		return _KeyOfValue()(_S_value(_Link_type(_x)));
	}
	static _Color_type& _S_color(_Base_ptr _x)
	{
		return (_Color_type&)(_Link_type(_x)->_m_color);
	}

	static _Link_type _S_minimum(_Link_type _x)
	{
		return (_Link_type)_Rb_tree_node_base::_S_minimum(_x);
	}

	static _Link_type _S_maximum(_Link_type _x)
	{
		return (_Link_type)_Rb_tree_node_base::_S_maximum(_x);
	}


public:
	typedef _Rb_tree_iterator<value_type, reference, pointer>	iterator;
	typedef _Rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

	_Rb_tree():_Base(allocator_type()), _m_node_count(0), _m_key_compare()
	{
		_M_empty_initialize();
	}
	_Rb_tree(const _Compare& _comp) :_Base(allocator_type()), _m_node_count(0), _m_key_compare(_comp)
	{	
		_M_empty_initialize();
	}

	_Rb_tree(const _Compare& _comp, allocator_type&_a) :_Base(_a), 
				_m_node_count(0), _m_key_compare(_comp)
	{
		_M_empty_initialize();
	}

	_Rb_tree(const _Rb_tree& _x) :_Base(_x.get_allocator()),
				_m_node_count(0), _m_key_compare(_x._m_key_compare)
	{
		if(_x._M_root() == nullptr)
		{
			_M_empty_initialize();
		}
		else
		{
			_S_color(_m_header) = _S_rb_tree_red;
			_M_root() = _M_copy(_x._M_root(), _m_header);
			_M_leftmost() = _S_minimum(_M_root());
			_M_rightmost() = _S_maximum(_M_root());
		}
		_m_node_count = _x._m_node_count;
	}
	~_Rb_tree()
	{
		clear();
	}

	_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> &
		operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> &_x);

private:
	iterator _M_insert(_Base_ptr _x, _Base_ptr _y, const value_type &_val);
	_Link_type _M_copy(_Link_type _x, _Link_type _p);
	void _M_erase(_Link_type _x);

	void _M_empty_initialize()
	{
		_S_color(_m_header) = _S_rb_tree_red;
		_M_root() = nullptr;
		_M_leftmost() = _m_header;
		_M_rightmost() = _m_header;
	}

public:
	_Compare key_comp()const
	{
		return _m_key_compare;
	}
	iterator begin()
	{
		return _M_leftmost();
	}
	const_iterator begin()const
	{
		return _M_leftmost();
	}
	iterator end()
	{
		return _m_header;
	}
	const_iterator end()const
	{
		return _m_header;
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
		return _m_node_count == 0;
	}
	size_type size()const
	{
		return _m_node_count;
	}
	size_type max_size()const
	{
		return size_type(-1);
	}

	void swap(_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc> &_x)
	{
		swap(_m_header, _x.max_size());
		swap(_m_node_count, _x._m_node_count);
		swap(_m_key_compare, _x._m_key_compare);
	}

	void clear()
	{
		if(_m_node_count != 0)
		{
			_M_erase(_M_root());
			_M_leftmost() = _m_header;
			_M_root() = nullptr;
			_M_rightmost() = _m_header;
			_m_node_count = 0;
		}
	}
public:
	pair<iterator, bool> insert_unique(const value_type& _val);
	iterator insert_unique(iterator _position, const value_type& _val);
	void insert_unique(const value_type* _first, const value_type* _last);
	void insert_unique(const_iterator _first, const_iterator _last);

	iterator insert_equal(const value_type _x);
	iterator insert_equal(iterator _position, const value_type &_val);
	void insert_equal(const_iterator _first, const_iterator _last);
	void insert_equal(const value_type* _first, const value_type *_last);

	void erase(iterator _postion)
	{
		_Link_type _y = (_Link_type)_Rb_tree_rebalance_for_erase(_postion._m_node, _m_header->_m_parent, 
			_m_header->_m_left, _m_header->_m_right);

		destroy_node(_y);
		--_m_node_count;
	}

	size_type erase(const key_type& _key)
	{
		pair<iterator, iterator> _p = equal_range(_key);
		size_type _n = 0;
		distance(_p.first, _p.second, _n);
		erase(_p.first, _p.second);
		return _n;
	}

	void erase(iterator _first, iterator _last)
	{
		if (_first == begin() && _last == end())
			clear();
		else
		{
			while (_first != _last)
				erase(_first++);
		}
	}

public:
	iterator find(const key_type &_key);
	const_iterator find(const key_type &_key)const;
	size_type count(const key_type& _key) const;
	iterator lower_bound(const key_type &_key);
	const_iterator lower_bound(const key_type &_key)const;
	iterator upper_bound(const key_type &_key);
	const_iterator upper_bound(const key_type &_key)const;
	pair<iterator, iterator> equal_range(const key_type &_key);
	pair<const_iterator, const_iterator> equal_range(const key_type &_key)const;

};

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>&
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& _x)
{
	if(this != &_x)
	{
		clear();
		_m_node_count = 0;
		_m_key_compare = _x._m_key_compare;
		if(_x._M_root() == nullptr)
		{
			_M_root() = 0;
			_M_leftmost() = _m_header;
			_M_rightmost() = _m_header;
		}
		else
		{
			_M_root() = _M_copy(_x._M_root(), _m_header);
			_M_leftmost() = _S_minimum(_M_root());
			_M_rightmost() = _S_maximum(_M_root());
			_m_node_count = _x._m_node_count;
		}
	}
	return *this;
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
_M_insert(_Base_ptr _x, _Base_ptr _y, const value_type& _val)
{
	_Link_type _x_tmp = (_Link_type)_x;
	_Link_type _y_tmp = (_Link_type)_y;
	_Link_type _z;

	if(_y_tmp == _m_header || _x_tmp != nullptr ||
		_m_key_compare(_KeyOfValue()(_val), _S_key(_y_tmp)))
	{
		_z = _M_create_node(_val);
		_S_left(_y_tmp) = _z;

		if (_y_tmp == _m_header)
		{
			_M_root() = _z;
			_M_rightmost() = _z;
		}
		else if (_y_tmp == _M_leftmost())
			_M_leftmost() = _z;
	}
	else
	{
		_z = _M_create_node(_val);
		_S_right(_y_tmp) = _z;
		if (_y_tmp == _M_rightmost())
			_M_rightmost() = _z;
	}

	_S_parent(_z) = _y_tmp;
	_S_left(_z) = 0;
	_S_right(_z) = 0;
	
	_Rb_tree_rebalance(_z, _m_header->_m_parent);
	++_m_node_count;
	return iterator(_z);
}

	template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Link_type _x, _Link_type _p)
{
	_Link_type _top = _M_clone_node(_x);
	_top->_m_parent = _p;

	if (_x->_m_right)
		_top->_m_right = _M_copy(_S_right(_x), _top);

	_p = _top;
	_x = _S_left(_x);

	while(_x)
	{
		_Link_type _y = _M_clone_node(_x);
		_p->_m_left = _y;
		_y->_m_parent = _p;
		if (_x->_m_right)
			_y->_m_right = _M_copy(_S_right(_x), _y);

		_p = _y;
		_x = _S_left(_x);
	}
	return _top;
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type _x)
{
	while (_x)
	{
		_M_erase(_S_right(_x));
		_Link_type _y = _S_left(_x);
		destroy_node(_x);
		_x = _y;
	}
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool> 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& _val)
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();
	bool _comp = true;
	while (_x)
	{
		_y = _x;
		_comp = _m_key_compare(_KeyOfValue()(_val), _S_key(_x));
		_x = _comp ? _S_left(_x) : _S_right(_x);
	}

	iterator _it = iterator(_y);
	if (_comp)
	{
		if (_it == begin())
			return pair<iterator, bool>(_M_insert(_x, _y, _val), true);
		else
			--_it;
	}

	if (_m_key_compare(_S_key(_it._m_node), _KeyOfValue()(_val)))
		return pair<iterator, bool>(_M_insert(_x, _y, _val), true);

	return pair<iterator, bool>(_it, false);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator _position, const value_type& _val)
{
	if (_position._m_node == _m_header->_m_left)
	{
		if (size() > 0 && _m_key_compare(_KeyOfValue()(_val), _S_key(_position._m_node)))
			return _M_insert(_position._m_node, _position._m_node, _val);
		else
			return insert_unique(_val).first;
	}
	else if (_position._m_node == _m_header)
	{
		if (_m_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(_val)))
			return _M_insert(0, _M_rightmost(), _val);
		else
			return insert_unique(_val).first;
	}
	else
	{
		iterator _before = _position;
		--_before;
		if (_m_key_compare(_S_key(_before._m_node), _KeyOfValue()(_val))
			&& _m_key_compare(_KeyOfValue()(_val), _S_key(_position._m_node)))
		{
			if (_S_right(_before._m_node) == nullptr)
				return _M_insert(nullptr, _before._m_node, _val);
			else
				return _M_insert(_position._m_node, _position._m_node, _val);
		}
		else
			return insert_unique(_val).first;
	}
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
insert_unique(const value_type* _first, const value_type* _last)
{
	for (; _first != _last; ++_first)
		insert_unique(*_first);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
insert_unique(const_iterator _first, const_iterator _last)
{
	for (; _first != _last; ++_first)
		insert_unique(*_first);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(const value_type _val)
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();
	if(_x)
	{
		_y = _x;
		_x = _m_key_compare(_KeyOfValue()(_val), _S_key(_x)) ? _S_left(_x) : _S_right(_x);
	}
	return _M_insert(_x, _y, _val);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
insert_equal(iterator _position, const value_type& _val)
{
	if (_position._m_node == _m_header->_m_left)
	{
		if (size() > 0 && !_m_key_compare(_S_key(_position._m_node), _KeyOfValue()(_val)))
			return _M_insert(_position._m_node, _position._m_node, _val);
		else
			return insert_equal(_val);
	}
	else if (_position._m_node == _m_header)
	{
		if (!_m_key_compare(_KeyOfValue()(_val), _S_key(_M_rightmost())))
			return _M_insert(0, _M_rightmost(), _val);
		else
			return insert_equal(_val);
	}
	else
	{
		iterator _before = _position;
		--_before;
		if (!_m_key_compare(_KeyOfValue()(_val), _S_key(_before._m_node))
			&& !_m_key_compare(_S_key(_position._m_node), _KeyOfValue()(_val)))
		{
			if (_S_right(_before._m_node) == nullptr)
				return _M_insert(nullptr, _before._m_node, _val);
			else
				return _M_insert(_position._m_node, _position._m_node, _val);
		}
		else
			return insert_equal(_val);
	}
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
insert_equal(const_iterator _first, const_iterator _last)
{
	for (; _first != _last; ++_first)
		insert_equal(*_first);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::
insert_equal(const value_type* _first, const value_type* _last)
{
	for (; _first != _last; ++_first)
		insert_equal(*_first);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::find(const key_type& _key)
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (!_m_key_compare(_S_key(_x), _key))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}

	iterator it = iterator(_y);
	return (it == end() || _m_key_compare(_key, _S_key(it._m_node))) ? end() : it;
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::find(const key_type& _key) const
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (!_m_key_compare(_S_key(_x), _key))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}

	iterator it = iterator(_y);
	return (it == end() || _m_key_compare(_key, _S_key(it._m_node))) ? end() : it;
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::size_type 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::count(const key_type& _key) const
{
	pair<const_iterator, const_iterator> _p = equal_range(_key);
	size_type _n = 0;
	distance(_p.first, _p.second, _n);
	return _n;
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::lower_bound(const key_type& _key)
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (!_m_key_compare(_S_key(_x), _key))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}
	return iterator(_y);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::lower_bound(const key_type& _key) const
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (!_m_key_compare(_S_key(_x), _key))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}
	return const_iterator(_y);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::upper_bound(const key_type& _key)
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (_m_key_compare(_key, _S_key(_x)))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}
	return iterator(_y);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::upper_bound(const key_type& _key) const
{
	_Link_type _y = _m_header;
	_Link_type _x = _M_root();

	while (_x)
	{
		if (_m_key_compare(_key, _S_key(_x)))
		{
			_y = _x;
			_x = _S_left(_x);
		}
		else
			_x = _S_right(_x);
	}
	return const_iterator(_y);
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, 
	typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator> 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::equal_range(const key_type& _key)
{
	return pair<iterator, iterator>(lower_bound(_key), upper_bound(_key));
}

template <typename _Key, typename _Value, typename _KeyOfValue, typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator, 
	typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::const_iterator> 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::equal_range(const key_type& _key) const
{
	return pair<const_iterator, const_iterator>(lower_bound(_key), upper_bound(_key));
}

NAMESPACE_END

#endif//!_TREE_H_