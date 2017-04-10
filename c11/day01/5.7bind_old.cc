#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class Person
{
	public:
		Person(string name,int age){
			this->mName = name;
			this->mAge = age;
		}

		bool operator==(const Person& p){
			if (this->mName == p.mName && this->mAge == p.mAge){
				return true;
			}
			return false;
		}

		//重载<
		bool operator<(const Person& p) const{
			return this->mAge < p.mAge;
		}
	public:
		string mName;
		int mAge;
};

struct search_condition : public binary_function<Person*,Person*,bool>{
	bool operator()(Person* p,Person* p2) const{
		if (p->mName == p2->mName && p->mAge == p2->mAge){
			return true;
		}
		return false;
	}
};

void test03()
{

	vector<Person*> v;

	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);


	Person* p = new Person("bbb",20);

	vector<Person*>::iterator it = find_if(v.begin(), v.end(), bind2nd(search_condition(),p));
	if (it == v.end()){
		cout << "找不到!" << endl;
	}
	else{
		cout << "找到:" << (*it)->mName << ":" << (*it)->mAge << endl;
	}

}
int main()
{
	test03();
	return 0;
}
/* 
  int count = std::count_if(coll.begin(),coll.end(),
  std::bind1st(less<int>(),10)); //大于10的个数
  
  int count = std::count_if(coll.begin(),coll.end(),
  std::bind2nd(less<int>(),10)); //小于10的个数
----------------------------------------------------- 
  int count = std::count_if(coll.begin(),coll.end(),
  std::bind(less<int>(),10,_1)); //大于10的个数



 */
