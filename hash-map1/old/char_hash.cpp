//#include <map>
#include <ext/hash_map>
#include <string>
#include <iostream>

#include <string.h>

typedef struct {
	size_t len;
	char   *data;
}icr_str_t;
#define icr_string(str) {sizeof(str) - 1, (char *)str}

struct m_strcmp {
	bool operator()(const char *p1, const char *p2) const {
		return strcasecmp(p1, p2) == 0;
	}
};
typedef __gnu_cxx::hash_map<char *, icr_str_t *, __gnu_cxx::hash<char *>, m_strcmp>::iterator hm_itor;
int m_map() {
	icr_str_t sfx1 = icr_string(".mp4");
	icr_str_t sfx2 = icr_string(".mp3");
	icr_str_t sfx3 = icr_string(".flv");
	icr_str_t sfx4 = icr_string(".f4vl");
	std::cout << sfx1.data << std::endl;
	std::cout << sfx2.data << std::endl;
	std::cout << sfx3.data << std::endl;
	std::cout << sfx4.data << std::endl;

	__gnu_cxx::hash_map<char *, icr_str_t *, __gnu_cxx::hash<char *>, m_strcmp> sfx_map;
	sfx_map.insert(std::pair<char*, icr_str_t*>(sfx1.data, &sfx1));
	sfx_map.insert(std::pair<char*, icr_str_t*>(sfx2.data, &sfx2));
	sfx_map.insert(std::pair<char*, icr_str_t*>(sfx3.data, &sfx3));
	sfx_map.insert(std::pair<char*, icr_str_t*>(sfx4.data, &sfx4));
	//sfx_map[".mp4"] = sfx1;
	//
	hm_itor itr;
	int i = 0;
	for( i = 0; i < 1000000; i++) {
		if((itr = sfx_map.find(sfx1.data)) != sfx_map.end()) {
			//std::cout << "find : " << itr->second->data << std::endl;
			//std::exit(1);
		}
	}
	return 0;
}

int main()
{
	m_map();
	return 0;
}
