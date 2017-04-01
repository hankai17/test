#include<iostream>
#include<string.h>
#include<string>

std::string revise_result(std::string& str)
{
    if(str.data() == nullptr)
    {
        return "NULL";
    }
    int is_blank = 0;
    int str_num = 1;
    char* p = (char*)str.data();
    while(*p != '\0')
    {
        if(*p == ' ')
        {
            is_blank = 1;
            p++;
            continue;
        }
        else
        {
            p++; 
            if(is_blank)
            {
                str_num++; 
                is_blank = 0;
            }
        }
    }
    //std::cout<<"str_num="<<str_num<<std::endl;
    char data[1024] = {0};
    if(str_num == 1)
    {
        char* d1 = nullptr;
        char* tmp;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                *tmp = '\0';
                tmp++;
            }
            else
            {
                tmp++;
            }
        }
        std::string res1 = {d1};
        std::string res = res1 + " " + "NULL"; 
        return res;
    }
    if(str_num == 2)
    {
        char* d1 = nullptr;
        char* d2 = nullptr;
        char* tmp;
        int is_blank = 0;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                is_blank = 1;
                *tmp = '\0';
                tmp++;
                continue;
            }
            else
            {
                tmp++;
                if(is_blank)
                {
                    d2 = tmp - 1;
                    is_blank = 0;
                }
            }
        }
        std::string res1 = {d1};
        std::string res2 = {d2};
        std::string res3 = res1 + " " + res2;
        return res3;
    }
    if(str_num == 3)
    {
        char* d1 = nullptr;
        char* d2 = nullptr;
        char* d3 = nullptr;
        char* tmp;
        int is_blank = 0;
        int str_num = 1;
        int in_flag1 = 0;
        int in_flag2 = 0;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                is_blank = 1;
                *tmp = '\0';
                tmp++;
                continue;
            }
            else
            {
                if(is_blank)
                {
                    //str_num++; 
                    ++str_num; 
                    is_blank = 0;
                }
                tmp++; 
            }
            if(str_num == 2 && in_flag1 == 0)
            {
                d2 = tmp - 1; 
                in_flag1 = 1;
            }
            if(str_num == 3 && in_flag2 == 0)
            {
                d3 = tmp - 1; 
                in_flag2 = 1;
            }
        }
        std::string s1 = {d1};
        std::string s2 = {d2};
        std::string s3 = {d3};

        std::string res = s1 + s2 + " " + s3;
        return res;
    }
    return "NULL";
}

int main()
{
    std::string res = "山西省太原市  全省共用出口     电信";
    //std::string res = "山西省太原市  电信";
    std::string res1 = "印度 ";

    std::cout<<revise_result(res)<<std::endl;;
    //std::cout<<revise_result(res1)<<std::endl;;


    return 0;
}
