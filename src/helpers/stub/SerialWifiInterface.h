#pragma once

#include <helpers/stub/NullInterface.h>

/**
 * Stub implementation of SerialWiFiInterface that does nothing, for platforms that don't support WiFi. This
 * allows the AioSerialInterface code to compile and run on platforms without WiFi support, without needing
 * #ifdefs everywhere in the codebase to check for WiFi support.
 */
class SerialWiFiInterface : public NullInterface
{
public:
    void begin(int port) {}
};
