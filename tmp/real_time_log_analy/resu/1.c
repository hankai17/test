#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned short          uint16_t;  

struct Item {
    uint16_t id;   
    uint16_t type;   
    uint16_t length;   
    uint16_t domain_len;
    char* p;
};

int main() {
    FILE* fd = fopen("./test_struct", "rb");
    char buf[1024] = {0};
    //int ret = fread(buf, 1, sizeof(1024), fd);

    fseek (fd, 0, SEEK_END);
    int size = ftell(fd);
    fseek (fd, 0, 0);

    int ret = fread(buf, 1, size, fd);
    printf("fread ret: %d\n", ret);

    printf("buf: %s\n", buf);

    struct Item* i = (struct Item*)buf;
    printf("i.id: %d\n", i->id);
    printf("i.type: %d\n", i->type);
    printf("i.len: %d\n", i->length);
    printf("i.domain_len: %d\n", i->domain_len);
    printf("i.p: %c\n", i->p);

    fclose(fd);
    return 0;
}
