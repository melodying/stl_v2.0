
#include "vector.h"
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include "set.h"
#include "map.h"

#include <stdio.h>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include "algorithm.h"

class Test
{
public:
	Test()
	{
		_len = 5;
		memset(data, 0, 40);
	}
	void SetData(int *arr, int len)
	{
		_len = len;
		for (int i = 0; i < len; ++i)
		{
			data[i] = arr[i];
		}
	}
	int GetData(int index)
	{
		return data[index];
	}

	void Print()
	{
		for (int i = 0; i < _len; ++i)
		{
			printf("%d ", data[i]);
		}
		printf("\n");
	}

	bool operator<(const Test &_t)const
	{
		return _len < _t._len;
	}
private:
	int data[10];
	int _len;
};


void VectorTest()
{
	// 自定义的类类型
	int arr[5] = { 1, 2, 3, 4, 5 };
	Test test;
	test.SetData(arr, 5);
	BA::vector<Test> vecTest;
	for (int i = 0; i < 10; ++i)
	{
		vecTest.push_back(test);
	}

	for (auto value : vecTest)
	{
		value.Print();
	}

	auto it1 = vecTest.begin();
	it1 = vecTest.end();
	auto it2 = vecTest.rend();
	it2 = vecTest.rbegin();

	vecTest.size();
	vecTest.resize(10);
	vecTest.capacity();
	vecTest.empty();
	vecTest.reserve(20);
	vecTest[2].Print();
	vecTest.front();
	vecTest.back();
	vecTest.pop_back();

	// TODO: 插入之后迭代器可能失效. 不能直接用之前的迭代器. 
	// 连续的插入操作最好别用同一个迭代器来控制.因为进行插入操作的时候可能导致空间重新分配. 迭代器会失效
	/*it1 = vecTest.begin();
	vecTest.insert(it1 + 2, test);
	vecTest.insert(it1 + 3, 3, test);
	vecTest.insert(it1 + 1, vecTest.begin(), vecTest.end());*/

	it1 = vecTest.begin();
	vecTest.erase(it1 + 2);
	vecTest.erase(it1 + 2, it1 + 3);

	BA::vector<Test> vec;
	vecTest.swap(vec);

	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		it->Print();
	}

	vec.clear();
}

void ListTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}

	BA::list<Test> list;
	BA::list<Test> list1(test, test + 5);
	BA::list<Test> list2(list1.begin(), list1.end());
	BA::list<Test> list3(5);
	BA::list<Test> list4(list2);

	BA::list<int> li(arr, arr + 5);
	BA::list<int> li2;
	for (int i = 6; i < 11; ++i)
		li2.push_back(i);

	BA::list<int>::iterator it = li2.begin();
	++it;
	++it;
	//li2.splice(it, li);
	li2.merge(li);

	for (auto it1 = li2.begin(); it1 != li2.end(); ++it1)
	{
		printf("%d ", *it1);
	}

	printf("\n");
	for (auto it1 = li.begin(); it1 != li.end(); ++it1)
	{
		printf("%d ", *it1);
	}
	li.clear();
	li.push_back(1);
	li.push_back(3);
	li.push_back(2);
	li.push_back(5);
	li.push_back(4);
	li.sort();
	li.begin();
	li.end();
	li.empty();
	li.size();
	li.front();
	li.back();
	li.pop_back();
	li.push_front(0);
	li.pop_front();
	li.insert(li.begin(), 4);
	li.erase(li.begin());
	li.resize(8);
	li.remove(3);

	BA::list<int> l1;
	BA::list<int> l2;
	l1.push_back(1);
	l1.push_back(2);
	l1.push_back(3);
	l2.push_back(4);
	l2.push_back(5);
	l1.merge(l2);

	for (auto l11 : l1)
	{
		printf("%d", l11);
	}
	for (auto l22 : l2)
	{
		printf("%d", l22);
	}

}

void DequeTest()
{
	Test test;
	Test t1[10];
	int arr[5] = { 1,2,3,4,5 };
	test.SetData(arr, 5);
	BA::deque<Test> deque(5, test);

	for (size_t i = 0; i < deque.size(); ++i)
		deque[i].Print();

	
	deque.push_front();
	for (size_t i = 0; i < deque.size(); ++i)
		deque[i].Print();

	const BA::deque<Test> de;
	BA::deque<Test>::const_iterator it(de.begin());

	
	auto it1 = de.begin();
	it1 = it1 + 1;
	it1 = 1 + it1;
	auto a = de[1];

	deque.insert(deque.begin() + 2, test);
	deque.insert(deque.begin() + 1, 10, test);
	deque.insert(deque.begin(), it,it + 1);
	deque.insert(deque.begin(), deque.begin(), deque.end());
}

void StackTest()
{
	BA::stack<int> s;
	BA::stack<int> s1;
	s.push(1);
	s.pop();
	s.top();
	s.empty();
	s.size();
	if (s == s1)
	{

	}
	if (s < s1)
	{

	}
}

void QueueTest()
{
	BA::queue<int> s;
	BA::queue<int> s1;
	s.push(1);
	s.pop();
	
	s.empty();
	s.size();

	if(s == s1)
	{
		
	}
	if(s < s1)
	{
		
	}
	
}

void PriorityQueueTest()
{
	BA::priority_queue<int> pq;
	pq.push(3);
	pq.push(1);
	pq.push(5);
	pq.push(2);
	int len = pq.size();
	int top = pq.top();
	pq.pop();
	top = pq.top();

}


void setTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}
	
	BA::set<Test> s;
	s.begin();
	s.end();
	s.clear();
	for (int i = 0; i < 5; ++i)
	{
		s.insert(test[i]);
	}

	printf("%d\n", s.count(test[2]));
	
}

void mapTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}

	BA::map<char, Test> m;

	for (int i = 0; i < 5; ++i)
	{
		const char c = i + '0';
		// TODO: map的插入bug。又一个由const引发的错误
		// 在map中的value_type的key应为const。因为构造的pair中的key并不是const。造成无法插入
		// 如果将构造的pair中的first_type设置为const。make_pair()无法工作
		// 如果使用pair的拷贝构造则没有问题。问题应该是出在const的成员变量只能初始化。无法赋值
		
		// 需要手动构造const first_type的pair。使用起来不够友好
		BA::pair<const char, Test> pair(c, test[i]);
		
		m.insert(pair);
	}

	m['0'].Print();
}

int main()
{
	VectorTest();
	ListTest();
	DequeTest();
	StackTest();
	QueueTest();
	PriorityQueueTest();

	
	return 0;
}