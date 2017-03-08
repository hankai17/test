#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <list>
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
//定义一个抽象的病单
class Command
{
    public:
    Command(Doctor *doctor)
    {
        this->doctor = doctor;
    }
    ~Command() 
    {
        if (this->doctor != NULL) 
        {
            delete doctor;
            doctor = NULL;
        }
    }
    virtual void treat() = 0;
    protected:
    Doctor *doctor;
};
//治疗眼睛的病单
class CommandEyes :public Command
{
    public:
    CommandEyes(Doctor *doctor) :Command(doctor) {}
    //病单执行治疗的时候，实际上让医生治疗
    void treat() 
    {
        doctor->treatEyes();//命令让最终的执行者 干活。
    }
};
//治疗鼻子的病单
class CommandNose :public Command
{
    public:
    CommandNose(Doctor *doctor) :Command(doctor)
    {
    }
    virtual void treat() 
    {
        doctor->treatNose();
    }
};
//护士类
class Nurse
{
    public:
    Nurse(Command *cmd)
    {
        this->cmd = cmd;
    }
    ~Nurse() 
    {
        if (this->cmd != NULL) 
        {
            delete this->cmd;
        }
    }
    //下发病单的方法
    void notify() 
    {
        this->cmd->treat();
    }
    private:
    Command *cmd;
};
//护士长
class NurseBoss
{
    public:
    NurseBoss()
    {
        m_list.clear();
    }
    ~NurseBoss() 
    {
        m_list.clear();
    }
    //给护士长添加病单的方法
    void setCmd(Command *cmd)
    {
        this->m_list.push_back(cmd);
    }
    //下发手里所有病单的指令
    void notify()
    {
        for (list<Command*>::iterator it = m_list.begin(); it != m_list.end(); it++)
        {
            (*it)->treat(); // 会发生多态， Command 是所有命令的父类。
        }
    }
    private:
    list<Command*> m_list;
};
int main(void)
{
    NurseBoss *woman = new NurseBoss;
    Command *cmd1 = new CommandEyes(new Doctor);
    Command *cmd2 = new CommandNose(new Doctor);
    //将所有指令都给护士长
    woman->setCmd(cmd1);
    woman->setCmd(cmd2);
    woman->notify();
    return 0;
}
