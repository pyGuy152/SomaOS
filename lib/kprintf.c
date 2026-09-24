#include "string.h"
#include "vga.h"
#include "kprintf.h"
#include "stdarg.h"

void kprintf(char* str, ...){
    va_list args;
    va_start(args, str);

    char buf[32];
    
    for (int i = 0; str[i] != '\0'; i++){
        if (str[i] != '%'){
            vga_put(str[i]);
            continue;
        }
        i++;

        switch (str[i]){
            case 's': {
                char *string_input = va_arg(args, char *);
                vga_write(string_input);
                break;
            }case 'c': {
                char c = (char)va_arg(args, int);
                vga_put(c);
                break;
            }case 'd': case 'i': {
                int i = va_arg(args, int);
                itoa(i,buf);
                vga_write(buf);
                break;
            }case '%': {
                vga_put('%');
                break;
            }default: {
                vga_put('%');
                vga_put(str[i]);
                break;
            }
        }
    }
    va_end(args);
}