#pragma once
#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>

struct Any {
  Any(void) : m_type_id(std::type_index(typeid(void))) {}
  Any(const Any& that) : m_ptr(that.Clone()), m_type_id(that.m_type_id) {}
  Any(Any && that) : m_ptr(std::move(that.m_ptr)), m_type_id(that.m_type_id) {}

  //创建智能指针时 对于一般的类型通过std::decay来移除引用和cv符从而获取原始类型
  template<typename U, 
		   class = typename std::enable_if< 
											!std::is_same< typename std::decay<U>::type, Any >::value, 
											 U 
										  >::type
		  >  //U不是any类型
	Any(U && value) : 
	m_ptr(new Derived <typename std::decay<U>::type> (std::forward<U> (value))), //分配具体类型 初始化给基类指针
  m_type_id(std::type_index(typeid(typename std::decay<U>::type))){}

  bool is_null() const { return !bool(m_ptr); }
  template<class U> bool is() const { return m_type_id == std::type_index(typeid(U)); }

  //将Any转换为实际的类型
  template<class U> U& any_cast() {
	  if (!is<U>()) {
		std::cout << "can not cast " << typeid(U).name() << " to " << m_type_id.name() << std::endl;
		throw std::logic_error{"bad cast"};
	  }
	  auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
	  return derived->m_value;
	}

  Any& operator=(const Any& a) {
	std::cout<<"="<<std::endl;
	if (m_ptr == a.m_ptr) return *this;
	m_ptr = a.Clone();
	m_type_id = a.m_type_id;
	return *this;
  }

  private:
  struct Base;
  typedef std::unique_ptr<Base> base_ptr;

  struct Base { //关键点 基类没用模板
	virtual ~Base() {}
	virtual base_ptr Clone() const = 0;
  };

  template<typename T> struct Derived : Base {
	template<typename U> Derived(U && value) : m_value(std::forward<U>(value)) {}
	base_ptr Clone() const { return base_ptr(new Derived<T>(m_value)); }
	T m_value; //派生类中存放具体的值和类型
  };

  base_ptr Clone() const {
	if (m_ptr != nullptr) return m_ptr->Clone();
	return nullptr;
  }

  base_ptr m_ptr;
  std::type_index m_type_id;
};

