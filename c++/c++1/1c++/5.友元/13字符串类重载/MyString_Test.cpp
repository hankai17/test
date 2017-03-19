
#include "MyString.h"

//c中没有字符串 所以定义一个字符串类（以0结尾的）


void main1()
{
	MyString s1;
	MyString s2("s2");
	MyString s2_2 = NULL;
	MyString s3 = s2;
	MyString s4 = "s444444444";
	
	//s4 = "s2222";
	//s4 = s2;
	//MyString& operator=(const char * p);
	//MyString& operator=(const MyString & s);

	//s4 = "s2222";
	//s4[1] = '4';
	//printf("%c\n", s4[1]);

	//cout << s4 << endl;
	//ostream &operator<<(ostream & out,MyString &s)


	system("pause");
	return;
}


//if中是==

void main2()
{
	MyString s1;
	MyString s2("s2");
	MyString s3 = s2;


	if (s2 == "s222222")// bool operator==(const char * p);
	{
		printf("相等");
	}
	else
	{
		printf("不相等");
	}

	if (s2 == s3)//bool operator==(const MyString & s);bool operator==(const char * p);
	{
		printf("相等");
	}
	else
	{
		printf("不相等");
	}
}

void main3()
{
	MyString s1;
	MyString s2("s2");
	MyString s3 = s2;
	s3 = "aaa";
	if (s3 < "bbbb")// int operator<(const char * p);
	{
		printf("s3小于bbb");
	}
	else 
	{
		printf("s3大于bbb");
	}
	MyString s4 = "sdafadsf";
	strcpy(s4.c_str(), "aa33");//MFC
	//cout << s4 << endl;
}

