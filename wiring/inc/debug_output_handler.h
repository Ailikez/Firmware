/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

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


#ifndef DEBUG_OUTPUT_HANDLER_H_
#define	DEBUG_OUTPUT_HANDLER_H_

#include "wiring_platform.h"
#include "wiring_usartserial.h"
#include "wiring_usbserial.h"
#include "service_debug.h"
#include "delay_hal.h"

#ifdef INTOROBOT_USB_SERIAL
struct SerialUSBDebugOutput
{
    SerialUSBDebugOutput(int baud=115200, LoggerOutputLevel level=ALL_LEVEL)
    {
        SerialUSB.begin(baud);
        set_logger_output(log_output, level);
    }

    static void log_output(const char* msg)
    {
        SerialUSB.print(msg);
    }

};
#endif

#ifndef INTOROBOT_WIRING_NO_USART_SERIAL
struct SerialDebugOutput
{
    SerialDebugOutput(int baud=115200, LoggerOutputLevel level=ALL_LEVEL)
    {
        Serial.begin(baud);
        set_logger_output(log_output, level);
    }

    static void log_output(const char* msg)
    {
        Serial.print(msg);
    }

};

#if Wiring_Serial2
struct Serial1DebugOutput
{
    Serial1DebugOutput(int baud=115200, LoggerOutputLevel level=ALL_LEVEL)
    {
        Serial1.begin(baud);
        set_logger_output(log_output, level);
    }

    static void log_output(const char* msg)
    {
        Serial1.print(msg);
    }

};
#endif
#endif

#endif	/* DEBUG_OUTPUT_HANDLER_H */

