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



#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdexcept>

#include "config.h"
#include "alloc.h"
#include "uninitialized.h"
#include "algobase.h"
#include "iterator.h"

NAMESPACE_BEGIN


template<typename _Tp, typename _Alloc>
class _Vector_base
{
public:
	typedef _Alloc allocator_type;
	allocator_type get_allocator()const
	{
		return allocator_type();
	}

	_Vector_base(const _Alloc&):_m_start(nullptr), _m_finish(nullptr), _m_end(nullptr){}
	_Vector_base(size_t _n, const _Alloc&) :_m_start(nullptr), _m_finish(nullptr), _m_end(nullptr)
	{
		_m_start = _M_allocate(_n);
		_m_finish = _m_start;
		_m_end = _m_start + _n;
	} 

	~_Vector_base()
	{
		_M_deallocate(_m_start);
	}
protected:
	_Tp *_m_start;
	_Tp *_m_finish;
	_Tp *_m_end;

	typedef simple_alloc<_Tp, _Alloc> _m_data_allocator;

	_Tp* _M_allocate(size_t _n)
	{
		return _m_data_allocator::allocate(_n);
	}

	void _M_deallocate(_Tp *_p)
	{
		_m_data_allocator::deallocate(_p);
	}
};



template <typename _Tp, typename _Alloc = alloc >
class vector : protected _Vector_base<_Tp, _Alloc>
{
	typedef _Vector_base<_Tp, _Alloc> _Base;
public:
	typedef size_t				size_type;
	typedef _Tp					value_type;
	typedef value_type*			iterator;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef const value_type*	const_iterator;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef int					difference_type;

	typedef typename _Base::allocator_type allocator_type;
	allocator_type get_allocator()const
	{
		return _Base::get_allocator();
	}

	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

protected:
	using _Base::_m_start;
	using _Base::_m_finish;
	using _Base::_m_end;
	using _Base::_M_allocate;
	using _Base::_M_deallocate;

public:

	// 使用者可以自定义自己的空间配置器
	explicit vector(const allocator_type& _a = allocator_type()):_Base(_a){}

	vector(size_type _n, const value_type &_val, 
		const allocator_type &_a = allocator_type()):_Base(_n, _a)
	{
		_m_finish = uninitialized_fill_n(_m_start, _n, _val);
	}

	vector(size_type _n):_Base(_n, allocator_type())
	{
		_m_finish = uninitialized_fill_n(_m_start, _n, _Tp());
	}

	vector(const vector<_Tp, _Alloc>& _other):_Base(_other.size(), _other.get_allocator())
	{
		_m_finish = uninitialized_copy(_other.begin(), _other.end(), _m_start);
	}

	vector(const _Tp* _first, const _Tp* _last, 
		const allocator_type &_a = allocator_type()) : _Base(_last - _first, _a)
	{
		_m_finish = uninitialized_copy(_first, _last, _m_start);
	}

	~vector()
	{
		destroy(_m_start, _m_finish);
	}

	iterator begin()
	{
		return _m_start;
	}
	const_iterator begin() const
	{
		return _m_start;
	}

	iterator end()
	{
		return _m_finish;
	}
	const_iterator end()const
	{
		return _m_finish;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin()const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend()const
	{
		return const_reverse_iterator(begin());
	}

	size_type size()const
	{
		return size_type(end() - begin());
	}

	size_type max_size()const
	{
		return (size_type(-1) / sizeof(value_type));
	}

	bool empty()const
	{
		return _m_start == _m_finish;
	}

	reference at(size_type _n)
	{
		return const_cast<reference> (static_cast<const vector&>(*this).at(_n));
	}

	const_reference at(size_type _n)const
	{
		if (_n < size())
			return *(begin() + _n);
		else
		{
			throw std::out_of_range("out of array range");
		}
	}

	void resize(size_type _new_size, const value_type &val)
	{
		if (_new_size < size())
			erase(begin() + _new_size, end());
		else
		{
			insert(end(), _new_size - size(), val);
		}
	}

	void resize(size_type _new_size)
	{
		resize(_new_size, _Tp());
	}

	// 为容器预定一块更大的空间
	void reserve(size_type _n)
	{
		if(capacity() < _n)
		{
			const size_type _old_size = size();
			iterator _tmp = _M_allocate_and_copy(_m_start, _m_finish, _n);
			destroy(_m_start, _m_finish);
			_M_deallocate(_m_start);

			_m_start = _tmp;
			_m_finish = _m_start + _old_size;
			_m_end = _m_start + _n;
		}
	}

	void swap(vector<_Tp, _Alloc> &other)
	{
		swap(_m_start, other._m_start);
		swap(_m_finish, other._m_finish);
		swap(_m_end, other._m_end);
	}

	reference front()
	{
		return *(begin());
	}

	const_reference front()const
	{
		return *(begin());
	}

	reference back()
	{
		return *(end() - 1);
	}

	const_reference back()const
	{
		return *(end() - 1);
	}

	const_reference operator[](size_type _index)const
	{
		return *(begin() + _index);
	}
	reference operator[](size_type _index)
	{
		return *(begin() + _index);
	}

	iterator insert(iterator _pos, const value_type &_val)
	{
		size_type n = _pos - begin();
		insert(_pos, 1, _val);
		return begin() + n;
	}

	void insert(iterator _pos, size_type _n, const value_type &_val)
	{
		if ((size_type)(_m_end - _m_finish) < _n) // 空间不够
		{
			size_type _old_size = size();
			size_type _len = _old_size + (_old_size > _n ? _old_size : _n);
			iterator _new_start = _M_allocate(_len);
			iterator _new_finish = uninitialized_copy(begin(), _pos, _new_start);
			_new_finish = uninitialized_fill_n(_new_finish, _n, _val);
			_new_finish = uninitialized_copy(_pos, end(), _new_finish);
			
			destroy(begin(), end());
			_M_deallocate(_m_start);
			_m_start = _new_start;
			_m_finish = _new_finish; 
			_m_end = _m_start + _len;
		}
		else if ((size_type)(_m_finish - _pos) < _n)// 填充的值部分需要构造的情况
		{
			size_type _elems_after = _m_finish - _pos;
			uninitialized_fill_n(_m_finish, _n - _elems_after, _val);

			iterator _old_finish = _m_finish;
			_m_finish += (_n - _elems_after);

			uninitialized_copy(_pos, _old_finish, _m_finish);
			_m_finish += _elems_after;
			fill(_pos, _old_finish, _val);
		}
		else if (0 < _n) // 空间够, 填充的值不需要构造的情况
		{
		 	uninitialized_copy(_m_finish - _n, _m_finish, _m_finish);
			iterator _old_finish = _m_finish;
			_m_finish += _n;
			copy_backward(_pos, _old_finish - _n, _old_finish);
			fill_n(_pos, _n, _val);
		}

	}

	void insert(iterator _pos, iterator _first, iterator _last)
	{
		if(_first != _last)
		{
			size_type _n = (size_type)(_last - _first);

			if(size_type(_m_end - _m_finish) >= _n)//空间足够
			{
				const size_type _elem_after = _m_finish - _pos;
				iterator _old_finish = _m_finish;

				if (_elem_after > _n) // 无元素需要构造
				{
					uninitialized_copy(_m_finish - _n, _m_finish, _m_finish);
					_m_finish += _n;
					copy_backward(_pos, _old_finish - _n, _old_finish);
					copy(_first, _last, _pos);
				}
				else //部分元素需要构造
				{
					uninitialized_copy(_first + _elem_after, _last, _m_finish);
					_m_finish += _n - _elem_after;
					uninitialized_copy(_pos, _old_finish, _m_finish);
					_m_finish += _elem_after;
					copy(_first, _first + _elem_after, _pos);
				}
			}
			else
			{
				const size_type _old_size = size();
				const size_type _len = _old_size + (_old_size > _n ? _old_size : _n);
				iterator _new_start = _M_allocate(_len);
				iterator _new_finish = uninitialized_copy(begin(), _pos, _new_start);
				_new_finish = uninitialized_copy(_first, _last, _new_finish);
				_new_finish = uninitialized_copy(_pos, _m_finish, _new_finish);

				destroy(_m_start, _m_finish);
				_M_deallocate(_m_start);
				_m_start = _new_start;
				_m_finish = _new_finish;
				_m_end = _new_start + _len;
			}
		}
	}

	void push_back(const value_type &_val)
	{
		insert(end(), _val);
	}

	size_type capacity()
	{
		return _m_start == nullptr ? 0 : _m_end - _m_start;
	}

	iterator erase(iterator _pos)
	{
		if (_pos + 1 != end())
			copy(_pos + 1, _m_finish, _pos);

		--_m_finish;
		destroy(_m_finish);
		return _pos;
	}

	iterator erase(iterator _first, iterator _last)
	{
		iterator tmp = copy(_last, _m_finish, _first);
		destroy(tmp, _m_finish);
		_m_finish -=  (_last - _first);
		return _first;
	}

	void pop_back()
	{
		erase(end() - 1);
	}

	void clear()
	{
		erase(begin(), end());
	}

	vector& operator=(vector& _other)
	{
		if(this != &_other)
		{
			const size_type _otherLen = _other.size();
			if(_otherLen > capacity())
			{
				iterator _tmp = _M_allocate_and_copy(_other.begin(), _other.end(), _otherLen);
				destroy(_m_start, _m_finish);
				_M_deallocate(_m_start);
				_m_start = _tmp;
				_m_end = _m_start + _otherLen;
			}
			else if (size() >= _otherLen)
			{
				iterator tmp = copy(_other.begin(), _other.end(), begin());
				destroy(tmp, end());

			}
			else
			{
				copy(_other.begin(), _other.begin() + size(), begin());
				uninitialized_copy(_other.begin() + size(), _other.end(), _m_finish);
			}
			_m_finish = _m_start + _otherLen;
		}
		return *this;
	}

	void assign(size_type _n, const _Tp &_val)
	{
		_M_fill_assign(_n, _val);
	}

	void _M_fill_assign(size_type _n, const _Tp &_val);

protected:
	iterator _M_allocate_and_copy(const_iterator _first, const_iterator _last, size_type _n)
	{
		iterator _result = _M_allocate(_n);
		uninitialized_copy(_first, _last, _result);
		return _result;
	}

	void swap(iterator& _lhs, iterator& _rhs)
	{
		iterator _tmp = _lhs;
		_lhs = _rhs;
		_rhs = _tmp;
	}
};

template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_type _n, const _Tp& _val)
{
	if (_n > capacity())
	{
		vector<_Tp, _Alloc> _tmp(_n, _val, get_allocator());
		_tmp.swap(*this);
	}
	else if (_n > size())
	{
		fill(begin(), end(), _val);
		_m_finish = uninitialized_fill_n(_m_finish, _n - size(), _val);
	}
	else
		erase(fill(begin(), _n, _val), end());
}

template<typename _Tp, typename _Alloc>
bool operator==(const vector<_Tp, _Alloc>&_lhs, const vector<_Tp, _Alloc> &_rhs)
{
	return _lhs.size() == _rhs.size() && equal(_lhs.begin(), _lhs.end(), _rhs.begin());
}

template<typename _Tp, typename _Alloc>
bool operator!=(const vector<_Tp, _Alloc>&_lhs, const vector<_Tp, _Alloc> &_rhs)
{
	return !(_lhs == _rhs);
}


template <typename _Tp, typename _Alloc>
bool operator<(const vector<_Tp, _Alloc> &_x, const vector<_Tp, _Alloc> &_y)
{
	return lexicographical_compare(_x.begin(), _x.end(), _y.begin(), _y.end());
}

template <typename _Tp, typename _Alloc>
bool operator>(const vector<_Tp, _Alloc> &_x, const vector<_Tp, _Alloc> &_y)
{
	return !(_x < _y);
}

template <typename _Tp, typename _Alloc>
bool operator<=(const vector<_Tp, _Alloc> &_x, const vector<_Tp, _Alloc> &_y)
{
	return !(_y < _x);
}

template <typename _Tp, typename _Alloc>
bool operator>=(const vector<_Tp, _Alloc> &_x, const vector<_Tp, _Alloc> &_y)
{
	return !(_x < _y);
}


NAMESPACE_END


#endif//!_VECTOR_H_
