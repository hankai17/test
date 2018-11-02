#include "messagebus.hh"

MessageBus g_bus;
const string Topic = "Drive";
const string CallBackTopic = "DriveOk";

struct Subject {
  Subject() {
	g_bus.Attach([this]{DriveOk();}, CallBackTopic);
  }
  void SendReq(const string topic) {
	g_bus.SendReq<void, int>(50, topic);
  }
  void DriveOk() {
	cout<<"drive ok"<<endl;
  }
};

struct Car {
  Car() {
	g_bus.Attach([this](int speed){Drive(speed);}, Topic); //向总线注册 主题为drive消息类型为void(int)的消息
  }
  void Drive(int speed) {
	cout<<"Car drive "<<speed<<endl;
    g_bus.SendReq<void>(CallBackTopic);
  }
};

struct Bus {
  Bus() {
	g_bus.Attach([this](int speed){Drive(speed);});
  }
  void Drive(int speed) {
	cout<<"Bus drive "<<speed<<endl;
    g_bus.SendReq<void>(CallBackTopic);
  }
};

struct Truck {
  Truck() {
	g_bus.Attach([this]{Drive();});
  }
  //void Drive(int speed) {
  void Drive() {
	//cout<<"Truck drive "<<speed<<endl;
	cout<<"Truck drive "<<endl;
    g_bus.SendReq<void>(CallBackTopic);
  }
};

void test() {
  Subject subject;
  Car car;
  Bus bus;
  Truck truck;

  subject.SendReq(Topic); //car bus
  subject.SendReq(""); //truck

  //g_bus.Remove<void, int>();
  //subject.SendReq("");
}

int main() {
  test();
  return 0;
}
