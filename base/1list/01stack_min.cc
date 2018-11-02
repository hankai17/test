#include<iostream>
#include<stack>

class my_stack
{
    public:
        int stack_push(int data);
        int stack_pop();
        void traversal();
        void get_min() {
            std::cout<<"min "<<cur_min<<std::endl;
        }

    private:
        std::stack<int> stack_data;
        std::stack<int> stack_min;
        int cur_min;
};

int my_stack::stack_push(int data) {
    if (stack_data.size() == 0) {
        stack_data.push(data);
        stack_min.push(data);
        cur_min = data;  //忘了写
        return 0;
    }

    stack_data.push(data);
    if (data <= cur_min) { //必须加上=号  如果当前最小值有很多重复的 这就牵扯到如何弹出最后一个最小值的问题
        stack_min.push(data);
        cur_min = data;
    }
    return 0;
}

int my_stack::stack_pop() {
    if (stack_data.size() == 0) {
        return 0;
    }

    if (stack_data.top() == cur_min) {
        stack_min.pop();
        cur_min = stack_min.top();
    }
    stack_data.pop();
    return 0;
}

void my_stack::traversal() {
    std::cout<<"stack 不支持遍历"<<std::endl; 
}


int main()
{
    my_stack s;
    s.stack_push(3);
    s.stack_push(4);
    s.stack_push(5);
    s.stack_push(1);
    s.stack_push(2);
    s.stack_push(1);

    s.get_min();

    s.stack_pop();
    s.get_min(); //1
    
    s.stack_pop();
    s.get_min(); //1

    s.stack_pop();
    s.get_min(); //3

    s.stack_pop();
    s.get_min(); //3

    s.stack_pop();
    s.get_min(); //3

    return 0;
}










/* 放弃 这样传参太多 太麻烦 所以设计一个类 
int stack_push(std::stack<int> stack_data, std::stack<int> stack_min) {
    return 0;
}

int stack_pop() {
    return 0;
}

int main()
{
    std::stack<int> stack_data;
    std::stack<int> stack_min;


    return 0;
}
*/
