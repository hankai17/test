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

/*将str1字符串中第一次出现的str2字符串替换成str3*/  
void replaceFirst(char *str1, char *str2, char *str3) {  
    char str4[strlen(str1) + 1];  
    char *p;  
    strcpy(str4, str1);  
    if ((p = strstr(str1, str2)) != NULL)/*p指向str2在str1中第一次出现的位置*/ {  
        while (str1 != p && str1 != NULL)/*将str1指针移动到p的位置*/ {  
            str1++;  
        }  
        str1[0] = '/0'; /*将str1指针指向的值变成/0,以此来截断str1,舍弃str2及以后的内容，只保留str2以前的内容*/  
        strcat(str1, str3); /*在str1后拼接上str3,组成新str1*/  
        strcat(str1, strstr(str4, str2) + strlen(str2)); /*strstr(str4,str2)是指向str2及以后的内容(包括str2),strstr(str4,str2)+strlen(str2)就是将指针向前移动strlen(str2)位，跳过str2*/  
    }  
}  

/*将str1出现的所有的str2都替换为str3*/  
void replace(char *str1, char *str2, char *str3) {  
    while (strstr(str1, str2) != NULL) {  
        replaceFirst(str1, str2, str3);  
    }  
}  

/*截取src字符串中,从下标为start开始到end-1(end前面)的字符串保存在dest中(下标从0开始)*/  
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

/*返回src中下标为index的字符*/  
char charAt(char *src, int index) {  
    char *p = src;  
    int i = 0;  
    if (index < 0 || index > strlen(src))  
        return 0;  
    while (i < index)i++;  
    return p[i];  
}  

/*返回str2第一次出现在str1中的位置(下表索引),不存在返回-1*/  
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

/*返回str1中最后一次出现str2的位置(下标),不存在返回-1*/  
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

/*删除str左边第一个非空白字符前面的空白字符(空格符和横向制表符)*/  
void ltrim(char *str) {  
    int i = 0, j, len = strlen(str);  
    while (str[i] != '/0') {  
        if (str[i] != 32 && str[i] != 9)break; /*32:空格,9:横向制表符*/  
        i++;  
    }  
    if (i != 0)  
        for (j = 0; j <= len - i; j++) {  
            str[j] = str[j + i]; /*将后面的字符顺势前移,补充删掉的空白位置*/  
        }  
}  

/*删除str最后一个非空白字符后面的所有空白字符(空格符和横向制表符)*/  
void rtrim(char *str) {  
    char *p = str;  
    int i = strlen(str) - 1;  
    while (i >= 0) {  
        if (p[i] != 32 && p[i] != 9)break;  
        i--;  
    }  
    str[++i] = '/0';  
}  

/*删除str两端的空白字符*/  
void trim(char *str) {  
    ltrim(str);  
    rtrim(str);  
}  

//这是libcurl接收数据的回调函数，相当于recv的死循环  
//其中stream可以自定义数据类型，这里我传入的是文件保存路径  

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
//加上-lcurl库  

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
        //指定回调函数  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  
        //这个变量可作为接收或传递数据的作用  
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
                //打开字符集转换    
                iconv_t hIconv = iconv_open(tocode,"iso-8859-1");  
                if (-1 == (int) hIconv) {  
                    return -1; //打开失败，可能不支持的字符集    
                }       
                printf("%s\n",src_html);  
                //开始转换    
                iRet = iconv(hIconv, (char**) (&src_html), strlen(src_html), (char**) (&output_html), strlen(src_html));                  
                printf("%s\n", output_html);    
                printf("ok");  
                if(strcmp(output_html,"")==0)  
                {  
                    printf("%s\n",src_html);  
                }  
                //关闭字符集转换    
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
