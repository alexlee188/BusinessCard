#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define NEWLINE "\r\n"

void console_init(unsigned char priority);
void _ISR __attribute ((auto_psv)) _T4Interrupt(void);

#endif
