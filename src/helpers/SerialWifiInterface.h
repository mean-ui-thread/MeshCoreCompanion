#pragma once

#include <helpers/PlatformSupport.h>

#ifdef ESP32

  #if SUPPORT_WIFI
    #include <helpers/esp32/SerialWifiInterface.h>
  #endif

#elif defined(RP2040_PLATFORM)

  // RP2040 does not have built-in WiFi support, but we can still support external WiFi modules over SPI/UART,
  // but the user needs to explicitly define SUPPORT_WIFI in their platformio.ini to enable the code for those
  // interfaces.

  // User will need to implement their own SerialWiFiInterface that implements the BaseSerialInterface methods
  // for their specific WiFi module, and then include it here.

#elif defined(NRF52_PLATFORM)

  // nRF52 does not have built-in WiFi support, but we can still support external WiFi modules over SPI/UART,
  // but the user needs to explicitly define SUPPORT_WIFI in their platformio.ini to enable the code for those
  // interfaces.

  // User will need to implement their own SerialWiFiInterface that implements the BaseSerialInterface methods
  // for their specific WiFi module, and then include it here.

#endif

#if !SUPPORT_WIFI
  #include <helpers/stub/SerialWifiInterface.h>
#endif
