//
// Created by oxynux on 2/24/19.
//

#include "kernel/system.h"

void reboot(void)
{
    uint8_t temp;

    asm volatile ("cli"); /* disable all interrupts */

    /* Clear all keyboard buffers (output and command buffers) */
    do
    {
        temp = inb(KBRD_INTRFC); /* empty user data */
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
            inb(KBRD_IO); /* empty keyboard data */
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);

    outb(KBRD_INTRFC, KBRD_RESET); /* pulse CPU reset line */
    loop:
    asm volatile ("hlt"); /* if that didn't work, halt the CPU */
    goto loop; /* if a NMI is received, halt again */
}