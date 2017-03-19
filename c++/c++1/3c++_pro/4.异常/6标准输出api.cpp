#include <iostream>
using namespace std;
#include <iomanip>


void main1()
{
	cout << "hello" << endl;
	cout.put('h').put('e').put('l') << endl;
	 
	char * p = "hello it";
	cout.write(p, strlen(p)) << endl;
	cout.write(p, strlen(p) - 4) << endl;
	cout.write(p, strlen(p)+4)<<endl;

	system("pause");
	return;
}
void main()
{
	cout << "<start>";
	cout.width(30);
	cout.fill('*');
	//cout.setf(ios::showbase);//#include <iomanip>
	//cout.setf(ios::internal);
	cout << hex << 123 << "<End>\n";


	system("pause");
}