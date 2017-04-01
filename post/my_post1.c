#include <curl/curl.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

/**
 * This struct is for memory store
 * @brief The MemoryStruct struct
 */
struct MemoryStruct{
    char* memory;
    size_t size;
};

/**
 * Esta funci¨®n es para el proceso de recepci¨®n de datos recibidos
 * @brief WriteMemoryCallback
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size*nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct *)userp;
    mem->memory =(char*) realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* Sin memoria! */
        printf("No hay memoria suficiente memoria \n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

/**
 * This function is for the POST process
 * @brief sendHTTP
 * @param data
 * @param url
 * @return
 */
static string sendHTTP(string data,string url){
    CURL *curl;
    CURLcode res;
    struct curl_slist* headers=NULL;
    struct MemoryStruct chunk; //Stores the received data
    chunk.memory = (char*) malloc(1);
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    headers = curl_slist_append(headers, "Content-Type: application/json"); //If you send another type change this
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //URL to connect
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); //Some servers need this
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str()); //Data to send
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length()); //Size of the data to send
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //Headers
    curl_easy_setopt(curl, CURLOPT_POST, 1L); //Especicate the HTTP function to make
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); //Put the readback function to use
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); //Passes the variable that stores memory
    res = curl_easy_perform(curl); //Execute the process
    curl_slist_free_all(headers); //Release memory
    if(res != CURLE_OK) { // Verify any error
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    else{
        //printf("%lu bytes received\n", (long)chunk.size); //To print the amount of bytes received
    }
    curl_easy_cleanup(curl); //ALWAYS cleanup
    string datoOut(chunk.memory);
    free(chunk.memory); //Release memory
    return datoOut;
}

int main(int argc, char *argv[]){
    //string data = "PUT YOUR JSON DATA TO SEND IN POST PROCESS";
    string data = "";
    string url="www.baidu.com";
    cout<<sendHTTP(data,url)<<endl;
    return 0;
}
