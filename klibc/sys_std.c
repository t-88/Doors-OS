#include "sys_std.h"
#include "shared.h"
#include "screen.h"

void int_to_str(int number, char str[]) {
    if(number == 0 || number == -0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    bool is_nigative = false;
    if(number < 0) {
        str[0] = '-';
        number *= -1;
        is_nigative = true;
    }

    char tmp_str[256];
    int size = 0;
    while (number != 0) {
        tmp_str[size] = '0' + number % 10; 
        number /= 10;
        size++;
    }
    str[size + is_nigative] = '\0';
    int counter = 0;
    while (counter < size) {
        str[counter + is_nigative] = tmp_str[size - counter - 1];
        counter++;
    }
}

void hex_to_str(int number, char str[]) {
    str[0] = '0';
    str[1] = 'x';
    if(number == 0) {
        str[2] = '0';
        str[3] = '\0';
        return;
    }
    
    char tmp_str[256];
    int size = 2;
    while (number != 0) {
        u8 moded = number % 16;
        if(moded > 9) {
            tmp_str[size - 2] = 'A' + (moded - 10); 
        } else {
            tmp_str[size - 2] = '0' + moded ; 
        }
        number /= 16;
        size++;

    }
    str[size] = '\0';
    int counter = 0;
    while (counter < size - 2 ) {
        str[counter + 2] = tmp_str[size - counter - 3];
        counter++;
    }   
}

int strlen(char str[]) {
    int count = 0;
    while (str[count++]) count++;
    return count - 1;
}
