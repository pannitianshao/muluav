#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GPIOInit.h>
#include <fcntl.h> //define O_WRONLY and O_RDONLY

GPIOInit::GPIOInit(void)
{
    flag_init = true;
}

int GPIOInit::gpio_export(int pin)
{
    char buffer[64];
    int len;
    int fd;
 
    fd = open("/sys/class/gpio/export",O_WRONLY,S_IWGRP);//O_WRONLY 
    if (fd < 0) {
        MSG("Failed to open export for writing!You need to set the root.\n");
        return(-1);
    }
    len = snprintf(buffer, sizeof(buffer), "%d", pin);
    if (write(fd, buffer, len) < 0) {
        MSG("Failed to export gpio!");
        return -1;
    }
    close(fd);
    return 0;
}

int GPIOInit::gpio_unexport(int pin)
{
    char buffer[64];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open unexport for writing!\n");
        return -1;
    }
    len = snprintf(buffer, sizeof(buffer), "%d", pin);
    if (write(fd, buffer, len) < 0) {
        MSG("Failed to unexport gpio!");
        return -1;
    }

    close(fd);
    return 0;
}
//dir: 0-->IN, 1-->OUT
int GPIOInit::gpio_direction(int pin, int dir)
{
    static const char dir_str[] = "in\0out";
    char path[64];
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open gpio direction for writing!\n");
        return -1;
    }
    if (write(fd, &dir_str[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0) {
        MSG("Failed to set direction!\n");
        return -1;
    }
    close(fd);
    return 0;
}

//value: 0-->LOW, 1-->HIGH
int GPIOInit::gpio_write(int pin, int value)
{
    static const char values_str[] = "01";
    char path[64];
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        MSG("Failed to open gpio value for writing!\n");
        return -1;
    }
    if (write(fd, &values_str[value == 0 ? 0 : 1], 1) < 0) 
    {
        MSG("Failed to write value!\n");
        return -1;
    }
    close(fd);
    return 0;
}
int GPIOInit::gpio_read(int pin)
{
    char path[64];
    char value_str[3];
    int fd;

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        MSG("Failed to open gpio value for reading!\n");
        return -1;
    }
    if (read(fd, value_str, 3) < 0) {
        MSG("Failed to read value!\n");
        return -1;
    }
    close(fd);
    return (atoi(value_str));
}
//检查文件(所有类型,包括目录和文件)是否存在
//返回1:存在 0:不存在
int GPIOInit::IsFileExist(const char* path)
{
    return !access(path, F_OK);
}

void GPIOInit::test(void)
{
   
    printf("123");
}
bool GPIOInit::getflag_init(void)
{
    return flag_init;
}
GPIOInit::~GPIOInit(void)
{
}
