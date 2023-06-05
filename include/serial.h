#ifndef __SERIAL_H__
#define __SERIAL_H__
void putchar(char c);
void print(const char *s);
char getc();
void gets(char* buffer, int max_length);
void process_command();
#endif