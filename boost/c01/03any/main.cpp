#include <iostream>
#include <vector>
#include <string>
#include <auto_ptr.h>
/*
int main()
{

    typedef std::auto_ptr<Object> object_ptr;
    std::vector<object_ptr> some_values;
    some_values.push_back(new Object(10));
    some_values.push_back(new Object("Hello there"));
    some_values.push_back(new Object(std::string("Wow!")));

    std::string* p = dynamic_cast<std::string*>(some_values.back().get());
    assert(p);
    (*p) += " That is great!\n";
    std::cout << *p;

    return 0;
}
//:1,21s#^#//#g
//:1,21s#^//##g
*/
#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <string>
void example_func();

int main()
{
    std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char* c_str = "Hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow!"));
    std::cout<<some_values.empty()<<std::endl;

    std::string& s = boost::any_cast<std::string&>(some_values.back());
    //std::string s = boost::any_cast<std::string>(some_values.back());
    s += " That is great!\n";
    std::cout << s;

    std::string s1 = boost::any_cast<std::string>(some_values.back());
    s1 += "hehe";
    std::cout<<s1<<std::endl;

    std::string s2 = boost::any_cast<std::string>(some_values.back());
    std::cout<<s2<<std::endl;
    
    std::cout<<"------------------------"<<std::endl;
    //example_func(); 
    std::string s3 = boost::any_cast<std::string>(some_values[2]);
    std::cout<<s3<<std::endl;

    std::string* s4 = boost::any_cast<std::string>(&some_values[2]);
    std::cout<<s4<<std::endl;

    return 0;
}

void example_func() {
    boost::any variable(std::string("Hello world!"));

    // Following method may throw a boost::bad_any_cast exception
    // if actual value in variable is not a std::string
    std::string s1 = boost::any_cast<std::string>(variable); //<std::string> 即取什么 取的是string
    std::cout<<s1<<std::endl;

    // If actual value in variable is not a std::string
    // will return an NULL pointer
    std::string* s2 = boost::any_cast<std::string>(&variable);
    std::cout<<s2<<std::endl;

    (void)s2; // Supressing warnings about unused variable
}
