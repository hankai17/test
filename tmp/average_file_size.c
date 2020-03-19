#include<stdio.h>
#include<stdint.h>
#include<string.h>

typedef struct _white_item         
{                                  
        char opt;                  
        char type; // 1 , 2 m3u8   
        char is_hot;               
        int hit_count;             
        unsigned int ok_timer;     
        short siteid;              
        int64_t dat_size;          
        char home;                 
        unsigned int expire;       
        unsigned int last_modified;
        unsigned char infohash[20];
}TS_WHITE_ITEM;                    

int main(int argc, char** argv) {
        if (argc < 2) {
                printf("Use:\n");
                printf("        ./a.out ts_white_x\n");
                return 0;
        }
        char file_name[64] = {0};
        int cnt = 0;
        FILE * fp;
        unsigned char buf[4*1024];
        TS_WHITE_ITEM * ts_white_elm; 
        int i = 0;
        int file_num = 0;
        uint64_t sum_flow = 0;
        strcpy(file_name, argv[1]);
        printf("file_name: %s\n", file_name);

        if(NULL == (fp = fopen(file_name, "r"))) {
                printf("can not open file: %s\n", file_name);
                return 0;
        } 


        while((cnt = fread(buf, sizeof(TS_WHITE_ITEM), (4*1024/sizeof(TS_WHITE_ITEM)) ,fp))) {
                ts_white_elm = (TS_WHITE_ITEM *)buf;  

                for(i=0;i<cnt;i++,ts_white_elm++) {
                  file_num++;
                  sum_flow += ts_white_elm->dat_size;
                  //printf("cnt: %d\n", cnt);
                  //printf("date_size: %d\n", ts_white_elm->dat_size);

                }
                memset(buf, 0x0, 4*1024);

        }
        //printf("fread count: %d\n", (4*1024/sizeof(TS_WHITE_ITEM)));
        printf("file count: %d sum flow: %ld, average: %d\n", file_num, sum_flow, sum_flow/file_num);
        fclose(fp);
        return 0;
}
