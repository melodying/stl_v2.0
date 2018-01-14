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




#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdlib.h>
#include "config.h"

NAMESPACE_BEGIN

template <int _Tp>
class _malloc_alloc
{
public:
	static void *allocate(size_t _n)
	{
		void *_result = malloc(_n);
		return _result;
	}
	static void deallocate(void *_p)
	{
		free(_p);
	}
};

template<typename _TP, typename _Alloc>
class simple_alloc
{
public:
	static _TP* allocate(size_t _n)
	{
		return 0 == _n ? nullptr : (_TP*)_Alloc::allocate(_n * sizeof(_TP));
	}

	static void deallocate(_TP *_p)
	{
		if (nullptr != _p)
			_Alloc::deallocate(_p);
	}
};

typedef _malloc_alloc<0> malloc_alloc;

typedef malloc_alloc alloc;


NAMESPACE_END

#endif//!_ALLOC_H_