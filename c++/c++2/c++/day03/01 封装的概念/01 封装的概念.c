#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


typedef struct _Person{
	char name[64];
	int age;
}Person;

typedef struct _Aninal{
	char name[64];
	int age;
	int type; //动物种类
}Ainmal;

void PersonEat(Person* person){
	printf("%s在吃人吃的饭!\n", person->name);
}
void AnimalEat(Ainmal* animal){
	printf("%s在吃动物吃的饭!\n", animal->name);
}

int main(){

	Person person;
	strcpy(person.name, "小明");
	person.age = 30;
	AnimalEat(&person);

	system("pause");
	return EXIT_SUCCESS;
}