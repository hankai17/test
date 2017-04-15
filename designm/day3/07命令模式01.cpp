#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>

using namespace std;

//系统的一个核心类模块
class Doctor
{
	public:
		//治疗眼睛的方法
		void treatEyes() 
		{
			cout << "医生治疗了眼睛" << endl;
		}
		//治疗鼻子的方法
		void treatNose() 
		{
			cout << "医生治疗的鼻子" << endl;
		}
};

//治疗眼睛的病单
class CommandEyes
{
	public:
		CommandEyes(Doctor *doctor)
		{
			this->doctor = doctor;
		}
		~CommandEyes() 
		{
			if (this->doctor != NULL) 
			{
				delete this->doctor;
				this->doctor = NULL;
			}
		}
		//病单执行治疗的时候，实际上让医生治疗
		void treat() 
		{
			doctor->treatEyes();
		}
	private:
		Doctor *doctor;
};

//治疗鼻子的病单
class CommandNose
{
	public:
		CommandNose(Doctor *doctor) 
		{
			this->doctor = doctor;
		}
		~CommandNose() 
		{
			if (this->doctor != NULL) 
			{
				delete this->doctor;
				this->doctor = NULL;
			}
		}

		void treat()
		{
			doctor->treatNose();
		}
	private:
		Doctor *doctor;
};

//病人
int main(void)
{
#if 0
	//直接找医生
	Doctor *doctor = new Doctor;
	doctor->treatEyes();
	doctor->treatNose();
#endif
	//病人通过填写病单 来找医生
	Doctor *doctor = new Doctor;
	CommandEyes *cmdEyes = new CommandEyes(new Doctor);
	cmdEyes->treat();

	delete cmdEyes;


	CommandNose *cmdNose = new CommandNose(new Doctor);
	cmdNose->treat();

	return 0;
}
