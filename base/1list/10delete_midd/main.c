#include<stdio.h>
#include "LinkList.h"

typedef struct Person
{
  int age;
  int id;
}Person;

void MyPrint(void *data)
{
  Person * p = (Person*)data;
  printf("name:%d  id:%d \n", p->id, p->age);
}

int main()
{
  Person p1={1,1};
  Person p2 = { 2, 2 };
  Person p3 = { 3, 3 };
  Person p4 = { 4, 4 };
  Person p5 = { 5, 5 };

  LinkList list=Init_LinkList();
  Insert_LinkList(list,0,(void *)&p1);
  //Insert_LinkList(list,0,(void *)&p2);
  //Insert_LinkList(list,0,(void *)&p3);
  //Insert_LinkList(list,0,(void *)&p4);
  //Insert_LinkList(list,0,(void *)&p5);

  Print_LinkList(list, MyPrint);

  printf("**************\n");
  //RemoveByPos_LinkList(list, 5);
  RemoveMiddle_LinkList(list);
  Print_LinkList(list, MyPrint);

  return 0;

}


int main1()
{
  Person p1={1,1};
  Person p2 = { 2, 2 };
  Person p3 = { 3, 3 };
  Person p4 = { 4, 4 };
  Person p5 = { 5, 5 };

  LinkList list=Init_LinkList();
  Insert_LinkList(list,0,(void *)&p1);
  Insert_LinkList(list,0,(void *)&p2);
  Insert_LinkList(list,0,(void *)&p3);
  Insert_LinkList(list,0,(void *)&p4);
  Insert_LinkList(list,0,(void *)&p5);

  Print_LinkList(list, MyPrint);

  printf("**************\n");
  RemoveByPos_LinkList(list, 5);
  Print_LinkList(list, MyPrint);

  return 0;

}

