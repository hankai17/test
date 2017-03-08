#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <list>
using namespace std;
//命令的最终执行者
class Cooker
{
    public:
    //烤串
    void makeChuaner() 
    {
        cout << "烤串师傅进行了烤串" << endl;
    }
    //烤鸡翅
    void makeChicken() 
    {
        cout << "烤串师傅进行了烤鸡翅" << endl;
    }
};
//烤串的  抽象的 菜单
class Command
{
    public:
    Command(Cooker *cooker)
    {
        this->cooker = cooker;
    }
    ~Command() 
    {
        if (this->cooker != NULL) 
        {
            delete this->cooker;
            this->cooker = NULL;
        }
    }
    //菜单让最终的执行者干活的方法
    virtual void execute() = 0;
    protected:
    Cooker *cooker;
};
//烤串的菜单
class CommandChuaner :public Command
{
    public:
    CommandChuaner(Cooker *cooker) : Command(cooker) {}
    virtual void execute()  
    {
        //命令 最终让执行者干的工作。
        this->cooker->makeChuaner();
    }
};
//烤鸡翅的菜单
class CommandChicken :public Command
{
    public:
    CommandChicken(Cooker * cooker) : Command(cooker) {}
    virtual void execute() 
    {
        //命令 最终让执行者干的工作。
        this->cooker->makeChicken();
    }
};
//管理所有命令的一个模块
//服务员MM
class Waitress
{
    public:
    //给服务员添加菜单的方法
    void setCmd(Command *cmd)
    {
        this->cmd_list.push_back(cmd);
    }
    //让服务员mm 下单
    void notify() 
    {
        list<Command *>::iterator it = cmd_list.begin();
        for (; it != cmd_list.end(); it++) {
            (*it)->execute(); //在此发生了多态
        }
    }
    private:
    list<Command *> cmd_list;
};
int main(void)
{
    Waitress *mm = new Waitress;
    Command *chuanger = new CommandChuaner(new Cooker);
    Command *chicken = new CommandChicken(new Cooker);
    //把订单都给服务员
    mm->setCmd(chuanger);
    mm->setCmd(chicken);
    //让服务员下单，最终让师傅干活
    mm->notify();
    delete mm;
    return 0;
}
