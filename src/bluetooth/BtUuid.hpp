/**
 * Bluetooth Communication UUIDs - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_BTUUID_H
#define SOWACO_BTUUID_H

 // GATT Descriptor UUIDs
#define DESCRIPTOR_USER_DESCRIPTION "00002901-0000-1000-8000-00805F9B34FB"

// Sensors' service and characteristics UUIDs
#define SERVICE_SENSORS "8f85445c-76ae-4e3a-880e-48e7ba63e1b2"
#define CHAR_COLLECTOR_TOP_TEMPERATURE "995de86a-0e91-4404-b1d2-4f0784349064"
#define CHAR_COLLECTOR_MIDDLE_TEMPERATURE "a5932b9c-ad36-4f6d-a74c-6954a0a5cc32"
#define CHAR_COLLECTOR_BOTTOM_TEMPERATURE "febfb46e-ffdc-4cb4-b473-3bb396ce0020"
#define CHAR_COLLECTOR_AVERAGE_TEMPERATURE "11fc7921-fbde-4a32-b1e5-64eb75ca5634"
#define CHAR_TANK_TEMPERATURE "0ea92e64-6767-4de9-98ba-23e9bc79d1b2"
#define CHAR_TANK_LEVEL_DELAY "9e4f9ed0-ce5c-4b69-b18f-bf6b363891d8"
#define CHAR_TANK_LEVEL_DISTANCE "55a917ad-bca6-4a27-8ae3-506e7dffcaac"
#define CHAR_TANK_LEVEL_PERCENT "1866a67d-d3f9-4172-b07c-1ec5ede9e35c"

// Actuators' service and characteristics UUIDs
#define SERVICE_ACTUATORS "04e2b7db-c601-43a7-ba86-7be61f6716ef"
#define CHAR_COLLECTOR_VALVE "1996ae24-580f-4894-b338-d93a4b119a00"
#define CHAR_TANK_VALVE "1f101bcb-0b10-4c55-9d8a-e603aabf0574"
#define CHAR_DRAIN_VALVE "83df1fb3-89b0-4457-8cf2-bd93c865c394"
#define CHAR_TANK_DRAIN_VALVE "5a4e5c52-172f-46b2-9cf8-2fa54ece3f93"

// Device settings' service and characteristics UUIDs
#define SERVICE_DEVICE_SETTINGS "d294d762-7d5c-4cfe-be92-58990524f9c6"
#define CHAR_DEVICE_NAME "491a6094-964b-4cc6-ba9f-e1d6bd5d1939"
#define CHAR_DEVICE_PASSKEY "64b1d562-f05d-4201-9778-b2c9a91daa3f"
#define CHAR_DEVICE_APPLY_NEW_PLAN "2656ba39-9a9f-4f14-a791-8e4e34129406"
#define CHAR_DEVICE_RESTART "b98c79ab-0d2a-478a-abf8-fc92aa47c69c"
#define CHAR_DEVICE_RESET "c9632fca-a6d7-48bd-b453-8951f4566044"
#define CHAR_DEVICE_TIME "2a2dadbd-371b-4770-afa2-1792b53b65d8"
#define CHAR_AUTOMATION_MODE "6b01c145-b064-44bf-93e5-7828d8e711ea"
#define CHAR_PROCESS_STATE "c370c906-075d-4761-b9b5-2f807fbf2789"

// Process settings' service and characteristics UUIDs
#define SERVICE_PROCESS_SETTINGS "ba1e001e-a6e8-47a5-87e5-c26b6cee09ef"
#define CHAR_OPERATION_MODE "6c8a4f74-f676-4b73-a933-12b0f5b73fe9"
#define CHAR_OPERATION_START_TIME "bcae4243-9dca-488c-94fa-aa54f2e97765"
#define CHAR_OPERATION_END_TIME "6db02b00-7899-4aa1-b9f8-723a34016fae"
#define CHAR_GOAL_LEVEL "e027ce13-d32e-4042-8a0b-ee47f38bcefa"
#define CHAR_GOAL_TEMPERATURE "9bcadac5-42fe-4243-87fe-5930fcdc37e3"
#define CHAR_GOAL_TIME "cb97e95a-429c-4b18-bfe8-df8b576a0571"
#define CHAR_PIPE_DRAINING_TIME "6a6cc24d-4664-4df0-b7f7-983a5061b444"
#define CHAR_TANK_DRAINING_TIME "e11e3453-0544-41b1-909a-e4bc58e64963"

#endif // SOWACO_BTUUID_H