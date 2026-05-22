#include "AioSerialInterface.h"
#include <target.h>
#include <cstring>

void AioSerialInterface::beginUART(Stream &serial)
{
  _uart_interface.begin(serial);
  _transport_initialized[Transport::Mode::UART] = true;
}

void AioSerialInterface::beginUSB(Stream &serial)
{
  _usb_interface.begin(serial);
  _transport_initialized[Transport::Mode::USB] = true;
}

void AioSerialInterface::beginBLE(const char * prefix, char* node_name, uint32_t ble_pin)
{
  uint32_t runtime_ble_pin = ble_pin == 0 ? 123456 : ble_pin;
  _ble_interface.begin(prefix, node_name, runtime_ble_pin);
  _transport_initialized[Transport::Mode::BLE] = true;
}

void AioSerialInterface::beginWiFi(int tcp_port)
{
  _wifi_interface.begin(tcp_port);
  _transport_initialized[Transport::Mode::WIFI] = true;
}

bool AioSerialInterface::setTransportMode(Transport::Mode mode)
{
  if (mode >= Transport::Mode::_COUNT) {
    return false; // invalid mode
  }

  if (!_transport_initialized[mode]) {
    return false; // mode not initialized yet
  }

  if (_active_mode != mode) {
    _transport[_active_mode]->disable();
    _active_mode = mode;
    _transport[_active_mode]->enable();
  }
#if defined(ESP32)
  // TODO: is this 'setInhibitSleep' strictly only available for the ESP32 Board? If so, we need to
  // implement better abstraction, even if other boards do not have a sleep inhibit function.
  // TODO: Verify if we should also inhibit sleep for BLE mode, or if it's only necessary for WiFi mode.
  board.setInhibitSleep(_active_mode == Transport::Mode::WIFI);  // prevent sleep when WiFi is active
#endif
  return true;
}

bool AioSerialInterface::cycleTransportMode()
{
  const Transport::Mode first_mode = static_cast<Transport::Mode>(0);
  const Transport::Mode last_mode = static_cast<Transport::Mode>(Transport::Mode::_COUNT - 1);
  Transport::Mode mode = _active_mode;

  do {
    mode = (mode >= Transport::Mode::_COUNT - 1) ? first_mode : static_cast<Transport::Mode>(mode + 1); // grab the next one, wrap around in round-robin fashion
  } while (!_transport_initialized[mode]); // top looping until we find the next initialized mode. Please note that the "Transport::Mode::Off" mode is always initialized, so this loop will always terminate.

  return setTransportMode(mode);
}

void AioSerialInterface::enable()
{
  _transport[_active_mode]->enable();
}

void AioSerialInterface::disable()
{
  _transport[_active_mode]->disable();
}

bool AioSerialInterface::isEnabled() const
{
  return _transport[_active_mode]->isEnabled();
}

bool AioSerialInterface::isConnected() const
{
  return _transport[_active_mode]->isConnected();
}

bool AioSerialInterface::isWriteBusy() const
{
  return _transport[_active_mode]->isWriteBusy();
}

size_t AioSerialInterface::writeFrame(const uint8_t src[], size_t len)
{
  return _transport[_active_mode]->writeFrame(src, len);
}

size_t AioSerialInterface::checkRecvFrame(uint8_t dest[])
{
  return _transport[_active_mode]->checkRecvFrame(dest);
}
