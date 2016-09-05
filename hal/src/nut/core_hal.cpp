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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hw_config.h"
#include "core_hal.h"
#include "rng_hal.h"
#include "ui_hal.h"
#include "bootloader.h"
#include "gpio_hal.h"
#include "interrupts_hal.h"
#include "syshealth_hal.h"
#include "intorobot_macros.h"
#include "rtc_hal.h"
#include "service_debug.h"
#include "delay_hal.h"
#include "params_hal.h"
#include "memory_hal.h"
#include <Arduino.h>
#include "Schedule.h"
#include "esp8266_wifi_generic.h"
#include <core_version.h>
#include "subsys_version.h"
#include "flash_map.h"
#include "uart.h"



/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
void HAL_Core_Setup(void);

/* Private macro ------------------------------------------------------------*/
#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_SIZE    1
#define OPTIMISTIC_YIELD_TIME_US 16000

/* Private variables --------------------------------------------------------*/
struct rst_info resetInfo;

extern "C" void system_loop_handler(uint32_t interval_us);

/* Extern variables ----------------------------------------------------------*/
int atexit(void (*func)()) {
    return 0;
}

extern "C" void ets_update_cpu_frequency(int freqmhz);
void preloop_update_frequency() __attribute__((weak));
void preloop_update_frequency() {
#if defined(F_CPU) && (F_CPU == 160000000L)
    REG_SET_BIT(0x3ff00014, BIT(0));
    ets_update_cpu_frequency(160);
#endif
}

extern void (*__init_array_start)(void);
extern void (*__init_array_end)(void);

extern "C" void __gdb_init() {}
extern "C" void gdb_init(void) __attribute__ ((weak, alias("__gdb_init")));

extern "C" void __gdb_do_break(){}
extern "C" void gdb_do_break(void) __attribute__ ((weak, alias("__gdb_do_break")));

cont_t g_cont __attribute__ ((aligned (16)));
static os_event_t g_loop_queue[LOOP_QUEUE_SIZE];

static uint32_t g_micros_at_task_start;
static uint32_t g_micros_at_system_loop_start;

extern "C" void esp_yield() {
    if (cont_can_yield(&g_cont))
    {
        cont_yield(&g_cont);
    }
}

extern "C" void esp_schedule() {
    ets_post(LOOP_TASK_PRIORITY, 0, 0);
}

extern "C" void __yield() {
    if (cont_can_yield(&g_cont)) {
        esp_schedule();
        esp_yield();
    }
    else {
        //panic();
    }
}

extern "C" void yield(void) __attribute__ ((weak, alias("__yield")));

extern "C" void optimistic_yield(uint32_t interval_us) {
    if (cont_can_yield(&g_cont) &&
        (system_get_time() - g_micros_at_task_start) > interval_us)
    {
        g_micros_at_task_start = system_get_time();
        yield();
    }
}

static void loop_wrapper() {
    preloop_update_frequency();
    system_loop_handler(100);
    app_loop();
    run_scheduled_functions();
    esp_schedule();
}

static void loop_task(os_event_t *events) {
    g_micros_at_task_start = system_get_time();
    cont_run(&g_cont, &loop_wrapper);
    if (cont_check(&g_cont) != 0) {
       // panic();
    }
}

static void do_global_ctors(void) {
    void (**p)(void) = &__init_array_end;
    while (p != &__init_array_start)
        (*--p)();
}

extern "C" const char intorobot_subsys_version[32] __attribute__((section(".subsys.version"))) = SUBSYS_VERSION ;
void init_done() {
    gdb_init();
    do_global_ctors();
    printf("\n%08x\n", intorobot_subsys_version);
    HAL_Core_Config();
    HAL_Core_Setup();
    app_setup_and_loop_initial();
    esp_schedule();
}

extern "C" void user_init(void) {
    struct rst_info *rtc_info_ptr = system_get_rst_info();
    memcpy((void *) &resetInfo, (void *) rtc_info_ptr, sizeof(resetInfo));
    uart_div_modify(0, UART_CLK_FREQ / (115200));

    SysTick_Enable();

    cont_init(&g_cont);
    ets_task(loop_task, LOOP_TASK_PRIORITY, g_loop_queue, LOOP_QUEUE_SIZE);
    system_init_done_cb(&init_done);
}

void HAL_Core_Init(void)
{

}

void System_Loop_Handler(void* arg);
void SysTick_Handler(void* arg);
static os_timer_t systick_timer;
static os_timer_t system_loop_timer;

void HAL_Core_Config(void)
{
    //滴答定时器  //处理三色灯和模式处理
    os_timer_setfn(&systick_timer, (os_timer_func_t*)&SysTick_Handler, 0);
    os_timer_arm(&systick_timer, 1, 1);

    //Disable UART interrupts
    system_set_os_print(0);
    U0IE = 0;
    U1IE = 0;

    //Wiring pins default to inputs
    for (pin_t pin=D0; pin<=D6; pin++)
    {
        HAL_Pin_Mode(pin, INPUT);
    }

    HAL_RTC_Initial();
    HAL_RNG_Initial();

    HAL_IWDG_Initial();
    HAL_UI_Initial();
}

void HAL_Core_Setup(void)
{
    esp8266_setMode(WIFI_STA);
    esp8266_setDHCP(true);
    esp8266_setAutoConnect(false);
    esp8266_setAutoReconnect(true);
    HAL_IWDG_Config(DISABLE);
    bootloader_update_if_needed();
}

extern "C" void __real_system_restart_local();
void HAL_Core_System_Reset(void)
{
    HAL_Core_Write_Backup_Register(BKP_DR_03, 0x7DEA);
    __real_system_restart_local();
}

void HAL_Core_Enter_DFU_Mode(bool persist)
{
}

void HAL_Core_Enter_Config_Mode(void)
{
    HAL_PARAMS_Set_System_config_flag(!HAL_PARAMS_Get_System_config_flag());
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}

void HAL_Core_Enter_Firmware_Recovery_Mode(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(1);
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}

void HAL_Core_Enter_Com_Mode(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(2);
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}
/**
 * 恢复出厂设置 不清除密钥
 */

void HAL_Core_Enter_Factory_Reset_Mode(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(3);
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}

void HAL_Core_Enter_Ota_Update_Mode(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(4);
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}

/**
 * 恢复出厂设置 清除密钥
 */
void HAL_Core_Enter_Factory_All_Reset_Mode(void)
{
    HAL_PARAMS_Set_Boot_boot_flag(5);
    HAL_PARAMS_Save_Params();
    HAL_Core_System_Reset();
}

void HAL_Core_Enter_Safe_Mode(void* reserved)
{
}

void HAL_Core_Enter_Bootloader(bool persist)
{
}

uint16_t HAL_Core_Get_Subsys_Version(char* buffer, uint16_t len)
{
    char data[32];
    uint16_t templen;

    if (buffer!=NULL && len>0) {
        HAL_FLASH_Interminal_Read(SUBSYS_VERSION_ADDR, (uint32_t *)data, sizeof(data));
        if(!memcmp(data, "VERSION:", 8))
        {
            templen = MIN(strlen(&data[8]), len-1);
            memset(buffer, 0, len);
            memcpy(buffer, &data[8], templen);
            return templen;
        }
    }
    return 0;
}

typedef void (*app_system_loop_handler)(void);
app_system_loop_handler APP_System_Loop_Handler = NULL;

void HAL_Core_Set_System_Loop_Handler(void (*handler)(void))
{
    APP_System_Loop_Handler = handler;
}

static uint32_t g_at_system_loop = false;
void system_loop_handler(uint32_t interval_us)
{
    if( true == g_at_system_loop )
        return;

    if ((system_get_time() - g_micros_at_system_loop_start) > interval_us) {
        if (NULL != APP_System_Loop_Handler) {
            g_at_system_loop = true;
            APP_System_Loop_Handler();
            g_at_system_loop = false;
        }
        g_micros_at_system_loop_start = system_get_time();
    }
}

void SysTick_Handler(void* arg)
{
    HAL_SysTick_Handler();
    HAL_UI_SysTick_Handler();
}

void HAL_Core_System_Loop(void)
{
    optimistic_yield(100);
    system_loop_handler(100);
}


