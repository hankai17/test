#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
#include <list>
using namespace std;
//前置声明notifier
class Notifier;
//抽象的观察者
class Linstenner
{
    public:
    //当朋友被揍了我改怎么办
    virtual void onFriendBeFight(Linstenner *one, Linstenner *another/*, Notifier *baixiao*/) = 0;
    //one是打别人的人， another 是被揍的
    virtual void fighting(Linstenner *another, Notifier *notifier) = 0;
    virtual string getName() = 0;
    virtual string getParty() = 0;
};
//抽象的通知者
class Notifier
{
    public:
    //添加观察者
    virtual void addListenner(Linstenner *listenner) = 0;
    //删除观察者
    virtual void delListenner(Linstenner *listenner) = 0;
    //通知观察者
    virtual void notify(Linstenner *one, Linstenner *another) = 0;
};
// 武林中的人物
class Hero :public Linstenner
{
    public:
    Hero(string name, string party)
    {
        this->name = name;
        this->party = party;
    }
    //当我发现一个消息之后我该怎么办
    virtual void onFriendBeFight(Linstenner *one, Linstenner *another/*, Notifier *baixiao*/)
    {
        if (another->getName() != this->name &&one->getName() != this->name) {
            //不是当事人
            //如果不是当事人，需要判断打人的pary 和 被打的party是不是我自己哥们
            if (one->getParty() == this->party) {
                //自己人把 比人揍了
                cout << name << "发现自己人把别人揍了， 笑了 ， 拍手叫好" << endl;
            }
            else if (another->getParty() == this->party){
                //自己人被揍了
                cout << name << "发现自己人被别人走了， 出手援救" << endl;
                //this->fighting(one, baixiao);
            }
        }
        else {
            //当事人
            //如果是当事人，什么都不敢
        }
    }
    //揍人的方法
    virtual void fighting(Linstenner *another, Notifier *notifier)
    {
        cout << name << "[" << this->party << "]" << " 把 " 
        << another->getName() << "[" << another->getParty() << "]" << "给揍了" << endl;
        // 揍完之后，这个事件应该让百晓生知晓
        //应该调用百晓生 的notify方法
        notifier->notify(this, another);
    }
    string getName() 
    {
        return this->name;
    }
    string getParty() 
    {
        return this->party;
    }
private:
    string name;
    string party;
};
class Baixiao :public Notifier
{
    public:
    //添加观察者
    virtual void addListenner(Linstenner *listenner)  {
        this->l_list.push_back(listenner);
    }
    //删除观察者
    virtual void delListenner(Linstenner *listenner) {
        this->l_list.remove(listenner);
    }
    //通知观察者
    virtual void notify(Linstenner *one, Linstenner *another)  {
        for (list<Linstenner *>::iterator it = l_list.begin(); it != l_list.end(); it++) {
            (*it)->onFriendBeFight(one, another/*, this*/);
        }
    }
    private:
    //拥有所有武林人士的名单
    list<Linstenner*> l_list;
};
int main(void)
{
    Linstenner *hong7 = new Hero("洪七公", "丐帮");
    Linstenner *huangrong = new Hero("黄蓉", "丐帮");
    Linstenner *wuyazi = new Hero("无崖子", "逍遥派");
    Linstenner *tonglao = new Hero("天山童姥", "逍遥派");
    //创建一个百晓生
    Notifier *baixiao = new Baixiao;
    //百晓生 手机全部的武林人士名单
    baixiao->addListenner(hong7);
    baixiao->addListenner(huangrong);
    baixiao->addListenner(wuyazi);
    baixiao->addListenner(tonglao);
    //以上初始化完毕
    hong7->fighting(wuyazi, baixiao);
    cout << "----" << endl;
    tonglao->fighting(hong7, baixiao);
    return 0;
}
