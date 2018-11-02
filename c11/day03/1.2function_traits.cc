int func(int a,string b);

function_traits<decltype(func)>::function_type; //获取函数类型 int _cdecl(int, string) 这个有点类似于typeid().name()

function_traits<decltype(func)>::return_type; //int

function_traits<decltype(func)>::arity; //2

function_traits<decltype(func)>::arg_type<0>; // 获取函数第一个参数类型 int
