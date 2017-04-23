#include <cstdlib>
#include <iostream>
#include<fstream>

using namespace std;

struct Students {
    char regNum[9];
    char firstName[35];
    char surname[35];
    int age;
};

int main(int argc, char** argv) 
{
    char opt;
    char ch;
    int number;
    Students student;
    ofstream outFile;
    outFile.open("./studentsFile", ios::app | ios::binary);

    int x;
    cout << "Welcome: Random chakadaro whatever..";
    cout << "Ukudei??\nChoose option number: \n\n 1. Add records \n 2. Read records..\n\n";
    cin>>x;

    if (x==1)
        do {
            cout << "Enter Registration Number..\t"<<endl;
            //        gets(student.regNum);
            cin>>(student.regNum);
            cout << "Enter First Name..";
            //        gets(student.firstName);
            cin>>(student.firstName);
            cout << "Enter Surname..";
            //        gets(student.surname);
            cin>>(student.firstName);
            cout << "Enter Age..";
            cin >> student.age;
            outFile.write((char*) &student, sizeof (struct Students));
            if (outFile)
                cout << "\nRecord saved successfully" << endl;
            cout << "Another record <y-yes n-no>" << endl;
            cin.ignore();
            opt = getchar();
            cin.ignore();
        } 

        while (opt != 'n');
    outFile.close();
    ifstream inFile("./studentsFile", ios::in | ios::binary);


    if (x==2)
    {
        cout << "Enter the number of record to retrieve...";
        cin >> number;
        inFile.seekg(sizeof (struct Students) *(number - 1), ios::beg);//algorithm to seek through the record to the requested record
        inFile.read((char *) &student, sizeof (struct Students));
        cout << "Registration Number..." << student.regNum << endl;
        cout << "First name.." << student.firstName << endl;
        cout << "Surname.." << student.surname << endl;
        cout << "Age.." << student.age << endl;
    }

    else {cout<<"Error, please enter a valid option number..."<<endl;}

    //    while (opt != 'n');
    //    outFile.close();
    //    ifstream inFile("Files\\studentsFile", ios::in | ios::binary);
    return 0;
}

