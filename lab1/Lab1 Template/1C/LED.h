/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 1C
 */

#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void Red_LED_Off(void);
void Red_LED_On(void);
void Red_LED_Toggle(void);

#endif
