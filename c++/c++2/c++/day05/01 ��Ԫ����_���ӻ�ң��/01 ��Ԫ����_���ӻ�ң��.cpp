#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


class Remote;

class Television{
	friend class Remote;
public:
	enum{On,Off}; //定义电视机开关机状态
	enum{minVol,maxVol = 100}; //音量
	enum{minChannel = 1,maxChannel = 255}; //频道
	//电视机初始状态
	Television(){
		mState = Off;
		mVolumn = minVol;
		mChannel = minChannel;
	}
	//电视开关机
	void OnOrOff(){
		this->mState = (this->mState == Off ? On : Off);
	}
	//音量调节
	void VolumnUp(){
		if (this->mState == Off){
			return;
		}
		if (mVolumn >= maxVol){
			return;
		}
		mVolumn++;
	}

	void VolumnDown(){
		if (this->mState == Off){
			return;
		}
		if (mVolumn <= minVol){
			return;
		}
		mVolumn--;
	}

	//换台
	void ChannelUp(){
		if (this->mState == Off){
			return;
		}
		if (mChannel >= maxChannel){
			return;
		}
		mChannel++;
	}
	void ChannelDown(){
		if (this->mState == Off){
			return;
		}
		if (mChannel <= minChannel){
			return;
		}
		mChannel--;
	}
	//显示电视机当前状态
	void ShowState(){
		cout << "电视机状态:" << (mState == On ? "已开机" : "已关机") << endl;
		if (mState == On){
			cout << "当前频道:" << mChannel << endl;
			cout << "当前音量:" << mVolumn << endl;
		}
	}
private:
	int mState; //电视的开关机状态
	int mVolumn; //电视机的音量
	int mChannel; //频道
};


void test01(){

	Television television;
	television.OnOrOff(); //开机或者关机
	television.ChannelUp();
	television.ChannelUp();
	television.ChannelUp();
	television.ChannelUp();

	television.VolumnUp();
	television.VolumnUp();
	television.VolumnUp();

	//显示电视机当前状态
	television.ShowState();
}


//遥控器
class Remote{
public:
	Remote(Television* tele){
		pTele = tele;
	}
	//电视开关机
	void OnOrOff(){
		pTele->OnOrOff();
	}
	//音量调节
	void VolumnUp(){
		pTele->VolumnUp();
	}
	void VolumnDown(){
		pTele->VolumnDown();
	}
	//换台
	void ChannelUp(){
		pTele->ChannelUp();
	}
	void ChannelDown(){
		pTele->ChannelDown();
	}
	void ShowState(){
		pTele->ShowState();
	}
	//换到指定的频道
	void SetChannel(int channel){
		if (channel < Television::minChannel || channel > Television::maxChannel){
			return;
		}
		pTele->mChannel = channel;
	}
private:
	Television* pTele;
};


//遥控器玩电视
void test02(){
	Television* televison = new Television;
	Remote* remote = new Remote(televison);
	remote->OnOrOff(); //遥控器打开电视
	remote->VolumnUp();
	remote->VolumnUp();
	//遥控器可以设置指定的频道
	remote->SetChannel(100);
	remote->ShowState();
}


int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}