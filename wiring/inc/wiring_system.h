/**
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */


#ifndef WIRING_SYSTEM_H__
#define WIRING_SYSTEM_H__

#include "wiring_ticks.h"
#include "wiring_string.h"
#include "wiring_product.h"
#include "system_mode.h"
#include "system_update.h"
#include "system_sleep.h"
#include "system_cloud.h"
#include "system_event.h"
#include "core_hal.h"
#include "interrupts_hal.h"
#include "core_hal.h"
#include "system_user.h"
#include "system_config.h"

#ifdef INTOROBOT_PLATFORM
#define SYSTEM_HW_TICKS 1
#else
#define SYSTEM_HW_TICKS 0
#endif

#if SYSTEM_HW_TICKS
#include "hw_ticks.h"
#endif

class Stream;

class SystemClass {
public:

    SystemClass(System_Mode_TypeDef mode = DEFAULT) {
        set_system_mode(mode);
    }

    static System_Mode_TypeDef mode(void) {
        return system_mode();
    }

    static bool firmwareUpdate(Stream *serialObj) {
        //return system_firmwareUpdate(serialObj);
        return true;
    }

    static void factoryReset(void);
    static void dfu(bool persist=false);
    static void reset(void);

    static void enterSafeMode(void) {
        HAL_Core_Enter_Safe_Mode(NULL);
    }

#if SYSTEM_HW_TICKS
    static inline uint32_t ticksPerMicrosecond()
    {
        return SYSTEM_US_TICKS;
    }

    static inline uint32_t ticks()
    {
        return SYSTEM_TICK_COUNTER;
    }

    static inline void ticksDelay(uint32_t duration)
    {
        uint32_t start = ticks();
        while ((ticks()-start)<duration) {}
    }
#endif

    static void sleep(IntoRobot_Sleep_TypeDef sleepMode, long seconds=0);
    static void sleep(long seconds) { sleep(SLEEP_MODE_WLAN, seconds); }
    static void sleep(uint16_t wakeUpPin, InterruptMode edgeTriggerMode, long seconds=0);
    static String deviceID(void) { return intorobot_deviceID(); }

    static uint16_t buttonPushed(uint8_t button=0) {
        return system_button_pushed_duration(button, NULL);
    }

    //system event
    static bool on(system_event_t events, void(*handler)(system_event_t event, int param, uint8_t *data, uint16_t datalen)) {
        return !system_subscribe_event(events, reinterpret_cast<system_event_handler_t*>(handler), nullptr);
    }

    static bool on(system_event_t events, void(*handler)(system_event_t event, int param)) {
        return system_subscribe_event(events, reinterpret_cast<system_event_handler_t*>(handler), NULL);
    }

    static bool on(system_event_t events, void(*handler)(system_event_t event)) {
        return system_subscribe_event(events, reinterpret_cast<system_event_handler_t*>(handler), NULL);
    }

    static bool on(system_event_t events, void(*handler)()) {
        return system_subscribe_event(events, reinterpret_cast<system_event_handler_t*>(handler), NULL);
    }

    static void off(void(*handler)(system_event_t event, int param, uint8_t *data, uint16_t datalen)) {
        system_unsubscribe_event(event_all, handler, nullptr);
    }

    static void off(system_event_t events, void(*handler)(system_event_t event, int param, uint8_t *data, uint16_t datalen)) {
        system_unsubscribe_event(events, handler, nullptr);
    }

    static uint32_t freeMemory();

    template<typename Condition, typename While> static bool waitConditionWhile(Condition _condition, While _while) {
        while (_while() && !_condition()) {
            intorobot_process();
        }
        return _condition();
    }

    template<typename Condition> static bool waitCondition(Condition _condition) {
        return waitConditionWhile(_condition, []{ return true; });
    }

    template<typename Condition> static bool waitCondition(Condition _condition, system_tick_t timeout) {
        const system_tick_t start = millis();
        return waitConditionWhile(_condition, [=]{ return (millis()-start)<timeout; });
    }
/*
    bool set(hal_system_config_t config_type, const void* data, unsigned length)
    {
        return HAL_Set_System_Config(config_type, data, length)>=0;
    }

    bool set(hal_system_config_t config_type, const char* data)
    {
        return set(config_type, data, strlen(data));
    }

    inline bool featureEnabled(HAL_Feature feature)
    {
        return HAL_Feature_Get(feature);
    }

    inline int enableFeature(HAL_Feature feature)
    {
        int result = HAL_Feature_Set(feature, true);
        if (feature==FEATURE_RETAINED_MEMORY && !HAL_Feature_Get(FEATURE_WARM_START)) {
            system_initialize_user_backup_ram();
        }
        return result;
    }

    inline int disableFeature(HAL_Feature feature)
    {
        return HAL_Feature_Set(feature, false);
    }
*/
#ifdef configSETUP_ENABLE
    void configBegin(system_config_type_t config_type)
    {
        set_system_config_type(config_type);
    }

    void configEnd(void)
    {
        set_system_config_type(SYSTEM_CONFIG_TYPE_NONE);
    }

    system_config_type_t configStatus(void)
    {
        return get_system_config_type();
    }

    void configProcess(void)
    {
        system_config_process();
    }
#endif
};

extern SystemClass System;

#define SYSTEM_MODE(mode)  SystemClass SystemMode(mode);
#define SYSTEM_THREAD(state) STARTUP(system_thread_set_state(intorobot::feature::state, NULL));
#define SYSTEM_CONFIG_MODE(mode) STARTUP(set_system_config_mode(mode));

#define waitFor(condition, timeout) System.waitCondition([]{ return (condition)(); }, (timeout))
#define waitUntil(condition) System.waitCondition([]{ return (condition)(); })

#endif  /* WIRING_SYSTEM_H__ */

