/*
 * OpenHaystack Zephyr firmware - Track your personal Bluetooth devices via
 * Apple's Find My network
 *
 * Copyright (c) 2021 Koen Vervloesem <koen@vervloesem.eu>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _OPENHAYSTACK_H_
#define _OPENHAYSTACK_H_

#include <bluetooth/addr.h>
#include <bluetooth/bluetooth.h>

/*
 * Advertising parameters for the Offline Finding advertisement
 */
extern struct bt_le_adv_param of_adv_param;

/*
 * Template with manufacturer data of the Offline Finding advertisement
 */
extern uint8_t of_manufacturer_data[29];

/*
 * Offline Finding advertising data
 */
extern struct bt_data of_advertising_data[1];

/*
 * Set the Bluetooth MAC address based on the first six bytes of the public key.
 */
void of_set_address_from_key(bt_addr_le_t *address, char key[28]);

/*
 * Copy the remaining bytes and first two bits of the public key to the
 * manufacturer data.
 */
void of_set_manufacturer_data_from_key(uint8_t data[29], char key[28]);

#endif /* _OPENHAYSTACK_H_ */