#include "c_search.hh"

int main(int argc, char* argv[])
{
    std::ifstream f;
    char* buf;
    int file_size;

    if(init_file(f,&buf,&file_size))
    {
        std::cout<<"init_file err\n";
        return 0;
    }

    std::string result;
    std::string ip { "1.1.1.1" };
    if(search_ip(buf, file_size, ip, result))
    {
        std::cout<<"search_ip err\n";
        return 0;
    }
    std::cout<<result<<std::endl;

    close_file(f, buf);

    return 0;
}
