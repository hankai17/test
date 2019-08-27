#include <iostream>
#include <list>

int main() {
  std::list<int> mylist1, mylist2;
  std::list<int>::iterator it;

  for(int i = 1; i <= 4; ++i)
     mylist1.push_back(i);        //mylist1: 1 2 3 4
  for(int i = 1; i <= 3; ++i)
     mylist2.push_back(i * 10);   //mylist2: 10 20 30

  it = mylist1.begin();
  ++it;                           //points to 2
  std::list<int>::iterator tmp_it1 = mylist2.begin();

  mylist1.splice(it, mylist2);   //list2元素插入list1并从list2中删除。

  for (std::list<int>::iterator tit = mylist1.begin(); tit != mylist1.end(); ++tit) //mylist1: 1 10 20 30 2 3 4
    std::cout << ' ' << *tit; std::cout << '\n';
  for (std::list<int>::iterator tit = mylist2.begin(); tit != mylist2.end(); ++tit)
    std::cout << ' ' << *tit; std::cout << '\n';

  std::cout<<"tmp_it1 = "<<*tmp_it1<<std::endl; //10
  std::cout<<"-----------------"<<std::endl;


  std::cout<<"before *it = "<<*it<<std::endl; //2
  mylist2.splice(mylist2.begin(), mylist1, it); //2
  std::cout<<"after *it = "<<*it<<std::endl;
  for (std::list<int>::iterator tit = mylist1.begin(); tit != mylist1.end(); ++tit) //mylist1: 1 10 20 30 2 3 4
    std::cout << ' ' << *tit; std::cout << '\n';
  for (std::list<int>::iterator tit = mylist2.begin(); tit != mylist2.end(); ++tit)
    std::cout << ' ' << *tit; std::cout << '\n';

  std::cout<<"-----------------"<<std::endl;
  it = mylist1.begin();
  std::advance(it,3);           // "it" points now to 30

  mylist1.splice ( mylist1.begin(), mylist1, it, mylist1.end());
                                // mylist1: 30 3 4 1 10 20
    //注意此处mylist前后交换了，这可以用作list形如循环移位的操作
  return 0;
}

