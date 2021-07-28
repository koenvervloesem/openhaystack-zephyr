/*
 * OpenHaystack Zephyr firmware - Track your personal Bluetooth devices via
 * Apple's Find My network
 *
 * Copyright (c) 2021 Koen Vervloesem <koen@vervloesem.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "debug.h"

void console_init(void)
{
	struct device *dev = device_get_binding(CONFIG_UART_CONSOLE_ON_DEV_NAME);
	uint32_t dtr = 0;

	if (usb_enable(NULL)) {
		return;
	}

	/* Poll if the DTR flag was set, optional */
	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		k_msleep(250); // Let other tasks run if no terminal is connected to USB
	}

	if (strlen(CONFIG_UART_CONSOLE_ON_DEV_NAME) != strlen("CDC_ACM_0") ||
	    strncmp(CONFIG_UART_CONSOLE_ON_DEV_NAME, "CDC_ACM_0",   strlen(CONFIG_UART_CONSOLE_ON_DEV_NAME))) {
		printk("Error: Console device name is not USB ACM\n");

		return;
	}

	while (1) {
		k_msleep(20000);
	}
}