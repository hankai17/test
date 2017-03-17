#include<iostream>

int main1() //linux 下 中文转utf8
{
    char chara[] = { "中国" };
    std::cout<<"chara="<<chara<<std::endl;
    printf("%s\n",chara);
    return 0;
}

int main()
{
    wchar_t chara[] = L"中国 "; //转为 unicode码
    std::cout<<"chara="<<chara<<std::endl;
    std::wcout<<"chara="<<chara<<std::endl;
    wprintf(L"%ls",chara);

    return 0;
}
