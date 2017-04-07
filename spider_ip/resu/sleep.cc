#include <iostream>
#include <thread>
#include <chrono>

int main(void)
{
    std::chrono::milliseconds sleepDuration(250);
    while(1)
    {
        std::this_thread::sleep_for(sleepDuration);
        std::cout<<"*"<<std::endl;
    } 
    return 0;
}
