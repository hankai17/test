#include"mystring.hh"

mystring::mystring() {
  this->mLength = 0;
  this->pAddress = new char[1];
  this->pAddress[0] = '\0';
}

mystring::mystring(const char* str) {
  //字符串长度
  this->mLength = strlen(str);
  this->pAddress = new char[this->mLength + 1];
  strcpy(this->pAddress,str);
}

mystring::mystring(const mystring& str) {
  this->mLength = str.mLength;
  this->pAddress = new char[this->mLength + 1];
  strcpy(this->pAddress,str.pAddress);
}

//字符串操作
const char* mystring::c_str() { return this->pAddress; }
int mystring::size() { return this->mLength; }

//赋值操作
mystring& mystring::operator=(const mystring& str) {

  if (this->pAddress != NULL) {
	delete[] this->pAddress;
	this->pAddress = NULL;
  }

  this->mLength = str.mLength;
  this->pAddress = new char[this->mLength + 1];
  strcpy(this->pAddress, str.pAddress);

  return *this;
}


mystring& mystring::operator=(const char* p) {

  if (this->pAddress != NULL) {
	delete[] this->pAddress;
	this->pAddress = NULL;
  }

  this->pAddress = new char[strlen(p) + 1];
  strcpy(this->pAddress,p);

  return *this;
}

//mystring str3 = str1 + str2; //str1.operator+(str2) => operator+(&str1,str2)
//重载+
mystring mystring::operator+(mystring& str) {

  //统计两个字符串长度和
  int newLength = this->mLength + str.mLength;
  //根据长度开辟内存
  char* buf = new char[newLength + 1];
  //初始化buf
  for (int i = 0; i < newLength + 1; i ++) {
	buf[i] = '\0';
  }

  strcat(buf, this->pAddress);
  strcat(buf, str.pAddress);

  //创建新的mystring对象
  mystring newString;
  delete[] newString.pAddress;
  newString.pAddress = buf;
  newString.mLength = newLength;

  return newString;
}

//mystring str3 = str1 + "abcd";
mystring mystring::operator+(const char* str) {

  int newLength = this->mLength + strlen(str);
  //根据长度开辟内存
  char* buf = new char[newLength + 1];
  //初始化buf
  for (int i = 0; i < newLength + 1; i++) {
	buf[i] = '\0';
  }
  strcat(buf, this->pAddress);
  strcat(buf, str);

  //创建新的mystring对象
  mystring newString;
  delete[] newString.pAddress;
  newString.pAddress = buf;
  newString.mLength = newLength;

  return newString;
}

// ==, != 重载
bool mystring::operator==(const char* p) {
  if (strcmp(this->pAddress,p) == 0) {
	return true;
  }
  return false;
}

bool mystring::operator!=(const char* p) {
  if (strcmp(this->pAddress, p) != 0) {
	return true;
  }
  return false;
}

bool mystring::operator==(mystring& obj) {
  if (strcmp(this->pAddress,obj.pAddress) == 0) {
	return true;
  }
  return false;
}

bool mystring::operator!=(mystring& obj) {
  if (strcmp(this->pAddress, obj.pAddress) != 0) {
	return true;
  }
  return false;
}

//重载左移操作符
std::ostream& operator<<(std::ostream& out, mystring& str) {
  out << str.pAddress;
  return out;
}

mystring::~mystring() {
  if (this->pAddress != NULL) {
	delete[] this->pAddress;
	this->pAddress = NULL;
	this->mLength = 0;
  }
}

