#include<iostream>
using namespace std;
int main()
{
    int ret = system("curl http://www.ip.cn/index.php?ip=202.108.249.253");
    cout<<"ret="<<ret<<endl;

    int ret1 = system("curl http://www.ip138.com/ips138.asp?ip=202.108.249.253&action=2");
    cout<<"ret1="<<ret1<<endl;

    return 0;
}

