#include <GPIOInit.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
int main()
{
   GPIOInit gpio;
   if(!gpio.IsFileExist("/sys/class/gpio/gpio388")){
  
   gpio.gpio_export(gpio0_pin);
   }
   gpio.gpio_direction(gpio0_pin,OUT);
   gpio.gpio_write(gpio0_pin,1);
   if(!gpio.IsFileExist("/sys/class/gpio/gpio298")){
  
   gpio.gpio_export(gpio1_pin);
   }
   gpio.gpio_direction(gpio1_pin,OUT);
   gpio.gpio_write(gpio1_pin,1);
   while(1)
   {
       gpio.gpio_write(gpio0_pin,0);
       sleep(1);
       std::cout<<"0"<<std::endl;
       gpio.gpio_write(gpio0_pin,1);
       sleep(1);
       std::cout<<"1"<<std::endl;
   }


   //std::cout<<gpio.IsFileExist("/sys/class/gpio/gpio388")<<std::endl;
   return 0;
}
