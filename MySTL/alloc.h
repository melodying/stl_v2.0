#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <stdlib.h>

template <typename _Tp>
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



#endif//!_ALLOC_H_