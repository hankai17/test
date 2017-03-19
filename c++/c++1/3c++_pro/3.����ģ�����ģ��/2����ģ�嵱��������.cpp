#include <iostream> 
using namespace std;

//对字符数组 int数组进行排序
template <typename T1,typename T2>
//int mySort(int *array, int size)
int mySort(T1 *array, T2 size)
{
	T2 i, j;
	T1 tmp;
	if (array == NULL)
	{
		return -1;
	}
	for (int i = 0; i < size; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			if (array[i] < array[j])
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
	return 0;
}

template <typename T,typename T2>
int myPrint(T *array, T2 size)
{
	T2 i = 0;
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	return 0;
}
void main()
{
	/*
	//int类型
	{
		int myarray[] = { 11, 22, 44, 23, 45, 76, 3, 9 };
		int size = sizeof(myarray) / sizeof(*myarray);
		mySort(myarray, size);
		printf("排序之后\n");
		mySort<int, int>(myarray, size);//mySort(myarray,size);
		myPrint<int, int>(myarray, size);//类型参数化myPrint(myarray, size);
		system("pause");
		return;
	}
	*/
	//char类型
	char buf[] = "amtysdfgfhaaaaaafdsafa";
	int len = strlen(buf);
	mySort<char, int>(buf, len);
	myPrint<char, int>(buf, len);
	system("pause");
	return;

	//函数模板本质：类型参数化

}