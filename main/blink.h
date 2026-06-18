#ifndef BLINK_H
#define BLINK_H

#include <stdint.h>

void blink_init(void);
void blink_set_period(uint32_t period_ms);

#endif