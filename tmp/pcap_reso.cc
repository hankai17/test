#include<iostream>
#include<stdio.h>
#include "pcapfile.h"
#include "string.h"
#include "mydef.h"

//curr_pos must in ETH before call this fun
int process_E_I_U(FILE* pFile, unsigned short* data_len) {
    //printf("=============================>\n");
    //0 Now at ETH_HEADER
    int cur_pos = (int)ftello(pFile);
    if(fseeko(pFile, cur_pos + 14 + 20 + 4, SEEK_SET) < 0)
        return -1;
    //1 Now at UDP len
    unsigned char udp_len_h;
    unsigned char udp_len_l;

    fread(&udp_len_h, sizeof(unsigned char), 1, pFile);
    fread(&udp_len_l, sizeof(unsigned char), 1, pFile);
    unsigned short udp_len = udp_len_h * 256 + udp_len_l;
    if(udp_len <= 0)
        return -1;
    //2 Out the true udp_data_len
    *data_len = udp_len - 8;
    printf("udp true data len: %d\n", *data_len); 

    //3 reset at udp_data
    if(fseeko(pFile, (int)ftello(pFile) + 2, SEEK_SET) < 0)
        return -1; //Now at UDP data
    //printf("<=============================\n");

    return 0;
}

int open_pcapfile(char* filepath, char* desc_error) {
        FILE * pFile = NULL;
        pFile = fopen(filepath, "r");
        if(!pFile) {
                strcpy(desc_error, "fopen err");
                return -1;
        }
        UINT16 file_head_len = sizeof(PCAPFILEHEAD);
        PCAPFILEHEAD pfileHead;

    fread((char*)&pfileHead, sizeof(PCAPFILEHEAD), 1, pFile);
    printf("pfileHead.magic: %x\n", pfileHead.magic);
    printf("pfileHead.majorVer: %x\n", pfileHead.majorVer);
    printf("pfileHead.minorVer: %x\n", pfileHead.minorVer);
    printf("--------------------> begin read \n");

    int offset = 0;
    if(fseeko(pFile, 40, SEEK_SET) < 0) {
        fclose(pFile);
        return -1; 
    }
    //Now at ETH_HEADER

    if(fseeko(pFile, 40 + 14 + 20 + 4, SEEK_SET) < 0) {
        fclose(pFile);
        return -1; //Now at UDP len
    }

    unsigned char udp_len_h;
    unsigned char udp_len_l;
    fread(&udp_len_h, sizeof(unsigned char), 1, pFile);
    fread(&udp_len_l, sizeof(unsigned char), 1, pFile);
    unsigned short udp_len = udp_len_h * 256 + udp_len_l;
    printf("udp_len: %d\n", udp_len);
    udp_len -= 8; //104 - 8 = 96

    if(fseeko(pFile, (int)ftello(pFile) + 2, SEEK_SET) < 0) {
        fclose(pFile);
        return -1; //Now at UDP data
    }

    int pcap_size = 1000000;
    unsigned char buf[1024 * 4]={0};
    int  blockSize = sizeof(buf);
    int writelen = 0;
    
    for(;;) {
        if(udp_len > 0) {
            memset(buf, 0x0, 1024*4);
            //0 read true udp data len
            int ret = fread( buf, udp_len, 1, pFile);

            //1 now we got udp data buf. we can process udp data


            //2 skip pcap_header
            if(fseeko(pFile, (int)ftello(pFile) + 16, SEEK_SET) < 0) {
                break;
            }

            //3 because of 2, now we at ETH_HEADER
            if(process_E_I_U(pFile, &udp_len) < 0)
                break;
        }
    }

    fclose(pFile);
    return 0;
}

int main() {
    char desc_err[1024] = {0};
    int ret = open_pcapfile((char*)"./1.pcap", desc_err);
    if(ret < 0) {
        printf("open_pcapfile err: %s\n", desc_err);
        return 0;
    }
    return 0;
}


