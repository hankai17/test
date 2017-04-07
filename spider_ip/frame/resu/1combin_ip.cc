#include<iostream>

int main()
{
    int ip[256] = {0};
    for(int j=0; j<256; j++)
    {
        ip[j] = j;
    }
    for(int i=1; i<256; i++)
    {
        for(int ii=0; ii<256; ii++)
        {
            for(int iii=0; iii<256; iii++)
            {
                for(int iiii=0; iiii<256; iiii++)
                {
                    std::cout<<i<<" "<<ii<<" "<<iii<<" "<<iiii<<" "<<std::endl;
                }
            }
        }
    }


    return 0;
}
