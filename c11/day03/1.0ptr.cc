为了防止资源泄露 请使用RAII对象 他们在构造函数中获得资源并在析构中释放资源
两个常备使用的RAII类是 shared_ptr和auto_ptr 前者最佳copy行为比较直观 若选择后者复制动作会使它指向null

//autoptr伪代码 
AutoPtr(AutoPtr<T>& ap)//拷贝构造函数  
        :_ptr(ap._ptr) {  
        ap._ptr = NULL;  
}  
  
AutoPtr<T>& operator=(AutoPtr<T>& ap)//赋值运算符重载 {  
    if (this != &ap)  {  
        if (_ptr)  {  
            delete _ptr;  
        }  
        _ptr = ap._ptr;  
        ap._ptr = NULL;  
    }  
    return *this;  
} 

~AutoPtr()//析构函数 {  
    if (_ptr)  {  
        delete _ptr;  
        _ptr = NULL;  
    }  
}
1 一块堆上的内存不能被两个智能指针同时指向(这个就是所有权和管理权的问题)  如果两个智能指针同时指向的话 析构时导致堆空间内存被释放两次
2 在auto_ptr的实现机制中 采用的是delete(调析构函数 然后再释放内存) 所以auto_ptr所管理的对象必须是new出来的 不能是malloc出来的 
3 auto_ptr不能管理数组 因为析构调用的是delete
4 较普通指针优点是:普通指针在new和delete之间发生异常 如果我不捕获异常 就不会执行delete 那么这片内存就不能被回收 这就是RAII

//这种将一块空间的权限完全交给别人的方法 不好
//假设调用了拷贝构造函数 我们的本意是用一个对象去构造一个与之一模一样的对象 可是结果我们把自己给弄丢了 完全没有达到我们预期的目标 如果我们想再次构造出来的对象的内容时(想想那个double print函数) 会发现对象的资源已经被清理了 所以会导致程序崩溃
//再如 sort的快排实现中将元素复制到某个局部临时对象中 但对于auto_ptr却将原元素置null 这就导致最后的排序结果中可能有大量的null
//资源转移通常只会在特定的场合发生(一般情况下都是拷贝) 例如转移一个临时变量(右值)给某个named variable（左值）或者一个函数的返回(右值) 
https://blog.csdn.net/qq_34992845/article/details/68939527  //里面分析有源码 没看懂

unique_ptr又叫scoped_ptr auto_ptr因为转移资源及转交权限从而引发一系列问题 根源就是拷贝构造和赋值运算符重载函数导致 这个智能指针就是禁止两者 本质上来说unique_ptr禁用了copy用move替代
实现起来也比较简单
private :  
    ScopedPtr(const ScopedPtr& ap);//拷贝构造函数声明  
    ScopedPtr& operator=(const ScopedPtr& ap);//赋值运算符重载函数声明  
使得对象所有函数外部均无法使用

基本用法:
    std::unique_ptr<A> up1; 
    up1.reset(new A(3)); //销毁内部对象并接受新对象的所有权(若缺省参数即没有任何对象所有权 此时仅将内部对象释放并置空)
    std::unique_ptr<A> up2(new A(4));

    A* p = up2.release(); //放弃内部对象的所有权 将内部指针置为空 返回内部对象的指针 此指针需要手动释放
    delete p;
/*
    unique_ptr<int> pInt(new int(5));
    unique_ptr<int> pInt2(pInt); //报错
    unique_ptr<int> pInt3 = pInt; //报错
*/

    std::unique_ptr<A> up3(new A(11));
    std::unique_ptr<A> up4 = std::move(up3); //unique_ptr虽然不支持拷贝、赋值操作 但提供移动机制来将指针的所有权从一个unique_ptr转移给另一个unique_ptr 如果需要转移所有权可以使用std::move()函数 注意此时up3已为空！！！
    up4 = nullptr;//显式销毁所指对象 同时智能指针变为空指针 与u_s2.reset()等价

    //可以拷贝或赋值一个将亡unique_ptr 
    unique_ptr func1(int a) {  
        return unique_ptr<int> (new int(a));  
    }  
//get 获得内部对象的指针 由于重载了()方法因此和直接使用对象是一样的 如unique_ptr<int> sp(new int(1)); sp与sp.get()是等价的
//swap 交换两个对象(即交换所拥有的对象)

//优点 在容器中保存指针 https://www.cnblogs.com/DswCnblog/p/5628195.html
    vector<unique_ptr<int>> vec;
    unique_ptr<int> p(new int(5));
    vec.push_back(std::move(p)); //使用移动语义

scoped_ptr的所有权是从一而终的 auto_ptr则允许用户以很自然的方式转交资源两者的设计理念都保证同一时刻所有权持有者唯一
auto_ptr的好处是显而易见的 比如用vector保存智能指针时由于大量元素操作涉及资源转移 scoped_ptr完全没法用 然而其设计上的最大问题在于让一切赋值行为变成了资源转交 这一点不符合最小惊讶原则 你很难一上来就接受原件被“拷贝”以后会失效这件事 所以C++11使用unique_ptr代替auto_ptr 除非在自身即将消亡的时候 资源的转交只能通过另一成员release显式完成。总而言之，auto_ptr最大的弊端在于允许很多几乎没有实际用处又不符合常规认知的行为 用scoped_ptr和unique_ptr可以利用静态检查早早地指出这些无意义操作的存在

https://stackoverflow.com/questions/5026197/what-c-smart-pointer-implementations-are-available

//插个题外话
A get_A() {
    return A();
}
int main() {
    //A a = get_A(); 默认调用两次拷贝构造 第一次是必须的拷贝成匿名对象 
    //A&& a = get_A(); 调用一次拷贝构造 第一次拷贝成匿名对象 然后直接转正
    return 0;
}
//返回一个元素必须得拷贝 这就有损性能 so便有了move
A(A&& a) :m_ptr(a.m_ptr) { //直接转移控制权限！！！                                          
    std::cout<<"move construct"<<std::endl;
    a.m_ptr = nullptr;                     
}                                          
