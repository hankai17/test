#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

class Hero {
    public:
    Hero() {
        m_strategy = nullptr;
    }
    void setStrategy(std::function<void()> cb) {
        m_strategy = cb;
    }
    void fight() {
        cout << "begin fight" << endl;
        m_strategy();
    }
    private:
    std::function<void()> m_strategy;
};

int main(void) {
    Hero *hero = new Hero;

    cout << "will use gun" << endl;
    hero->setStrategy([](){
          std::cout << "ak47" << std::endl;
          });
    hero->fight();

    cout << "will use cool weapon" << endl;
    hero->setStrategy([](){
          std::cout << "knief" << std::endl;
          });
    hero->fight();

    return 0;
}
