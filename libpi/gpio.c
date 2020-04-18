/*
 * write code to allow blinking using arbitrary pins.    
 * Implement:
 *	- gpio_set_output(pin) --- set GPIO <pin> as an output (vs input) pin.
 *	- gpio_set_on(pin) --- set the GPIO <pin> on.
 * 	- gpio_set_off(pin) --- set the GPIO <pin> off.
 * Use the minimal number of loads and stores to GPIO memory.  
 *
 * start.s defines a of helper functions (feel free to look at the assembly!  it's
 *  not tricky):
 *      uint32_t get32(volatile uint32_t *addr) 
 *              --- return the 32-bit value held at <addr>.
 *
 *      void put32(volatile uint32_t *addr, uint32_t v) 
 *              -- write the 32-bit quantity <v> to <addr>
 * 
 * Check-off:
 *  1. get a single LED to blink.
 *  2. attach an LED to pin 19 and another to pin 20 and blink in opposite order (i.e.,
 *     one should be on, while the other is off).   Note, if they behave weirdly, look
 *     carefully at the wording for GPIO set.
 */
#include "gpio.h"

/*
 * These routines are given by us and are in start.s
 */
// writes the 32-bit value <v> to address <addr>:   *(unsigned *)addr = v;
void put32(volatile void *addr, unsigned v);
// returns the 32-bit value at <addr>:  return *(unsigned *)addr
unsigned get32(const volatile void *addr);
// does nothing.
void nop(void);

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x3F200000
volatile unsigned *gpio_fsel0 = (void*)(GPIO_BASE + 0x00);
volatile unsigned *gpio_set0  = (void*)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (void*)(GPIO_BASE + 0x28);
volatile unsigned *gpio_lev0  = (void*)(GPIO_BASE + 0x34);

// Part 1 implement gpio_set_on, gpio_set_off, gpio_set_output

void gpio_set_function(unsigned pin, gpio_func_t function) {
    volatile unsigned* gpio_fsel = gpio_fsel0 + pin / 10;
    unsigned value = get32(gpio_fsel);
    unsigned shift = pin % 10 * 3;
    value &= ~(7 << shift);
    value = (function << shift) | value;
    put32(gpio_fsel, value);
}

// set <pin> to be an output pin.  note: fsel0, fsel1, fsel2 are contiguous in memory,
// so you can use array calculations!
void gpio_set_output(unsigned pin) {
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

// set GPIO <pin> on.
void gpio_set_on(unsigned pin) {
    // implement this
    // use gpio_set0
    volatile unsigned* gpio_set = gpio_set0 + pin / 32;
    put32(gpio_set, 1 << (pin % 32));
}

// set GPIO <pin> off
void gpio_set_off(unsigned pin) {
    // implement this
    // use gpio_clr0
    volatile unsigned* gpio_clr = gpio_clr0 + pin / 32;
    put32(gpio_clr, 1 << (pin % 32));
}

// Part 2: implement gpio_set_input and gpio_read

// set <pin> to input.
void gpio_set_input(unsigned pin) {
    gpio_set_function(pin, GPIO_FUNC_INPUT);
}

// return the value of <pin>
int gpio_read(unsigned pin) {
    unsigned v = 0;

    volatile unsigned* gpio_lev = gpio_lev0 + pin / 32;
    v = get32(gpio_lev);
    v = (v >> pin % 32) & 1;

    return v;
}

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    if(v)
       gpio_set_on(pin);
    else
       gpio_set_off(pin);
}

