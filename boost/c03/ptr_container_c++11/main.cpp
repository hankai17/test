#include <set>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <cassert>

template <class T>
struct ptr_cmp: public std::binary_function<T, T, bool> { //仿函数即也叫函数对象
    template <class T1>
    bool operator()(const T1& v1, const T1& v2) const { //模板模板 怎么调 好高级
        return operator ()(*v1, *v2);
    }

    bool operator()(const T& v1, const T& v2) const {
        return std::less<T>()(v1, v2);
    }
};

template <class T>
struct ptr_deleter: public std::unary_function<T, void> {
    void operator()(T* ptr) {
        delete ptr;
    }
};

void example1() {

    std::set<int*, ptr_cmp<int> > s;
    s.insert(new int(1));
    s.insert(new int(0));

    // ...
    assert(**s.begin() == 0);

    // ...
    // Deallocating resources
    // Any exception in this code will lead to
    // memory leak
    //std::for_each(s.begin(), s.end(), ptr_deleter<int>()); //跟下面一个意思  都是遍历删除  有可能内存泄漏
    std::for_each(s.begin(), s.end(), 
                  boost::bing(::operator delete, _1)
                 );
}

//void example2_a() {
//    typedef std::auto_ptr<int> int_aptr_t;
//    std::set<int_aptr_t, ptr_cmp<int> > s;
//    s.insert(int_aptr_t(new int(1)));
//    s.insert(int_aptr_t(new int(0)));
//    // ...
//    assert(**s.begin() == 0);
//    // ...
//    // resources will be deallocated by auto_ptr<>
//}

void example2_b() {
    typedef std::unique_ptr<int> int_uptr_t; //看到typedef了吧 下面就很可能用到vec之类的容器
    std::set<int_uptr_t, ptr_cmp<int> > s;
    s.insert(int_uptr_t(new int(1)));
    s.insert(int_uptr_t(new int(0)));
    // ...
    assert(**s.begin() == 0); //看不懂
    // ...
    // resources will be deallocated by unique_ptr<>
}

#include <boost/shared_ptr.hpp> //移植性更好 适用于多种编译器  
void example3() {
    typedef boost::shared_ptr<int> int_sptr_t;
    std::set<int_sptr_t, ptr_cmp<int> > s;
    s.insert(int_sptr_t(new int(1)));
    s.insert(int_sptr_t(new int(0)));
    // ...
    assert(**s.begin() == 0);
    // ...
    // resources will be deallocated by shared_ptr<>
}

#include <boost/ptr_container/ptr_set.hpp>
void correct_impl() {
    boost::ptr_set<int> s; //省的搞仿函数了  
    s.insert(new int(1));
    s.insert(new int(0));
    // ...
    assert(*s.begin() == 0);
    // ...
    // resources will be deallocated by container itself
}

#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
void theres_more_example() {
    // Creating vector of 10 elements with values 100
    boost::ptr_vector<int> v;
    v.resize(10, new int(100));
    assert(v.size() == 10);
    assert(v.back() == 100);
}

int main() {
    example1();
//    example2_a();
    example2_b();
    example3();
    correct_impl();
    theres_more_example();
}
