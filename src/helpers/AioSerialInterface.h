#pragma once

#include <helpers/ArduinoSerialInterface.h>
#include <helpers/stub/NullInterface.h>
#include <helpers/SerialBLEInterface.h>
#include <helpers/SerialWifiInterface.h>
#include <helpers/TransportMode.h>


/**
 * All-in-One Interface that can switch between USB, BLE, and WiFi modes on the fly. Only one mode is active
 * at a time, and the others are disabled to save power. The active mode can be switched by calling setMode()
 * or cycleMode(), or by sending a command over the current active interface (not implemented yet).
 */
class AioSerialInterface : public BaseSerialInterface
{
  NullInterface _null_interface;
  ArduinoSerialInterface _uart_interface;
  ArduinoSerialInterface _usb_interface;
  SerialBLEInterface _ble_interface;
  SerialWifiInterface _wifi_interface;

  BaseSerialInterface *_transport[Transport::Mode::_COUNT] = {
    &_null_interface,
    &_uart_interface,
    &_usb_interface,
    &_ble_interface,
    &_wifi_interface,
  };


  bool _transport_initialized[Transport::Mode::_COUNT] = { 
    true, // Only mark Transport::Mode::Off as initialized.
    false,
    false,
    false,
    false,
  }; 

  Transport::Mode _active_mode = Transport::Mode::Off; 

public:

  /**
   * Initialize the UART interface with the given Stream (e.g. Serial(0)). This method does not enable the
   * UART interface.
   */
  void beginUART(Stream &serial);

  /**
   * Initialize the USB interface with the given Stream (e.g. Serial(0)). This method does not enable the
   * USB interface.
   */
  void beginUSB(Stream &serial);

  /**
   * Initialize the BLE interface with the given node name and pin code. The node name is used for
   * advertising and can be customized per device, while the pin code is used for security and should be the
   * same across devices to allow them to connect to each other. If the pin code is zero, a default value of
   * 123456 will be used. This method does not enable the BLE.
   */
  void beginBLE(const char * prefix = BLE_NAME_PREFIX, char *node_name = NULL, uint32_t ble_pin = BLE_PIN_CODE);

  /**
   * Initialize the WiFi interface with the given SSID, password, and port. The SSID and password are used
   * to connect to the WiFi network, while the port is used for TCP communication. This method does not
   * enable the WiFi.
   */
  void beginWiFi(int tcp_port = TCP_PORT);


  /**
   * Get the currently active transport mode. This will return the enum value of the active mode.
   * e.g. Off, Serial, BLE, or WiFi.
   */
  inline Transport::Mode transportMode() const { return _active_mode; }

  /**
   * Set the active transport mode by enum value. Returns true if the mode was successfully set, or false if
   * the mode is not available. This will disable the current active interface and enable the new one.
   */
  bool setTransportMode(Transport::Mode mode);

  /**
   * Cycle to the next available transport mode in round-robin order. Returns true if the mode was
   * successfully changed, or false if no other mode is available. This will disable the current active
   * interface and enable the new one.
   */
  bool cycleTransportMode();

  /**
   * Override the BaseSerialInterface methods to forward to the currently active interface. This way, the
   * rest of the code can use AioSerialInterface as if it were a single interface, and the
   * AioSerialInterface will handle forwarding the calls to the correct underlying interface based on the
   * active mode.
   */
  virtual void enable() override;

  /**
   * Disable all interfaces. This is called internally when switching modes to ensure that only the active
   * interface is enabled, but it can also be called externally to force all interfaces to be disabled
   * (e.g. for low power mode).
   */
  virtual void disable() override;

  /**
   * Check if the currently active interface is enabled. This will return true if the active interface is
   * enabled, false otherwise.
   */
  virtual bool isEnabled() const override;

  /**
   * Check if the currently active interface is connected (e.g. if BLE is connected to a central, or if
   * WiFi is connected to an access point). This will forward to the active interface's isConnected()
   * method.
   */
  virtual bool isConnected() const override;

  /**
   * Check if the currently active interface is busy writing data. This will forward to the active
   * interface's isWriteBusy() method.
   */
  virtual bool isWriteBusy() const override;

  /**
   * Write a frame of data to the currently active interface. This will forward to the active interface's
   * writeFrame() method.
   */
  virtual size_t writeFrame(const uint8_t src[], size_t len) override;

  /**
   * Check for a received frame of data from the currently active interface. This will forward to the active
   * interface's checkRecvFrame() method.
   */
  virtual size_t checkRecvFrame(uint8_t dest[]) override;
};

