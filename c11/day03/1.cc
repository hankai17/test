#include <iostream>
#include <type_traits>
#include <memory>

template <class T> 
struct Construct {
  typedef typename std::remove_reference<T>::type U; //移除可能的引用
  Construct() : m_ptr(new U){} //创建对象时 需要原始类型

  typename std::add_lvalue_reference<U>::type get_val() const {
	*m_ptr = 100;  //特意加的一行，以防*m_ptr是个未定义的值
	return *m_ptr.get();
  }

  private:
  std::unique_ptr<U> m_ptr;
};

int main(void)
{
  Construct<int> c;
  std::string a = c.get_val();
  std::cout <<a<< std::endl;
  return 0;
}
