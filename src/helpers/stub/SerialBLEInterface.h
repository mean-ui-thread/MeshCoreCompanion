#pragma once

#include <helpers/stub/NullInterface.h>

/**
 * Stub implementation of SerialBLEInterface that does nothing, for platforms that don't support BLE. This
 * allows the AioSerialInterface code to compile and run on platforms without BLE support, without needing
 * #ifdefs everywhere in the codebase to check for BLE support.
 */
class SerialBLEInterface : public NullInterface
{
public:
  void begin(const char* prefix, char* name, uint32_t pin_code) {}

};

