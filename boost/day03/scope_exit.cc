#include <boost/scope_exit.hpp>
#include <cstdlib>
#include <cstdio>
#include <cassert>

//当一个程序因为返回或异常离开当前作用域时 finally或scope块中的代码将会执行

int main() {
    std::FILE* f = std::fopen("example_file.txt", "w");
    assert(f);
    BOOST_SCOPE_EXIT(f) { //无论怎样都会执行这个函数
    // Whatever happened in scope, this code will be executed
    // and file will be correctly closed.
        std::fclose(f);
    } BOOST_SCOPE_EXIT_END

    // Some code that may throw or return.
    // ...
}

class theres_more_example {
public:
    void close(std::FILE*);

    void theres_more_example_func() {
        std::FILE* f = 0;
        BOOST_SCOPE_EXIT(f, this_) { // Capture object `this_`. //传递多个值
            this_->close(f);
        } BOOST_SCOPE_EXIT_END
    }
};
