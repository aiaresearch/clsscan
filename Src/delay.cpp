#include <iostream>
#include <thread>
#include <chrono> // 包含时间相关的头文件
#include <unistd.h>
#include "servo.h"
#include "GPIO.h"

int main() {
    // 创建一个新线程并启动
    std::thread t(servo,0);
    GPIOmotor();
    usleep(10'000'000);
    GPIOmotoroff();
    
    // 等待子线程结束
    //t.join();
    t.detach();
    
    return 0;
}
