/**
 * Application Globals - Header File
 * 
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_APP_H
#define SOWACO_APP_H

// General application information
#define APP_NAME "SoWaCo Device Server Application"
#define APP_VERSION "0.0.2"
#define APP_AUTHOR "Tibor Buzási"

// Bluetooth configuration
#define BT_ENCRYPTION ESP_BLE_SEC_ENCRYPT_MITM
#define BT_PERMISSION_READ ESP_GATT_PERM_READ_ENC_MITM
#define BT_PERMISSION_WRITE ESP_GATT_PERM_WRITE_ENC_MITM

// Factory device values
#define DEVICE_NAME "SoWaCo Device"
#define DEVICE_PASSKEY 975321
#define OPERATION_MODE 0
#define OPERATION_START_TIME 4
#define OPERATION_END_TIME 22
#define GOAL_LEVEL 75
#define GOAL_TEMPERATURE 40
#define GOAL_TIME 18
#define PIPE_DRAINING_TIME 30
#define TANK_DRAINING_TIME 120

#endif // SOWACO_APP_H