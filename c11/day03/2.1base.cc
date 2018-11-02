//常见的可变参数模板
template<class... T>  //...即声明一个参数包 包中可以包含任意个模板参数
void f(T... args) {
	cout<< sizeof... (args) <<endl;
}

f(); //0
f(1, 2); //2
f(1, 2.5, ""); //3

如果要用参数包中的参数 即展开参数包 
一种方法是通过递归模板函数展开包
//首先得提供展开函数+递归终止函数
void print() { //终止函数
	cout<<"empty"<<endl;
}
template<class T, class... Args> //参数包Args...在展开过程中递归的调用自己 每调一次参数少一个
void print(T head, Args... rest) {
	cout<<"para" << head <<endl;
	print(rest...);
}

int main() {
	print(1, 2, 3, 4);
}


一种是通过逗号表达式展开包
//递归好理解 缺点是得写一个重载的递归终止函数
...... 
看不下去了

