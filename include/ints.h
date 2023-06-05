#ifndef __VIC_H__
#define __VIC_H__
#include <stdint.h>
void handle_interrupt(uint64_t interrupt_number);
void timer_interrupt_handler();
void device_interrupt_handler();
void exception_handler();
void disable_interrupts();
void enable_interrupts();
void config_interrupts();
uint64_t check_interrupt();
void init_timer();
#endif
