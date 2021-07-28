/*
 * OpenHaystack Zephyr firmware - Track your personal Bluetooth devices via
 * Apple's Find My network
 *
 * Copyright (c) 2021 Koen Vervloesem <koen@vervloesem.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include <bluetooth/addr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <zephyr/types.h>

#include "openhaystack.h"

#ifdef CONFIG_USB_UART_CONSOLE
#include "debug.h"
#endif

// Don't make `const`, so we can replace the key in the compiled bin file
static char public_key[28] = "OFFLINEFINDINGPUBLICKEYHERE!";

static bt_addr_le_t device_address = { BT_ADDR_LE_RANDOM,
				       { 0xFF, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF } };

void main(void)
{
	int err, bt_id;

	printk("Starting OpenHaystack firmware...\n");

	// Set advertisement data
	of_set_manufacturer_data_from_key(of_manufacturer_data, public_key);

	// Set default address
	of_set_address_from_key(&device_address, public_key);
	bt_id = bt_id_create(&device_address, NULL);
	if (bt_id < 0) {
		printk("Can't create new identity (err %d)\n", bt_id);
		return;
	} else {
		printk("Created new identity %d\n", bt_id);
	}

	// Initialize the Bluetooth subsystem
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	printk("Bluetooth initialized\n");

	// Start advertising
	of_adv_param.id = 0;
	err = bt_le_adv_start(&of_adv_param, of_advertising_data, ARRAY_SIZE(of_advertising_data), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

}

#ifdef CONFIG_USB_UART_CONSOLE
// Task for starting up the USB console
K_THREAD_DEFINE(console_id, STACKSIZE, console_init, NULL, NULL, NULL, PRIORITY - 2, 0, 0);
#endif
