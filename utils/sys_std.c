#include "sys_std.h"

void int_to_str(int number, char str[]) {
    if(number == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    char tmp_str[256];
    int size = 0;
    while (number != 0) {
        tmp_str[size] = '0' + number % 10; 
        number /= 10;
        size++;
    }
    str[size] = '\0';
    int counter = 0;
    while (counter < size) {
        str[counter] = tmp_str[size - counter - 1];
        counter++;
    }
}