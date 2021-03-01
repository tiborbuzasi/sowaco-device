/**
 * Main Program Procedure
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include <Arduino.h>
#include <Preferences.h>

#include "common/AppServer.hpp"
#include "common/Settings.hpp"
#include "app.hpp"

AppServer* app = nullptr;

/**
 * Print application information to console
 */
void printAppInfo()
{
  Serial.println(APP_NAME);
  Serial.print("Version ");
  Serial.println(APP_VERSION);
  Serial.print("Written by ");
  Serial.println(APP_AUTHOR);
  Serial.println();
}

/**
 * Initialise device
 */
void setup()
{
#ifdef DEBUG
  // Initialise console
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_VERBOSE);

  // Print application information to console
  printAppInfo();
#endif

  // Create and initialise application server
  app = new AppServer();
  app->init();

#ifdef DEBUG
  Serial.println("Device initialised.");
  Serial.println();
#endif
}

/**
 * Run device loop
 */
void loop()
{
  app->update();
}