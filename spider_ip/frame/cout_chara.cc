#include<iostream>

int main1() //linux �� ����תutf8
{
    char chara[] = { "�й�" };
    std::cout<<"chara="<<chara<<std::endl;
    printf("%s\n",chara);
    return 0;
}

int main()
{
    wchar_t chara[] = L"�й� "; //תΪ unicode��
    std::cout<<"chara="<<chara<<std::endl;
    std::wcout<<"chara="<<chara<<std::endl;
    wprintf(L"%ls",chara);

    return 0;
}
