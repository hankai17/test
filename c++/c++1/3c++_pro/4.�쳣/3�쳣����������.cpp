#include <iostream>
using namespace std;

//throw int char* 异常
void my_strcpy2(char *to, char * from)
{
	if (from == NULL)
	{
		throw "源buf出错";//此时在全局区域分配内存
	}
	if (to == NULL)
	{
		throw "目的buf出错";
	}
	//copy时的场景检查
	if (*from == 'a')
	{
		throw "拷贝时出错";
	}

	while (*from != '\0')
	{
		*to = *from;
		to++;
		from++;
	}
	*to = '/0';
	
}


//传统机制
int my_strcpy(char *to, char * from)
{
	if (from == NULL)
	{
		return 1;
	}
	if (to == NULL)
	{
		return 2;
	}
	//copy时的场景检查
	if (*from == 'a')
	{
		return 3;
	}
	
	while (*from != '\0')
	{
		*to = *from;
		to++;
		from++;
	}
	*to = '/0';
	return 0;
}
void main2()
{
	int ret = 0;
	char buf1[] = "asldfkfsda";
	char buf2[1024] = { 0 };

	try
	{
		my_strcpy2(buf2, buf1);
	}
	catch (char * e)
	{
		cout << "char *类型异常" << endl;
	}
	catch (...)
	{
		cout << "未知异常" << endl;

	}

	/*
	if (ret != 0)
	{
		switch (ret)
		{
		case 1:
			printf("源buf出错！\n");
			break;
		case 2:
			printf("目的buf出错！\n");
			break;
		case 3:
			printf("copy过程出错！\n");
			break;
		default:
			printf("未知错！\n");
			break;
		}
	}

	printf("buf2:%s \n", buf2);
	*/
	system("pause");
	return;
}

void main1()
{
	int ret = 0;
	char buf1[] = "asldfkfsda";
	char buf2[1024] = {0};

	ret = my_strcpy(buf2, buf1);
	if (ret != 0)
	{
		switch (ret)
		{
		case 1:
			printf("源buf出错！\n");
			break;
		case 2:
			printf("目的buf出错！\n");
			break;
		case 3:
			printf("copy过程出错！\n");
			break;
		default:
			printf("未知错！\n");
			break;
		}
	}

	printf("buf2:%s \n", buf2);

	system("pause");
	return;
}

class BadSrcType {};
class BadDesType {};
class BadProcessType 
{
public:
	BadProcessType()
	{
		cout << "构造" << endl;
	}
	~BadProcessType()
	{
		cout << "析构" << endl;
	}
	BadProcessType(const  BadProcessType &obj)
	{
		cout << "拷贝构造" << endl;
	}


};

void my_strcpy3(char *to, char * from)
{
	if (from == NULL)
	{
		throw BadSrcType();//会不会产生一个匿名对象？
	}
	if (to == NULL)
	{
		throw BadDesType();
	}
	//copy时的场景检查
	if (*from == 'a')
	{
		throw BadProcessType();
	}

	while (*from != '\0')
	{
		*to = *from;
		to++;
		from++;
	}
	*to = '/0';

}

void main()
{
	int ret = 0;
	char buf1[] = "asldfkfsda";
	char buf2[1024] = { 0 };

	try
	{
		my_strcpy3(buf2, buf1);
	}
	//1结论：如果接受异常的时候 使用一个异常变量 则拷贝构造异常变量//两次拷贝构造
	
	catch (BadSrcType e)//是把匿名对象拷贝给e 还是 e还是那个匿名对象？
	{
		cout << "BadSrcType类型异常" << endl;
	}
	
	catch (BadDesType e)
	{
		cout << "BadDesType类型异常" << endl;
	}
	//2结论：使用引用的话 会使用throw的那个对象  //一次拷贝构造
	//catch (BadProcessType &e)
	//3结论：指针可以和引用，元素写一块 但是引用不能跟元素写一块
	catch (BadProcessType *e)
	{
		cout << "BadProcessType类型异常" << endl;
	}
	catch (...)
	{
		cout << "未知异常" << endl;

	}



	system("pause");
	return;
}