#include <sstream>  
#include <string>  
#include <iostream>  
using namespace std;  

//ios_base->ios->ostream->ostringstream
//ostringstream类通常用于执行C风格的串流的输出操作，格式化字符串，避免申请大量的缓冲区，替代sprintf。
//该类能够根据内容自动分配内存，并且其对内存的管理也是相当的到位。

int main()  
{  
    ostringstream oss1;  
    ostringstream oss2("abc");

    /*---------------------------------------------------------------------------- 
     *** 方法str()将缓冲区的内容复制到一个string对象中，并返回 
     ----------------------------------------------------------------------------*/  
    oss1 << "oss1 " << 2017 << endl; // 格式化，此处endl也将格式化进ostr1中  
    cout << oss1.str();   

    /*---------------------------------------------------------------------------- 
     *** 建议：在用put()方法时，先查看当前put pointer的值，防止误写 
     ----------------------------------------------------------------------------*/  
    long cur_pos = oss2.tellp(); //返回当前插入的索引位置(即put pointer的值)，从0开始   
    cout << "cur_pos = " << cur_pos << endl;   //0

    oss2.seekp(2); // 手动设置put pointer的值  
    oss2.put('g');     // 在put pointer的位置上写入'g'，并将put pointer指向下一个字符位置  
    cout << oss2.str() << endl;   //abg


    /*---------------------------------------------------------------------------- 
     *** 重复使用同一个ostringstream对象时，建议： 
     *** 1：调用clear()清除当前错误控制状态，其原型为 void clear (iostate state=goodbit); 
     *** 2：调用str("")将缓冲区清零，清除脏数据 
     ----------------------------------------------------------------------------*/  
    oss2.clear();  
    oss2.str("");   
    cout << oss2.str() << endl;   //空

    oss2.str("_def");  
    cout << oss2.str() << endl;  
    oss2 << "gggghh";    // 覆盖原有的数据，并自动增加缓冲区  
    cout << oss2.str() << endl;  

    return 0;
}  
