#pragma once

#ifdef ESP32_PLATFORM

  #ifndef SUPPORT_BLE
    #define SUPPORT_BLE 1
  #else
    #define SUPPORT_BLE 0
  #endif

  #ifndef SUPPORT_WIFI
    #define SUPPORT_WIFI 1
  #else
    #define SUPPORT_WIFI 0
  #endif

#elif defined(RP2040_PLATFORM)

  // RP2040 does not have built-in BLE support or built-in WiFi support, but we can still support external
  // BLE/WiFi modules over SPI/UART, but the user needs to explicitly define SUPPORT_BLE and/or SUPPORT_WIFI
  // in their platformio.ini to enable the code for those interfaces.

#elif defined(NRF52_PLATFORM)

  #ifndef SUPPORT_BLE
    #define SUPPORT_BLE 1
  #else
    #define SUPPORT_BLE 0
  #endif

  // nRF52 does not have built-in WiFi support, but we can still support external WiFi modules over SPI/UART,
  // but the user needs to explicitly define SUPPORT_WIFI in their platformio.ini to enable the code for those
  // interfaces.

#endif

#if SUPPORT_BLE

  // Provide Default BLE name prefix if not defined in build configuration
  #ifndef BLE_NAME_PREFIX
    #define BLE_NAME_PREFIX "MeshCore-"
  #endif

  // Provide Default BLE pin code if not defined in build configuration
  #ifndef BLE_PIN_CODE
    #define BLE_PIN_CODE 123456
  #endif

#endif

#if SUPPORT_WIFI

  // Provide default WiFi credentials if not defined in build configuration.
  #ifndef WIFI_SSID
    #define WIFI_SSID "Guest" // default to open 'Guest' network if SSID is not provided
  #endif

  // Provide default WiFi password if not defined in build configuration.
  #ifndef WIFI_PWD
    #define WIFI_PWD "" // assume open 'Guest' network if password is not provided
  #endif

  // provide default TCP port if not defined in build configuration
  #ifndef TCP_PORT
    #define TCP_PORT 5000
  #endif

#endif