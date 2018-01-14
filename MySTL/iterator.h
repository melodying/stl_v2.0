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



#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "config.h"
#include "iterator_base.h"
#include <cstddef>
#include <istream>
#include <iostream>

NAMESPACE_BEGIN


template<typename _Container>
class back_insert_iterator
{
protected:
	_Container *container;
public:
	typedef _Container          container_type;
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;

	// TODO:此处为什么没有const
	explicit back_insert_iterator(_Container &_x):container(_x){}
	back_insert_iterator<_Container>& operator=(const typename _Container::value_type &_val)
	{
		container->push_back(_val);
		return *this;
	}

	back_insert_iterator<_Container>& operator*() 
	{ 
		return *this; 
	}

	back_insert_iterator<_Container>& operator++()
	{ 
		return *this; 
	}

	back_insert_iterator<_Container>& operator++(int)
	{ 
		return *this; 
	}
};

template <typename _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& _x) 
{
	return back_insert_iterator<_Container>(_x);
}


template <typename _Container>
class front_insert_iterator 
{
protected:
	_Container* container;
public:
	typedef _Container          container_type;
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;

	explicit front_insert_iterator(_Container& __x) : container(&__x) {}
	front_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value) 
	{
		container->push_front(__value);
		return *this;
	}
	front_insert_iterator<_Container>& operator*() { return *this; }
	front_insert_iterator<_Container>& operator++() { return *this; }
	front_insert_iterator<_Container>& operator++(int) { return *this; }
};

template <typename _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& _x)
{
	return front_insert_iterator<_Container>(_x);
}


template <typename _Container>
class insert_iterator 
{
protected:
	_Container* container;
	typename _Container::iterator iter;
public:
	typedef _Container          container_type;
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;

	insert_iterator(_Container& __x, typename _Container::iterator __i)
		: container(&__x), iter(__i) {}
	insert_iterator<_Container>& operator=(const typename _Container::value_type& __value) 
	{
		iter = container->insert(iter, __value);
		++iter;
		return *this;
	}
	insert_iterator<_Container>& operator*() { return *this; }
	insert_iterator<_Container>& operator++() { return *this; }
	insert_iterator<_Container>& operator++(int) { return *this; }
};

template <typename _Container, typename _Iterator>
inline insert_iterator<_Container> inserter(_Container& _x, _Iterator _i)
{
	typedef typename _Container::iterator _iter;
	return insert_iterator<_Container>(_x, _iter(_i));
}

template <typename _BidirectionalIterator, typename _Tp, typename _Reference,
	typename _Distance>
class reverse_bidirectional_iterator
{
	typedef reverse_bidirectional_iterator<_BidirectionalIterator, _Tp,
		_Reference, _Distance>  _Self;
protected:
	_BidirectionalIterator current;
public:
	typedef bidirectional_iterator_tag iterator_category;
	typedef _Tp                        value_type;
	typedef _Distance                  difference_type;
	typedef _Tp*                       pointer;
	typedef _Reference                 reference;

	reverse_bidirectional_iterator() {}
	explicit reverse_bidirectional_iterator(_BidirectionalIterator __x): current(__x) {}

	_BidirectionalIterator base() const
	{
		return current;
	}

	_Reference operator*() const
	{
		_BidirectionalIterator _tmp = current;
		return *--_tmp;
	}

	pointer operator->() const
	{ 
		return &(operator*());
	}

	_Self& operator++()
	{
		--current;
		return *this;
	}
	_Self operator++(int)
	{
		_Self _tmp = *this;
		--current;
		return _tmp;
	}
	_Self& operator--()
	{
		++current;
		return *this;
	}
	_Self operator--(int)
	{
		_Self _tmp = *this;
		++current;
		return _tmp;
	}
};

template <typename _BiIter, typename _Tp, typename _Ref, typename _Distance>
inline bool operator==(
	const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& _x,
	const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& _y)
{
	return _x.base() == _y.base();
}

template <typename _BiIter, typename _Tp, typename _Ref, typename _Distance>
inline bool operator!=(
	const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& _x,
	const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& _y)
{
	return !(_x == _y);
}

template<typename _Iterator>
class reverse_iterator
{
protected:
	_Iterator current;
public:
	typedef typename iterator_traits<_Iterator>::iterator_category
		iterator_category;
	typedef typename iterator_traits<_Iterator>::value_type
		value_type;
	typedef typename iterator_traits<_Iterator>::difference_type
		difference_type;
	typedef typename iterator_traits<_Iterator>::pointer
		pointer;
	typedef typename iterator_traits<_Iterator>::reference
		reference;

	typedef _Iterator iterator_type;
	typedef reverse_iterator<_Iterator> _Self;
public:
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type __x) : current(__x) {}

	reverse_iterator(const _Self& __x) : current(__x.current) {}

	iterator_type base() const
	{
		return current;
	}

	reference operator*() const 
	{
		_Iterator _tmp = current;
		return *--_tmp;
	}

	pointer operator->() const
	{ 
		return &(operator*());
	}

	_Self& operator++() 
	{
		--current;
		return *this;
	}
	_Self operator++(int)
	{
		_Self _tmp = *this;
		--current;
		return _tmp;
	}
	_Self& operator--()
	{
		++current;
		return *this;
	}
	_Self operator--(int)
	{
		_Self _tmp = *this;
		++current;
		return _tmp;
	}

	_Self operator+(difference_type _n) const
	{
		return _Self(current - _n);
	}
	_Self& operator+=(difference_type _n) 
	{
		current -= _n;
		return *this;
	}
	_Self operator-(difference_type _n) const 
	{
		return _Self(current + _n);
	}
	_Self& operator-=(difference_type _n)
	{
		current += _n;
		return *this;
	}
	reference operator[](difference_type _n) const
	{
		return *(*this + _n);
	}

};

template <typename _Iterator>
inline bool operator==(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y)
{
	return _x.base() == _y.base();
}

template <typename _Iterator>
inline bool operator<(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y)
{
	return _y.base() < _x.base();
}

template <typename _Iterator>
inline bool operator!=(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y) 
{
	return !(_x == _y);
}

template <typename _Iterator>
inline bool operator>(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y)
{
	return _y < _x;
}

template <typename _Iterator>
inline bool operator<=(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y) 
{
	return !(_y < _x);
}

template <typename _Iterator>
inline bool operator>=(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y)
{
	return !(_x < _y);
}

template <typename _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator>& _x,
	const reverse_iterator<_Iterator>& _y) 
{
	return _y.base() - _x.base();
}

template <typename _Iterator>
inline reverse_iterator<_Iterator>
operator+(typename reverse_iterator<_Iterator>::difference_type _n,
	const reverse_iterator<_Iterator>& _x)
{
	return reverse_iterator<_Iterator>(_x.base() - _n);
}

// TODO : IO itreator

template<typename _Tp, typename _Dist = ptrdiff_t>
class istream_iterator;

template<typename _Tp, typename _Dist>
bool operator==(const istream_iterator<_Tp, _Dist>&,
	const istream_iterator<_Tp, _Dist>&);

template<typename _Tp, typename _Dist>
class istream_iterator
{
protected:
	std::istream* _m_stream;
	_Tp _m_value;
	bool _m_end_marker;

	void _M_read()
	{
		_m_end_marker = (*_m_stream) ? true : false;
		if (_m_end_marker)
			*_m_stream >> _m_value;

		_m_end_marker = (*_m_stream) ? true : false;
	}

public:
	typedef input_iterator_tag  iterator_category;
	typedef _Tp                 value_type;
	typedef _Dist               difference_type;
	typedef const _Tp*          pointer;
	typedef const _Tp&          reference;

	istream_iterator() : _m_stream(&std::cin), _m_end_marker(false) {}

	istream_iterator(std::istream& __s) : _m_stream(&__s)
	{
		_M_read();
	}

	reference operator*() const
	{ 
		return _m_value; 
	}
	pointer operator->() const 
	{ 
		return &(operator*()); 
	}

	istream_iterator<_Tp, _Dist>& operator++()
	{
		_M_read();
		return *this;
	}

	istream_iterator<_Tp, _Dist> operator++(int)
	{
		istream_iterator<_Tp, _Dist> _tmp = *this;
		_M_read();
		return _tmp;
	}
};

template<typename _Tp, typename _Dist>
bool operator==(const istream_iterator<_Tp, _Dist>& _x,
	const istream_iterator<_Tp, _Dist>& _y)
{
	return (_x._m_stream == _y._m_stream && _x._m_end_marker == _y._m_end_marker) ||
		_x._m_end_marker == false && _y._m_end_marker == false;
}

template <typename _Tp, typename _Distance>
inline bool operator!=(const istream_iterator<_Tp, _Distance>& _x,
	const istream_iterator<_Tp, _Distance>& _y) 
{
	return !(_x == _y);
}


template<typename _Tp>
class ostream_iterator
{
protected:
	std::ostream *_m_stream;
	const char *_m_string;
public:
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;

	ostream_iterator(std::ostream& __s) : _m_stream(&__s), _m_string(0) {}
	ostream_iterator(std::ostream& __s, const char* __c): _m_stream(&__s), _m_string(__c) {}

	ostream_iterator<_Tp>& operator=(const _Tp& __value)
	{
		*_m_stream << __value;
		if (_m_string) *_m_stream << _m_string;
		return *this;
	}
	ostream_iterator<_Tp>& operator*()
	{
		return *this; 
	}
	ostream_iterator<_Tp>& operator++()
	{ 
		return *this; 
	}
	ostream_iterator<_Tp>& operator++(int)
	{ 
		return *this; 
	}
};

template <class _Tp>
inline output_iterator_tag
iterator_category(const ostream_iterator<_Tp>&)
{
	return output_iterator_tag();
}


NAMESPACE_END

#endif//!_ITERATOR_H_