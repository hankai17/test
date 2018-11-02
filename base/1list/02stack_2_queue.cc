#include<iostream>
#include<stack>
#include<queue>
//即两个栈来回跳
class my_queue {
  public:
	int queue_push(int data);
	int queue_pop();

  private:
	std::stack<int> stack_push;
	std::stack<int> stack_pop;
};

int my_queue::queue_push(int data) { //只有stack_pop为空的时候才能一口气 把stack_push中的数据移进去
  stack_push.push(data);
  return 0;
}

int my_queue::queue_pop() {
  if (stack_pop.size() == 0) {
	while (!stack_push.empty()) {
	  int tmp = stack_push.top();
	  stack_pop.push(tmp);
	  stack_push.pop();
	}
  }
  int tmp = stack_pop.top();
  stack_pop.pop();
  return tmp; //弹出的值
}


int main() {
  class my_queue q;

  q.queue_push(1);
  q.queue_push(2);
  q.queue_push(3);
  q.queue_push(4);
  q.queue_push(5);

  int ret = q.queue_pop();
  std::cout<<"ret "<<ret<<std::endl;

  ret = q.queue_pop();
  std::cout<<"ret "<<ret<<std::endl;

  ret = q.queue_pop();
  std::cout<<"ret "<<ret<<std::endl;

  return 0;
}
