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
    //1.创建Lua状态  
    lua_State *L = luaL_newstate();  
    if (L == NULL)  
    {  
        return 0;  
    }  

    //2.加载Lua文件  
    int bRet = luaL_loadfile(L,"1hello.lua");  
    //int bRet = luaL_loadfile(L,"domain-white-list");  
    //int bRet = luaL_loadfile(L,"domain-white-list");  
    if(bRet)  
    {  
        cout<<"load file error"<<endl;  
        return 0;  
    }  

    //3.运行Lua文件  
    bRet = lua_pcall(L,0,0,0);  
    if(bRet)  
    {  
        cout<<"pcall error"<<endl;  
        return 0;  
    }  

    //5.读取table  
    lua_getglobal(L,"ser");//把全局变量ser压入堆栈   
    lua_getfield(L,-1,"name");//name在-1处 
    string str = lua_tostring(L,-1);//取name的第一个值
    cout<<"ser:name = "<<str.c_str()<<endl;   

    lua_getfield(L,-2,"name1");//name1在-2处  
    str = lua_tostring(L,-1);//取name1的第一个值
    cout<<"ser:name1.1 = "<<str.c_str()<<endl;  

/*
    lua_getfield(L,-2,"name1");//name1在-2处  
    str = lua_tostring(L,-2);//取name1的第一个值
    cout<<"tbl:name1 = "<<str.c_str()<<endl; //tbl:name = shun  
*/


    //=================== 栈顶 ===================   
    //  索引  类型      值  
    //   4   int：      30   
    //   3   string：   shun   
    //   2   table:     tbl  
    //   1   string:    I am so cool~  
    //=================== 栈底 ===================   

    //7.关闭state  
    lua_close(L);  
    return 0;  
}
