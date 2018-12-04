#include<iostream>
using namespace std;

class BaseException {
  public:
	virtual void printError() = 0;
};

class TargetSpaceNullException : public BaseException {
  public:
	TargetSpaceNullException() {
	  cout << "TargetSpaceNullException created!" << endl;
	}
	TargetSpaceNullException(const TargetSpaceNullException&) {
	  cout << "TargetSpaceNullException copy created!" << endl;
	}
	virtual void printError() {
	  cout << "target err!" << endl;
	}
	~TargetSpaceNullException() {
	  cout << "TargetSpaceNullException decreated!" << endl;
	}
};

class SourceSpaceNullException : public BaseException {
  public:
	SourceSpaceNullException() {
	  cout << "SourceSpaceNullException created!" << endl;
	}
	SourceSpaceNullException(const SourceSpaceNullException&) {
	  cout << "SourceSpaceNullException copy fun!" << endl;
	}
	void printError() {
	  cout << "source err!" << endl;
	}
	~SourceSpaceNullException() {
	  cout << "SourceSpaceNullException decreated!" << endl;
	}
};


void CopyString(char* dst,const char* source) {
  if (dst == NULL) {
	throw TargetSpaceNullException();
  }

  if(source == NULL) {
	throw SourceSpaceNullException();
  }

  const char* temp = source;
  char* target = dst;
  while (*temp != '\0') {
	*target = *temp;
	target++;
	temp++;
  }
}



int main() {
  const char* source = "hello world!";
  char buf[1024] = {0};

  try {
	CopyString(buf, NULL);
  }
#if 0
  catch (TargetSpaceNullException ex) {
	ex.printError();
  }
  catch (SourceSpaceNullException& ex) {
	ex.printError();
  }
#else
  catch (BaseException& ex){
	ex.printError();
  }
#endif
  cout << "buf:" << buf << endl;
  return 0;
}
