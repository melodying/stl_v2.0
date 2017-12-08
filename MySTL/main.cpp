
#include "vector.h"
#include <stdio.h>

class Test
{
public:
	Test()
	{
		index = 0;
	}
	void SetData(int *arr, int len)
	{
		for (int i = 0; i < len; ++i)
		{
			data[i] = arr[i];
		}
	}
	int GetData(int index)
	{
		return data[index];
	}
private:
	int data[10];
	int index;
};



int main()
{
	
	/*BA::vector<int, int> vec1;
	BA::vector<int, int> vec;
	for (int i = 0; i < 20; ++i)
	{
		vec1.push_back(i);
	}
	vec = vec1;
	for (int i = 0; i < vec.size(); ++i)
	{
		printf("%d", vec[i]);
	}*/

	BA::vector<Test, int> vec;

	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		vec.push_back(tmp);
	}

	for (auto value : vec)
	{
		for (int i = 0; i < 5; ++i)
		{
			printf("%d ", value.GetData(i));
		}
		printf("\n");
	}
	return 0;
}