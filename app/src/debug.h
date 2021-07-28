/*
 * OpenHaystack Zephyr firmware - Track your personal Bluetooth devices via
 * Apple's Find My network
 *
 * Copyright (c) 2021 Koen Vervloesem <koen@vervloesem.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <drivers/uart.h>
#include <usb/usb_device.h>

// Size of stack area used by each thread
#define STACKSIZE 1024

// Scheduling priority used by each thread
#define PRIORITY 7

void console_init(void);

#endif /* _DEBUG_H_ */