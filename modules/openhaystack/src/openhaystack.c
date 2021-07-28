/*
 * OpenHaystack Zephyr firmware - Track your personal Bluetooth devices via
 * Apple's Find My network
 *
 * Copyright (c) 2021 Koen Vervloesem <koen@vervloesem.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#include "openhaystack.h"

/*
 * Advertising parameters for the Offline Finding advertisement
 */
struct bt_le_adv_param of_adv_param = {
	.id = 1, // Don't use 0 because Zephyr doesn't allow resetting the default identity
	.sid = 0,
	.secondary_max_skip = 0,
	.options = BT_LE_ADV_OPT_USE_IDENTITY,
	// Offline Finding specification requires advertising interval of 2 seconds
	.interval_min = 2 * BT_GAP_ADV_SLOW_INT_MIN,
	.interval_max = 2 * BT_GAP_ADV_SLOW_INT_MIN,
	.peer = NULL,
};

/*
 * Template with manufacturer data of the Offline Finding advertisement
 */
uint8_t of_manufacturer_data[] = {
	0x4c, 0x00,                                     // Company ID (Apple)
	0x12,                                           // Offline Finding type
	0x19,                                           // Offline Finding data length in bytes
	0x00,                                           // Status (e.g. battery level)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Public key bytes 6..27
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,   // First two bits of public key
	0x00    // Hint (0x00 on iOS reports)
};

/*
 * Advertising data
 */
struct bt_data of_advertising_data[] = { BT_DATA(BT_DATA_MANUFACTURER_DATA, of_manufacturer_data,
						 sizeof(of_manufacturer_data)) };

/*
 * Set the Bluetooth MAC address based on the first six bytes of the public key.
 */
void of_set_address_from_key(bt_addr_le_t *address, char key[28])
{
	// The first two bits of a random static address are always 0b11.
	address->a.val[5] = key[0] | 0b11000000;
	address->a.val[4] = key[1];
	address->a.val[3] = key[2];
	address->a.val[2] = key[3];
	address->a.val[1] = key[4];
	address->a.val[0] = key[5];
}

/*
 * Copy the remaining bytes and first two bits of the public key to the
 * manufacturer data.
 */
void of_set_manufacturer_data_from_key(uint8_t data[29], char key[28])
{
	// Copy last 22 bytes of public key
	// Public key:       0123456789012345678901234567
	//                         ||||||||||||||||||||||
	//                         vvvvvvvvvvvvvvvvvvvvvv
	// Manufacturer data: 01234567890123456789012345678
	memcpy(&data[5], &key[6], 22);
	// Append first two bits of public key
	data[27] = key[0] >> 6;
}
