/*************************************************************************
	> File Name: main.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-01-19 17:37:21
 ************************************************************************/

#include<iostream>
#include "icr_icscheck.hpp"
#include<gperftools/profiler.h>


int main()
{
    ProfilerStart("google.log");
	icscheck test;
    ProfilerStop();
	while(1);
	return 0;
}


