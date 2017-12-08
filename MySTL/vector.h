#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "config.h"
#include <memory.h>
#include <stdlib.h>

NAMESPACE_BEGIN

template <typename _T, typename _Alloc>
class vector
{
public:
	typedef vector<_T, _Alloc> _Base;

	typedef _Alloc				allcoator_type;
	typedef size_t				size_type;
	typedef _T					value_type;
	typedef value_type*			iterator;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef const value_type*	const_iterator;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef int					difference_type;

protected:
	iterator _First;
	iterator _Last;
	iterator _End;

public:
	explicit vector():_First(nullptr), _Last(nullptr), _End(nullptr)
	{
	}

	vector(size_type _n, const value_type &val = value_type())
	{
		_First = (value_type*)malloc(_n * sizeof(value_type));
		memset(_First, val, sizeof(value_type)*_n);
		_Last = _First + _n;
		_End = _Last;
	}

	template <typename InputIterator>
	vector(InputIterator first, InputIterator last)
	{
		size_type _n = last - first;
		_First = (value_type*)malloc(_n * sizeof(value_type));
		
		for (InputIterator it = first; it != last; ++it)
		{
			iterator tmp = _First;
			*tmp = *it;
			++tmp;
		}

		_Last = _First + _n;
		_End = _Last;
	}

	~vector()
	{
		for (iterator it = _First; it != _Last; ++it)
		{
			it->~_T();
		}
		free(_First);
		_First = _Last = _End = nullptr;
	}

	iterator begin()
	{
		return _First;
	}
	const_iterator begin() const
	{
		return _First;
	}

	iterator end()
	{
		return _Last;
	}
	const_iterator end()const
	{
		return _Last;
	}

	size_type size()const
	{
		return _Last - _First;
	}

	size_type max_size()const
	{
		return -1 / sizeof(value_type);
	}

	bool empty()const
	{
		return _First == _Last;
	}

	reference at(size_type n)
	{
		if(n < size())
			return *(begin() + n);
		else
		{//may be throw
			return *begin();
		}
	}

	const_reference at(size_type n)const
	{
		if (n < size())
			return *(begin() + n);
		else
		{//may be throw
			return *begin();
		}
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

	const_reference operator[](size_type index)const
	{
		return *(begin() + index);
	}
	reference operator[](size_type index)
	{
		return *(begin() + index);
	}

	iterator insert(iterator pos, const value_type &val)
	{
		size_type n = pos - begin();
		insert(pos, 1, val);
		return begin() + n;
	}

	void insert(iterator pos, size_type n, const value_type &val)
	{
		if (_End - _Last < n) // 空间不够
		{
			size_type len = size() + (n < size() ? size() : n);
			iterator first = (iterator)malloc(sizeof(value_type) * len);
			
			// 将之前(_First -> pos)的元素拷贝到新空间, 并进行构造
			iterator last = _copy(_First, pos, first);

			// 在新空间中填充n个val
			_fill(last, n, val);
			
			// 拷贝 (pos -> _Last)到新空间
			_copy(pos, _Last, last + n);

			// 析构原空间, 释放原空间
			for (const_iterator it = _First; it != _Last; ++it)
			{
				it->~_T();
			}
			free(_First);

			// 指向新空间
			_End = first + len;
			_Last = first + size() + n;
			_First = first;
		}
		else if (_Last - pos < n)// 填充的值部分需要构造的情况
		{
			_copy(pos, _Last, pos + n);				
			_fill(_Last, n - (_Last - pos), val);
			// 直接填充pos -> _Last
			for(iterator it = pos; it != _Last; ++it)
			{
				*it = val;
			}
			_Last += n;
		}
		else if (0 < n) // 空间够, 填充的值不需要构造的情况
		{
			_copy(_Last - n, _Last, _Last);

			iterator tmp = pos;
			for (iterator it = _Last - n; it != _Last; ++it)
			{
				*it = *tmp;
				++tmp;
			}

			for (iterator it = pos; it != pos + n; ++it)
			{
				*it = val;
			}
			_Last += n;
		}

	}

	void insert(iterator pos, iterator begin, iterator end)
	{//在pos位置插入[first, last)个元素
		size_type n = begin - end;

		if (_End - _Last < n) // 空间不够
		{
			size_type len = size() + (n < size() ? size() : n);
			iterator _first = (iterator)malloc(sizeof(value_type) * len);

			iterator _last = _copy(_First, pos, _first);
			_last = _copy(begin, end, _last);
			_copy(pos, _Last, _last); 
			
			// 析构原空间, 释放原空间
			for (const_iterator it = _First; it != _Last; ++it)
			{
				it->~_T();
			}
			free(_First);

			// 指向新空间
			_End = _first + len;
			_Last = _first + size() + n;
			_First = _first;
		}
		else if (_Last - pos < n)// 填充的值部分需要构造的情况
		{
			_copy(pos, _Last, pos + n);
			_copy(begin + (_Last - pos), end, _Last);

			iterator tmp = pos;
			for (iterator it = begin; it != begin + (_Last - pos); ++it)
			{
				*pos = *it;
				++tmp;
			}
			_Last += n;
		}
		else if (0 < n) // 空间够, 填充的值不需要构造的情况
		{
			_copy(_Last - n, _Last, _Last);

			iterator tmp = pos;
			for (iterator it = _Last - n; it != _Last; ++it)
			{
				*it = *tmp;
				++tmp;
			}

			tmp = pos;
			for (iterator it = begin; it != end; ++it)
			{
				*tmp = *it;
				++tmp;
			}
			_Last += n;
		}
	}

	void push_back(value_type &val)
	{
		insert(end(), val);
	}

	size_type capacity()
	{
		return _First == nullptr ? 0 : _End - _First;
	}

	iterator erase(iterator pos)
	{
		// 将pos+1 到end()向前移一位
		iterator tmp = pos;
		for (iterator it = pos + 1; it != end(); ++it)
		{
			*tmp = *it;
			++tmp;
		}
		pos->~_T();
		--_Last;
		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator tmp = first;
		for (iterator it = last; it != _Last; ++it)
		{
			*tmp = *it;
			++tmp;
		}

		for (iterator it = tmp; it != _Last; ++it)
		{
			it->~_T();
		}
		_Last -= (last - first);
		return first;
	}

	void assign(iterator first, iterator last)
	{
		erase(begin(), end());
		insert(begin(), first, last);
	}

	void pop_back()
	{
		erase(end() - 1);
	}

	void clear()
	{
		erase(begin(), end());
	}

	_Base& operator=(_Base& other)
	{
		if(this != &other)
		{
			const size_type len = other.size();
			if (len > capacity())//超过容器总长时,重新分配空间
			{
				iterator first = (iterator)malloc(sizeof(value_type) * len);
				iterator last = _copy(other.begin(),other.end(), first);

				for (iterator it = _First; it != _Last; ++it)
					it->~_T();

				free(_First);
				_First = first;
				_End = _First + len;
			}
			else if(size() >= len)
			{
				iterator tmp = begin();
				for (iterator it = other.begin(); it != other.end(); ++it)
				{
					*tmp = *it;
					++tmp;
				}
				for (iterator it = tmp; it != end(); ++it)
				{
					it->~_T();
				}
			}
			else//长度大于size()且小于总长
			{
				iterator tmp = begin();
				for (iterator it = other.begin(); it != other.begin() + size(); ++it)
				{
					*tmp = *it;
					++tmp;
				}
				_copy(other.begin() + size(), other.end(), _Last);
			}
			_Last = _First + len;
		}
		return *this;
	}

protected:
	// 进行构造的拷贝
	iterator _copy(const_iterator first, const_iterator last, iterator des)
	{
		for (; first != last; ++first, ++des)
		{
			*des = _T(*first);
		}
		return des;
	}
	//进行构造的填充
	void _fill(iterator first, size_type n, const value_type &val)
	{
		for (; n > 0; --n, ++first)
		{
			*first = _T(val);
		}
	}
};



NAMESPACE_END


#endif//!_VECTOR_H_
