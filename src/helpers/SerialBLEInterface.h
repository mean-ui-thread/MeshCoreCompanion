#pragma once

#include <helpers/PlatformSupport.h>

#if defined(ESP32_PLATFORM)

  #if SUPPORT_BLE
    #include <helpers/esp32/SerialBLEInterface.h>
  #endif

#elif defined(RP2040_PLATFORM)

  // RP2040 does not have built-in BLE support or built-in WiFi support, but we can still support external
  // BLE/WiFi modules over SPI/UART, but the user needs to explicitly define SUPPORT_BLE and/or SUPPORT_WIFI
  // in their platformio.ini to enable the code for those interfaces.

  // User will need to implement their own SerialBLEInterface that implements the BaseSerialInterface methods
  // for their specific BLE module, and then include it here.

#elif defined(NRF52_PLATFORM)

  #if SUPPORT_BLE
    #include <helpers/nrf52/SerialBLEInterface.h>
  #endif

#endif

#if !SUPPORT_BLE
  #include <helpers/stub/SerialBLEInterface.h>
#endif
