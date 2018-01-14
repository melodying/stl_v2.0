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
*/


#ifndef _TEMPBUF_H_
#define _TEMPBUF_H_


#include <cstddef>
#include <cstdlib>

#include "config.h"
#include "pair.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "iterator_base.h"

NAMESPACE_BEGIN

template<typename _Tp>
pair<_Tp*, ptrdiff_t> _get_temporary_buffer(ptrdiff_t _len, _Tp*)
{
	if (_len > ptrdiff_t(INT_MAX / sizeof(_Tp)))
		_len = INT_MAX / sizeof(_Tp);

	while (_len > 0)
	{
		_Tp *_tmp = (_Tp*)malloc(_len * sizeof(_Tp));
		if (_tmp)
			return pair<_Tp*, ptrdiff_t>(_tmp, _len);

		_len /= 2;
	}
	return pair<_Tp*, ptrdiff_t>((_Tp*)0, 0);
}

template<typename _Tp>
pair<_Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t _len, _Tp*)
{
	return _get_temporary_buffer(_len, (_Tp*)0);
}

template<typename _Tp>
void return_temporary_buffer(_Tp* _p)
{
	free(_p);
}

template<typename _ForwardIter, typename _Tp>
class _Temporary_buffer
{
private:
	ptrdiff_t _m_original_len;
	ptrdiff_t _m_len;
	_Tp *	  _m_buffer;

	void _M_allocate_buffer()
	{
		_m_original_len = _m_len;
		_m_buffer = 0;

		if (_m_len > (ptrdiff_t)(INT_MAX / sizeof(_Tp)))
			_m_len = INT_MAX / sizeof(_Tp);

		while (_m_len > 0)
		{
			_m_buffer = (_Tp *)malloc(_m_len * sizeof(_Tp));
			if(_m_buffer)
				break;

			_m_len /= 2;
		}
	}

	void _M_initialize_buffer(const _Tp&, _true_type) {}
	void _M_initialize_buffer(const _Tp& _val, _false_type)
	{
		uninitialized_fill_n(_m_buffer, _m_len, _val);
	}
public:
	ptrdiff_t size() const
	{
		return _m_len;
	}
	ptrdiff_t requested_size() const
	{
		return _m_original_len;
	}
	_Tp* begin()
	{
		return _m_buffer;
	}
	_Tp* end()
	{
		return _m_buffer + _m_len;
	}

	_Temporary_buffer(_ForwardIter _first, _ForwardIter _last)
	{
		typedef typename _type_traits<_Tp>::has_trivial_default_constructor _Trivial;

		_m_len = 0;
		distance(_first, _last, _m_len);
		_M_allocate_buffer();
		if (_m_len > 0)
			_M_initialize_buffer(*_first, _Trivial());
	}

	~_Temporary_buffer()
	{
		destroy(_m_buffer, _m_buffer + _m_len);
		free(_m_buffer);
	}

private:
	_Temporary_buffer(const _Temporary_buffer&) = delete;
	void operator=(const _Temporary_buffer&) = delete;
};

template <class _ForwardIterator,
	class _Tp = typename iterator_traits<_ForwardIterator>::value_type>
struct temporary_buffer : public _Temporary_buffer<_ForwardIterator, _Tp>
{
	temporary_buffer(_ForwardIterator _first, _ForwardIterator _last)
		: _Temporary_buffer<_ForwardIterator, _Tp>(_first, _last) {}
	~temporary_buffer() {}
};






NAMESPACE_END


#endif//!_TEMPBUF_H_