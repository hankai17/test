template<typename Type>
struct get_size {
	static const int value = 1; //老版本定义编译器常量
};

template<typename Type>
struct get_size1 {
	enum{ value = 1 }; //也可以这样 定义一个编译期常量 更简洁
};

c11中定义编译期常量写法更简洁
template<typename Type>
struct get_size2 : std::integral_constant<int, 1> {}; //即从integral_constant派生即可 这种写法不需额外的变量

实现:
template<class T, T v> 
struct integral_constant {
	static const T value = v;
	typedef T value_type;
	typedef intergral_constant<T, v> type;
	operator value_type() { return value; }
}


