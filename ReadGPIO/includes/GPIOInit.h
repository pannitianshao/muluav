#ifndef GPIOInit_H_  
#define GPIOInit_H_  
#include <stdio.h> 

#define MSG(args...) printf(args)
#define gpio0_pin 388
#define gpio1_pin 298
#define IN  0
#define OUT 1
class GPIOInit
{
public:
    GPIOInit(void);
    ~GPIOInit(void);
    //函数声明
    int gpio_export(int pin);
    int gpio_unexport(int pin);
    int gpio_direction(int pin, int dir);
    int gpio_write(int pin, int value);
    int gpio_read(int pin);
    int IsFileExist(const char* path);
    void test(void);  
    bool getflag_init(void);
private:
    bool flag_init;    
};
#endif  
