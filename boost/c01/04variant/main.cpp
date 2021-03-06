#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>

void example_func(); 
void example_func1(); 
int main()
{
    typedef boost::variant<int, const char*, std::string> my_var_t;
    std::vector<my_var_t> some_values;
    some_values.push_back(10);
    some_values.push_back("Hello there!");
    some_values.push_back(std::string("Wow!"));

    std::string& s = boost::get<std::string>(some_values.back());
    s += " That is great!\n";
    std::cout << s;
     example_func(); 
     example_func1(); 
    return 0;
}



void example_func() {
    typedef boost::variant<boost::blank, int, const char*, std::string> my_var_t;

    // Default constructor will construct an instance of boost::blank
    my_var_t var;

    // 'which()' method returns an index of a type,
    // currently held by varint.
    assert(var.which() == 0); // Empty state
    std::cout<<var.which()<<std::endl; //0
    //var = "Hello, dear reader";
    var = 99;
    assert(var.which() != 0);
    std::cout<<var.which()<<std::endl; //2
}

void example_func1() {
    boost::variant<int, std::string> variable(0);

    // Following method may throw a boost::bad_get exception
    // if actual value in variable is not an int
    int s1 = boost::get<int>(variable);
    std::cout<<s1<<std::endl;
    // If actual value in variable is not an int
    // will return an NULL pointer
    int* s2 = boost::get<int>(&variable);
    std::cout<<s2<<std::endl;


    //(void)s1; // Supressing warnings about unused variable
    (void)s2; // Supressing warnings about unused variable
}
