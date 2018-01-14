/*
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

#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


struct _true_type{};
struct _false_type{};

template<typename _Tp>
struct _type_traits
{
	typedef _true_type		 this_dummy_member_must_be_first;

	typedef _false_type		 has_trivial_default_constructor;
	typedef _false_type		 has_trivial_copy_constructor;
	typedef _false_type      has_trivial_assignment_operator;
	typedef _false_type      has_trivial_destructor;
	typedef _false_type      is_POD_type;
};

template<>
struct _type_traits<bool> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<char> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<signed char> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<unsigned char>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<wchar_t> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<short>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<unsigned short>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<int>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<unsigned int> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<long> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<unsigned long> 
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<long long>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<unsigned long long>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<float>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<double>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<>
struct _type_traits<long double>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<typename _Tp>
struct _type_traits<_Tp*>
{
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_copy_constructor;
	typedef _true_type    has_trivial_assignment_operator;
	typedef _true_type    has_trivial_destructor;
	typedef _true_type    is_POD_type;
};

template<typename _Tp>
struct _Is_integer
{
	typedef _false_type _Integral;
};

template<>
struct _Is_integer<bool>
{
	typedef _true_type _Integral;
};

template<>
struct _Is_integer<char>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<unsigned char>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<signed char>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<wchar_t>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<short>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<unsigned short>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<int>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<unsigned int>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<long>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<unsigned long>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<long long>
{
	typedef _true_type _Integral;

};

template<>
struct _Is_integer<unsigned long long>
{
	typedef _true_type _Integral;

};



#endif//!_TYPE_TRAITS_H_