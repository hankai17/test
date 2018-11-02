#include "messagebus.hh"

MessageBus g_bus;
const string Topic = "Drive";

struct Subject {
  void SendReq(const string topic) {
	g_bus.SendReq<void, int>(50, topic);
  }
};

struct Car {
  Car() {
	g_bus.Attach([this](int speed){Drive(speed);}, Topic); //向总线注册 主题为drive消息类型为void(int)的消息
  }
  void Drive(int speed) {
	cout<<"Car drive "<<speed<<endl;
  }
};

struct Bus {
  Bus() {
	g_bus.Attach([this](int speed){Drive(speed);}, Topic);
  }
  void Drive(int speed) {
	cout<<"Bus drive "<<speed<<endl;
  }
};

struct Truck {
  Truck() {
	g_bus.Attach([this](int speed){Drive(speed);}); //向总线注册 主题为空消息类型为void(int)的消息
  }
  void Drive(int speed) {
	cout<<"Truck drive "<<speed<<endl;
  }
};

void test() {
  Subject subject;
  Car car;
  Bus bus;
  Truck truck;

  subject.SendReq(Topic); //car bus
  subject.SendReq(""); //truck

  g_bus.Remove<void, int>();
  subject.SendReq("");
}

int main() { //
  test();
  return 0;
}
