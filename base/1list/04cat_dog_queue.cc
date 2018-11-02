#include<iostream>
#include<string>
#include<queue>
/*
物收容所只收容狗与猫 并且严格遵守“先进先出”原则
在收养该收容所的动物时 收养人只能收养所有动物中 “最老”（根据进入收容所的时间长短）的动物
*/

class pet {
  public:
	pet(){}
	pet(std::string t):type(t) {}
	std::string get_pet_type() {return this->type;}

  private:
	std::string type;
};

class dog:public pet {
  public:
	dog():pet("dog") {}
};

class cat:public pet {
  public:
	cat():pet("cat") {}
};

//设计中间层
class pet_entry {
  public:
	pet_entry() { count = 0;}
	std::string get_entry_type() { return p->get_pet_type();}
	pet_entry(pet* p, int c) :p(p){
	  count = c;
	}

	int count; //戳 即第几个
  private:
	pet* p;
};

class cat_dog_queue {
  public:
	cat_dog_queue() {}

	void add(pet_entry* pe) { //必须得用多态了  so设计成员的时候都设为指针
	  if( pe->get_entry_type() == "dog") {
		dog_queue.push(pe);
	  } else { //不严谨
		cat_queue.push(pe);
	  }
	}

	pet_entry* time_is_now(int c) {
	  pet_entry* tmp_pet = nullptr;
	  if (c == (tmp_pet = dog_queue.front())->count) {
		dog_queue.pop();
	  } else {
		tmp_pet = cat_queue.front();
		cat_queue.pop();
	  }
	  return tmp_pet;
	}

	void get_poll_all() { //一旦弹出就破坏了源结构 想到了昨天的递归 想用递归在破换源结构后还原 发现不可行
	  int tmp_count = 0; 
	  int all_size = dog_queue.size() + cat_queue.size();       
	  pet_entry* tmp_pet = nullptr;
	  while (tmp_count < all_size) {
		tmp_pet = time_is_now(tmp_count);
		tmp_count++;
		std::cout<<"pop "<<tmp_pet->get_entry_type()<<std::endl;
	  }
	}

	void get_poll_dog() {
	  while(!dog_queue.empty()) {
		std::cout<<"pop "<<(dog_queue.front())->get_entry_type()<<std::endl;
		dog_queue.pop();
	  }
	}

	void get_poll_cat() {
	  while(!cat_queue.empty()) {
		std::cout<<"pop "<<(cat_queue.front())->get_entry_type()<<std::endl;
		cat_queue.pop();
	  }
	}

  private:
	std::queue<pet_entry*> dog_queue;
	std::queue<pet_entry*> cat_queue;
};


int main()
{
  pet_entry* p0 = new pet_entry(new cat(), 0);
  pet_entry* p1 = new pet_entry(new dog(), 1);
  pet_entry* p2 = new pet_entry(new dog(), 2);
  pet_entry* p3 = new pet_entry(new dog(), 3);
  pet_entry* p4 = new pet_entry(new cat(), 4);
  pet_entry* p5 = new pet_entry(new dog(), 5);

  cat_dog_queue cdq;
  cdq.add(p0);
  cdq.add(p1);
  cdq.add(p2);
  cdq.add(p3);
  cdq.add(p4);
  cdq.add(p5);

  cdq.get_poll_all();
  return 0;
}

//总结: 输出的是两个类型 所以存储的时候用两个队列
//
