#include <stdio.h>  
#include <curl/curl.h>  
#include <string.h>  
#include <stdlib.h>  
#include <ctype.h>  
#include <iconv.h>  
#include <unistd.h>  

#define TMP_FILE "tmp.html"  
#define HTML_BUFFER_SIZE 1024*800  

void split(char **arr, char *str, const char *del) {  
    char *s = strtok(str, del);  
    while (s != NULL) {  
        *arr++ = s;  
        s = strtok(NULL, del);  
    }  
}  

/*��str1�ַ����е�һ�γ��ֵ�str2�ַ����滻��str3*/  
void replaceFirst(char *str1, char *str2, char *str3) {  
    char str4[strlen(str1) + 1];  
    char *p;  
    strcpy(str4, str1);  
    if ((p = strstr(str1, str2)) != NULL)/*pָ��str2��str1�е�һ�γ��ֵ�λ��*/ {  
        while (str1 != p && str1 != NULL)/*��str1ָ���ƶ���p��λ��*/ {  
            str1++;  
        }  
        str1[0] = '/0'; /*��str1ָ��ָ���ֵ���/0,�Դ����ض�str1,����str2���Ժ�����ݣ�ֻ����str2��ǰ������*/  
        strcat(str1, str3); /*��str1��ƴ����str3,�����str1*/  
        strcat(str1, strstr(str4, str2) + strlen(str2)); /*strstr(str4,str2)��ָ��str2���Ժ������(����str2),strstr(str4,str2)+strlen(str2)���ǽ�ָ����ǰ�ƶ�strlen(str2)λ������str2*/  
    }  
}  

/*��str1���ֵ����е�str2���滻Ϊstr3*/  
void replace(char *str1, char *str2, char *str3) {  
    while (strstr(str1, str2) != NULL) {  
        replaceFirst(str1, str2, str3);  
    }  
}  

/*��ȡsrc�ַ�����,���±�Ϊstart��ʼ��end-1(endǰ��)���ַ���������dest��(�±��0��ʼ)*/  
void substring(char *dest, char *src, int start, int end) {  
    char *p = src;  
    int i = start;  
    if (start > strlen(src))return;  
    if (end > strlen(src))  
        end = strlen(src);  
    while (i < end) {  
        dest[i - start] = src[i];  
        i++;  
    }  
    dest[i - start] = '/0';  
    return;  
}  

/*����src���±�Ϊindex���ַ�*/  
char charAt(char *src, int index) {  
    char *p = src;  
    int i = 0;  
    if (index < 0 || index > strlen(src))  
        return 0;  
    while (i < index)i++;  
    return p[i];  
}  

/*����str2��һ�γ�����str1�е�λ��(�±�����),�����ڷ���-1*/  
int indexOf(char *str1, char *str2) {  
    char *p = str1;  
    int i = 0;  
    p = strstr(str1, str2);  
    if (p == NULL)  
        return -1;  
    else {  
        while (str1 != p) {  
            str1++;  
            i++;  
        }  
    }  
    return i;  
}  

/*����str1�����һ�γ���str2��λ��(�±�),�����ڷ���-1*/  
int lastIndexOf(char *str1, char *str2) {  
    char *p = str1;  
    int i = 0, len = strlen(str2);  
    p = strstr(str1, str2);  
    if (p == NULL)return -1;  
    while (p != NULL) {  
        for (; str1 != p; str1++)i++;  
        p = p + len;  
        p = strstr(p, str2);  
    }  
    return i;  
}  

/*ɾ��str��ߵ�һ���ǿհ��ַ�ǰ��Ŀհ��ַ�(�ո���ͺ����Ʊ��)*/  
void ltrim(char *str) {  
    int i = 0, j, len = strlen(str);  
    while (str[i] != '/0') {  
        if (str[i] != 32 && str[i] != 9)break; /*32:�ո�,9:�����Ʊ��*/  
        i++;  
    }  
    if (i != 0)  
        for (j = 0; j <= len - i; j++) {  
            str[j] = str[j + i]; /*��������ַ�˳��ǰ��,����ɾ���Ŀհ�λ��*/  
        }  
}  

/*ɾ��str���һ���ǿհ��ַ���������пհ��ַ�(�ո���ͺ����Ʊ��)*/  
void rtrim(char *str) {  
    char *p = str;  
    int i = strlen(str) - 1;  
    while (i >= 0) {  
        if (p[i] != 32 && p[i] != 9)break;  
        i--;  
    }  
    str[++i] = '/0';  
}  

/*ɾ��str���˵Ŀհ��ַ�*/  
void trim(char *str) {  
    ltrim(str);  
    rtrim(str);  
}  

//����libcurl�������ݵĻص��������൱��recv����ѭ��  
//����stream�����Զ����������ͣ������Ҵ�������ļ�����·��  

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {  
    int len = size * nmemb;  
    int written = len;  
    FILE *fp = NULL;  
    if (access((char*) stream, 0) == -1) {  
        fp = fopen((char*) stream, "wb");  
    } else {  
        fp = fopen((char*) stream, "ab");  
    }  
    if (fp) {  
        fwrite(ptr, size, nmemb, fp);  
    }  
    // printf("%s\n",ptr);  
    fclose(fp);  
    return written;  
}  
//����-lcurl��  

void test_post(char* url,char* data) {  
    CURL *curl;  
    CURLcode res;  
    curl = curl_easy_init();  
    if (curl) {  
        //www.baidu.com/#wd=java  
        curl_easy_setopt(curl, CURLOPT_URL, url);  
        curl_easy_setopt(curl, CURLOPT_POST, 1L);  
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);  
        res = curl_easy_perform(curl);  
        curl_easy_cleanup(curl);  
    }  
}  

int file_exists(char *filename) {  
    return (access(filename, 0) == 0);  
}  
int GetCharset(char *src_html,char *charCode) {  
    char tmp_html[HTML_BUFFER_SIZE]={0};  
    int pos = indexOf(src_html, "text/html; charset=");  
    if (pos > 0) {  
        strncpy(tmp_html, src_html + pos + strlen("text/html; charset="), strlen(src_html) - pos);  
        pos = indexOf(tmp_html, "\"");  
        if (pos > 0) {  
            strncpy(charCode, tmp_html, pos);  
        }  
    }  
    return 0;  

}  


void test_get(char* url) {  
    CURL *curl;  
    CURLcode res;  
    curl = curl_easy_init();  
    if (curl) {  
        if (file_exists(TMP_FILE))  
            remove(TMP_FILE);  
        curl_easy_setopt(curl, CURLOPT_URL, url);  
        //ָ���ص�����  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  
        //�����������Ϊ���ջ򴫵����ݵ�����  
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, TMP_FILE);  
        res = curl_easy_perform(curl);  
        char tocode[64] = "UTF-8";  
        if (CURLE_OK == res) {  
            char *ct;  
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);  
            if ((CURLE_OK == res) && ct)  
                printf("We received Content-Type: %s\n", ct);  
            //printf("====\n");  
            //int pos=strcspn(ct,"UTF-8");   
            int index = indexOf(ct, "=");  
            char* arr[3];  
            if (index > 0) {  
                split(arr, ct, "=");  
                //printf("%s\n", arr[1]);  
                strcpy(tocode, arr[1]);  
            }  
            FILE *fp = NULL;  
            fp = fopen(TMP_FILE, "r");  
            char src_html[HTML_BUFFER_SIZE]={0};  
            char output_html[HTML_BUFFER_SIZE]={0};  
            char tmp_html[HTML_BUFFER_SIZE]={0};  
            if (fp) {  
                fread(src_html, HTML_BUFFER_SIZE, 1, fp);     
                strcpy(tmp_html,src_html);  
                if(index <0) {  
                    GetCharset(tmp_html,tocode);  
                    printf("%s\n",tocode);  
                }  
                int iRet;  
                //���ַ���ת��    
                iconv_t hIconv = iconv_open(tocode,"iso-8859-1");  
                if (-1 == (int) hIconv) {  
                    return -1; //��ʧ�ܣ����ܲ�֧�ֵ��ַ���    
                }       
                printf("%s\n",src_html);  
                //��ʼת��    
                iRet = iconv(hIconv, (char**) (&src_html), strlen(src_html), (char**) (&output_html), strlen(src_html));                  
                printf("%s\n", output_html);    
                printf("ok");  
                if(strcmp(output_html,"")==0)  
                {  
                    printf("%s\n",src_html);  
                }  
                //�ر��ַ���ת��    
                iconv_close(hIconv);  

            }  

        }  
        curl_easy_cleanup(curl);  
    }  
}  

int main(int argc, char* argv) {  
    //printf("%s\n",argv[1]);  
    //http://192.168.1.6:8080/TestServer/index.html?fdasf=123456&af=89  
    //test_post("http://192.168.1.6:8080/TestServer/index.html","wd=hava&hehe=123456");  
    //test_get("http://www.ip.cn/index.php?ip=202.108.249.253");  
    char url[] = "http://www.ip138.com/ips138.asp?ip=202.108.249.253&action=2";
    test_get(url);  
    printf("\nok");  
    return 0;  
}
