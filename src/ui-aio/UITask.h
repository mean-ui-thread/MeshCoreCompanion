#pragma once

#include <MeshCore.h>
#include <helpers/ui/DisplayDriver.h>
#include <helpers/ui/UIScreen.h>
#include <helpers/SensorManager.h>
#include <helpers/BaseSerialInterface.h>
#include <Arduino.h>
#include <helpers/sensors/LPPDataHelpers.h>

#ifndef LED_STATE_ON
  #define LED_STATE_ON 1
#endif

#ifdef PIN_BUZZER
  #include <helpers/ui/buzzer.h>
#endif
#ifdef PIN_VIBRATION
  #include <helpers/ui/GenericVibration.h>
#endif

#include "AbstractUITask.h"
#include "NodePrefs.h"

#include <helpers/TransportMode.h>

class UITask : public AbstractUITask {
  DisplayDriver* _display;
  SensorManager* _sensors;
#ifdef PIN_BUZZER
  genericBuzzer buzzer;
#endif
#ifdef PIN_VIBRATION
  GenericVibration vibration;
#endif
  unsigned long _next_refresh, _auto_off;
  NodePrefs* _node_prefs;
  char _alert[80];
  unsigned long _alert_expiry;
  int _msgcount;
  unsigned long ui_started_at, next_batt_chck;
  int next_backlight_btn_check = 0;
#ifdef PIN_STATUS_LED
  int led_state = 0;
  int next_led_change = 0;
  int last_led_increment = 0;
#endif

#ifdef PIN_USER_BTN_ANA
  unsigned long _analogue_pin_read_millis = millis();
#endif

  UIScreen* splash;
  UIScreen* home;
  UIScreen* msg_preview;
  UIScreen* curr;

  void userLedHandler();

  // Button action handlers
  char checkDisplayOn(char c);
  char handleLongPress(char c);
  char handleDoubleClick(char c);
  char handleTripleClick(char c);

  void setCurrScreen(UIScreen* c);

  static Transport::Mode defaultGetTransportMode() {
    return Transport::Mode::Off;
  }
  
  // will be overridden by main.cpp and called to get the current transport mode label to put onto the display (e.g. "BLE", "USB", "WiFi")
  static const char* defaultGetTransportModeLabel()
  {
    return "Unknown";
  }

  // will be overridden by main.cpp and called to cycle between BLE/USB/WiFi modes
  static bool defaultCycleTransportMode() {
    return false;
  }

public:
  typedef Transport::Mode (*GetTransportModeFn)();
  typedef const char* (*GetTransportModeLabelFn)();
  typedef bool (*CycleTransportModeFn)();

  GetTransportModeFn getTransportMode;
  GetTransportModeLabelFn getTransportModeLabel;
  CycleTransportModeFn cycleTransportMode;

  // This is to override the default transport mode handlers with the ones from main.cpp, which have access to
  // the actual transport mode state. This allows the UITask to display the current transport mode and also to
  // trigger cycling through transport modes when the user performs a long press on the button. I know it's
  // a bit hacky to have the UITask depend on functions defined in main.cpp, but it avoids having to pass
  // transport mode state around everywhere.
  void setTransportModeHandlers(GetTransportModeFn get_mode,GetTransportModeLabelFn get_label, CycleTransportModeFn cycle_mode) {
    getTransportMode = get_mode ? get_mode : []() { return Transport::Mode::Off; };
    getTransportModeLabel = get_label ? get_label : defaultGetTransportModeLabel;
    cycleTransportMode = cycle_mode ? cycle_mode : defaultCycleTransportMode;
  }

  UITask(mesh::MainBoard* board, BaseSerialInterface* serial) : AbstractUITask(board, serial), _display(NULL), _sensors(NULL) {
    next_batt_chck = _next_refresh = 0;
    ui_started_at = 0;
    curr = NULL;
    getTransportMode = defaultGetTransportMode;
    getTransportModeLabel = defaultGetTransportModeLabel;
    cycleTransportMode = defaultCycleTransportMode;
  }
  void begin(DisplayDriver* display, SensorManager* sensors, NodePrefs* node_prefs);

  void gotoHomeScreen() { setCurrScreen(home); }
  void showAlert(const char* text, int duration_millis);
  int  getMsgCount() const { return _msgcount; }
  bool hasDisplay() const { return _display != NULL; }
  bool isButtonPressed() const;

  bool isBuzzerQuiet() { 
#ifdef PIN_BUZZER
    return buzzer.isQuiet();
#else
    return true;
#endif
  }

  void toggleBuzzer();
  bool getGPSState();
  void toggleGPS();


  // from AbstractUITask
  void msgRead(int msgcount) override;
  void newMsg(uint8_t path_len, const char* from_name, const char* text, int msgcount) override;
  void notify(UIEventType t = UIEventType::none) override;
  void loop() override;

  void shutdown(bool restart = false);
};
