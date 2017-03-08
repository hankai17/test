/*************************************************************************
  > File Name: 1hello.cpp
  > Author: Bill
  > Mail: hankai17@126.com 
  > Created Time: 2017-01-03 13:48:46
 ************************************************************************/

#include<iostream>
using namespace std;

#include <iostream>  
#include <string.h>  
using namespace std;  

extern "C"  
{  
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}  
int main()  
{  
    //1.����Lua״̬  
    lua_State *L = luaL_newstate();  
    if (L == NULL)  
    {  
        return 0;  
    }  

    //2.����Lua�ļ�  
    int bRet = luaL_loadfile(L,"1hello.lua");  
    //int bRet = luaL_loadfile(L,"domain-white-list");  
    //int bRet = luaL_loadfile(L,"domain-white-list");  
    if(bRet)  
    {  
        cout<<"load file error"<<endl;  
        return 0;  
    }  

    //3.����Lua�ļ�  
    bRet = lua_pcall(L,0,0,0);  
    if(bRet)  
    {  
        cout<<"pcall error"<<endl;  
        return 0;  
    }  

    //5.��ȡtable  
    lua_getglobal(L,"ser");//��ȫ�ֱ���serѹ���ջ   
    lua_getfield(L,-1,"name");//name��-1�� 
    string str = lua_tostring(L,-1);//ȡname�ĵ�һ��ֵ
    cout<<"ser:name = "<<str.c_str()<<endl;   

    lua_getfield(L,-2,"name1");//name1��-2��  
    str = lua_tostring(L,-1);//ȡname1�ĵ�һ��ֵ
    cout<<"ser:name1.1 = "<<str.c_str()<<endl;  

/*
    lua_getfield(L,-2,"name1");//name1��-2��  
    str = lua_tostring(L,-2);//ȡname1�ĵ�һ��ֵ
    cout<<"tbl:name1 = "<<str.c_str()<<endl; //tbl:name = shun  
*/


    //=================== ջ�� ===================   
    //  ����  ����      ֵ  
    //   4   int��      30   
    //   3   string��   shun   
    //   2   table:     tbl  
    //   1   string:    I am so cool~  
    //=================== ջ�� ===================   

    //7.�ر�state  
    lua_close(L);  
    return 0;  
}
