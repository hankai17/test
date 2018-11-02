typedef unsigned int uint_t; //即常用法

void func(unsigned int);
void func(uint_t); //将会与上函数重复

typedef最重要的是无法定义一个模板 
比如 
typedef std::map<std::string, int> map_int_t;
typedef std::map<std::string, string> map_string_t;
key都是string 而val是变化的

在c99中得这样写
template <typename val>
struct str_map {
	typedef std::map<std::string, val> type;
}
str_map<int>::type map1;
str_map<string>::type map2;

而c11中
template <typename val>
using str_map_t = std::map<std::string, val>;
str_map_t<int> map1;
str_map_t<string> map2;


上面的可以改为 using uint_t = unsigned int;

typedef不利于阅读
typedef void (*func_t) (int, string);
而c11 using func_t = void (*) (int, string)


