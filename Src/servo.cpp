#include "servo.h"

#include <unistd.h>
#include <PiPCA9685/PCA9685.h>
#include <iostream>
#include <bcm2835.h>

#include <iostream>
#include <thread>
#include <chrono>

void servo(int num) {
  
  PiPCA9685::PCA9685 pca{};
  pca.set_pwm_freq(50.0);
  
  pca.set_pwm(num, 0, 370);
  // usleep(1'000'000);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠1秒
  
  pca.set_pwm(num, 0, 460);
  // usleep(5'000'000);
  std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // 休眠1秒
  
  pca.set_pwm(num, 0, 370);
  // usleep(1'000'000);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠1秒

  // while(true) {
  //   pca.set_pwm(0, 0, 370);
  //   usleep(1'000'000);
  //   pca.set_pwm(0, 0, 415);
  //   usleep(1'000'000);
  //   pca.set_pwm(0, 0, 460);
  //   usleep(1'000'000);
  //   pca.set_pwm(0, 0, 415);
  //   usleep(1'000'000);
  // }
}