#pragma once

namespace Transport
{
  /**
   * TransportMode defines the different communication modes that the AioSerialInterface can use. This is used
   * to track the current active mode and to cycle between modes when the user presses the mode button. The
   * modes are defined as an enum class for better type safety, and the AioSerialInterface class provides
   * methods to get the current mode, cycle to the next mode, and get a human-readable label for the current
   * mode to display.
   */
  enum Mode
  {
    Off = 0,
    UART,
    USB,
    BLE,
    WIFI,
    _COUNT
  };

  /**
   * Returns a human-readable string label for the given TransportMode enum value. This is used for displaying
   * the current transport mode on the UI. The labels are based on the enum values.
   */
  static const char* modeToStr(Mode mode) {
    switch (mode) {
      case Mode::Off: return "Off";
      case Mode::UART: return "UART";
      case Mode::USB: return "USB";
      case Mode::BLE: return "BLE";
      case Mode::WIFI: return "WiFi";
      default: return "???";
    }
  }
}
