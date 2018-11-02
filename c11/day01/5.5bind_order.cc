#include <iostream>
#include <functional>

void output(int x, int y) {
  std::cout << x << " " << y << std::endl;
}

int main(void)
{
  std::bind(output, 1, 2)();
  std::bind(output, std::placeholders::_1, 2)(1); //std::placeholders::_1 ��ռλ ռλ�ǵ��õ�ʱ�򴫲�ȷ����ֵ  1 2
  std::bind(output, 2, std::placeholders::_1)(1); // ��һ��������2 �ڶ���������1
  std::bind(output, 2, std::placeholders::_2)(1);  //��һ��������2  error:����ʱû�еڶ�������
  std::bind(output, 2, std::placeholders::_2)(1, 2);  //��� 2 2   ����ʱ��һ���������̵���
  std::bind(output, std::placeholders::_1, std::placeholders::_2)(1, 2);  //��� 1 2
  std::bind(output, std::placeholders::_2, std::placeholders::_1)(1, 2);  //��� 2 1

  return 0;
}