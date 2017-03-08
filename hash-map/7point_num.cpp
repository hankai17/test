/*************************************************************************
	> File Name: 7point_num.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-09 16:35:09
 ************************************************************************/

#include<iostream>
using namespace std;

int nCountOfPoint(const string& paraStrUrl)  
{  
    int nSum = 0 ;  
    string::size_type nPos = 0 ;  
    while ((nPos = paraStrUrl.find_first_of(".", nPos)) != string::npos)  
    {  
        ++nPos ;  
        ++nSum ;  
    }  
  
    return nSum ;  
} 

int main()
{
	int pn=nCountOfPoint("www.baidu.com.1.mp4");
	cout<<"pn:"<<pn<<endl;
	return 0;
}
