#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

//protocol char struct
struct PROTOCOL_CHARS
{
	const char _http[5] = "http";
	const char _https[6] = "https";
};

//uri chars struct
struct URI_CHARS
{
	const char _colon = ':';
	const char _fslash = '/';
	const char _dot = '.';
};

//domain chars struct
struct DOMAIN_CHARS
{
	const char _suffix_com[4] = "com";
	const char _suffix_co[3] = "co";
	const char _suffix_org[4] = "org";
};

//get protocol from uri
void get_protocol(char uri[]){
	unsigned int len = strlen(uri);
	PROTOCOL_CHARS p_chars;
	bool secure;
	if(len > 0){
		for(int i = 0; i < strlen(p_chars._https); i++){
			if(uri[i] == p_chars._http[i]){
				secure = false;
			}else if(uri[i] == p_chars._https[i]){
				secure = true;
			}
		}
		if(secure){
			cout << "This protocol is secure \n" << endl;
		}else{
			cout << "This protocol is standard \n" << endl;
		}
	}
}

//helper function
int d_position(string uri){
	DOMAIN_CHARS d_chars;
	int pos;
	if((pos = uri.find(d_chars._suffix_co)) != -1){
		return pos;
	}else if((pos = uri.find(d_chars._suffix_com)) != -1){
		return pos;
	}else if((pos = uri.find(d_chars._suffix_org)) != -1){
		return pos;
	}
	return (pos = -1);
}

//get domain from uri
void get_domain(char uri[]){
	unsigned int len = strlen(uri);
	URI_CHARS u_chars;
	DOMAIN_CHARS d_chars;
	string uristr(uri);
	if(len > 0){
		int fslashp = uristr.find(u_chars._fslash);
		int strpos = uristr.find(u_chars._fslash, fslashp + 1) + 1;
		int endpos = d_position(uristr);
		if(endpos != -1){
			string result = uristr.substr(strpos, endpos);
			/*if(result.find(u_chars._fslash) != -1){
				cout << uristr.substr(strpos, endpos - 2) << endl;
			}else{
				cout << uristr.substr(strpos, endpos) << endl;
			}*/
			int pos;
			string result2;
			for(int i = 0; i < result.length(); i++){
				if((pos = result.find(u_chars._fslash)) != -1){
					result2 = result.substr(0, pos);
				}else{
					result2 = result;
				}
			}
			cout << "Domain: " << result2 << endl;
		}
	}
}

int char_count(string s, char c){
	int count = 0;
	for(int i = 0; i < s.size(); i++){
		if(s[i] == c) count++;
	}
	return count;
}

void find_endpoints(string s, char c){
	char arr[s.size()];
	strcpy(arr, s.c_str());
	char * a_ocurs = strchr(arr, c);
	while(a_ocurs != NULL){
		unsigned int idx = a_ocurs-arr+1;
		cout << "Endpoint: " << s.substr(idx, idx + 1) << endl;
		a_ocurs = strchr(a_ocurs + 1, c);
	}
}

void get_endpoints(char uri[]){
	unsigned int len = strlen(uri);
	URI_CHARS u_chars;
	string uristr(uri);
	if(len > 0){
		int endpoint_fslashp = uristr.find(u_chars._fslash);
		int strpos = uristr.find(u_chars._fslash, endpoint_fslashp + 2);
		if(strpos != -1){
			//cout << "Endpoint: " << char_count(uristr.substr(strpos, len), u_chars._fslash) << endl;
			find_endpoints(uristr.substr(strpos, len), u_chars._fslash);
		}
	} 
}

//main run function
int main(int argc, char* argv[]) {
	if(argc > 0){
		for(int i = 1; i < argc; i++){
			get_protocol(argv[i]);
			get_domain(argv[i]);
			get_endpoints(argv[i]);
		}
	}
	return 0;
}
