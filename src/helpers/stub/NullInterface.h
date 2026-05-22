#pragma once

#include <helpers/BaseSerialInterface.h>

/**
 * NullInterface is a dummy implementation of BaseSerialInterface that does nothing. This is used effectively
 * disable all communication without having to check for null pointers everywhere. All methods are implemented
 * as no-ops that return false or 0 as appropriate.
 */
class NullInterface : public BaseSerialInterface {
    bool _enabled;
public:
  NullInterface() : _enabled(false) {}
  virtual void enable() override { _enabled = true; }
  virtual void disable() override { _enabled = false; }
  virtual bool isEnabled() const override { return _enabled; }
  virtual bool isConnected() const override { return false; }
  virtual bool isWriteBusy() const override { return false; }
  virtual size_t writeFrame(const uint8_t src[], size_t len) override { return len; }
  virtual size_t checkRecvFrame(uint8_t dest[]) override { return 0; }
};