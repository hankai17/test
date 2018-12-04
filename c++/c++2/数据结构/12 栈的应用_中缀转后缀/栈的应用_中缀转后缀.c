#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SeqStack.h"


int IsNumber(char ch){
	return ch >= '0' && ch <= '9';
}
int IsLeft(char ch){
	return ch == '(';
}
int IsRight(char ch){
	return ch == ')';
}
int IsOperator(char ch){
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}
int GetPriotity(char ch){
	if (ch == '*' || ch == '/'){
		return 2;
	}

	if(ch == '+' || ch == '-'){
		return 1;
	}

	return 0;
}


int main(){

	char* str = "8+(3-1)*5";
	char* p = str;

	//³õÊ¼»¯Õ»
	SeqStack stack = Init_SeqStack();

	while(*p != '\0'){
		
		//Êý×ÖÖ±½ÓÊä³ö
		if (IsNumber(*p)){
			printf("%c",*p);
		}
		//·ûºÅ
		else{
		
			//×óÀ¨ºÅ½øÕ»
			if (IsLeft(*p)){
				Push_SeqStack(stack, p);
			}

			//ÔËËã·ûºÅ
			if (IsOperator(*p)){
			
				while (Size_SeqStack(stack) > 0 &&  GetPriotity(*(char*)Top_SeqStack(stack)) >= GetPriotity(*p) ){
					//Êä³öÕ»¶¥·ûºÅ
					printf("%c", *(char*)Top_SeqStack(stack));
					Pop_SeqStack(stack);
				}
				//µ±Ç°·ûºÅÈëÕ»
				Push_SeqStack(stack, p);
			}

			//ÓÒÀ¨ºÅ ´ÓÕ»ÖÐÆ¥Åä×óÀ¨ºÅ
			if (IsRight(*p)){
				
				while (Size_SeqStack(stack) > 0 && !IsLeft(*(char*)Top_SeqStack(stack))){
					printf("%c",*(char*)Top_SeqStack(stack));
					Pop_SeqStack(stack);
				}
				//µ¯³ö×óÀ¨ºÅ
				Pop_SeqStack(stack);
			}

		}
		p++;
	}

	//µ¯³öÕ»ÖÐÔªËØ
	while (Size_SeqStack(stack) > 0){
		printf("%c", *(char*)Top_SeqStack(stack));
		Pop_SeqStack(stack);
	}

	system("pause");
	return EXIT_SUCCESS;
}