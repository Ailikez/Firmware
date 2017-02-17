TARGET_ESP32_SDK=$(PLATFORM_MCU_PATH)/sdk

INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/config
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/bluedroid
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/bt
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/driver
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/esp32
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/ethernet
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/fatfs
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/freertos
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/log
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/mdns
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/mbedtls
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/mbedtls_port
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/vfs
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/ulp
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/newlib
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/nvs_flash
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/spi_flash
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/sdmmc
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/openssl
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/app_update
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/tcpip_adapter
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/xtensa-debug-module
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/coap
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/wpa_supplicant
<<<<<<< 995b7a22b99de56315fe8c967b822e5dd72d10bb
# INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/xtensa-debug-module

LDFLAGS += -L$(TARGET_ESP32_SDK)/lib -L$(TARGET_ESP32_SDK)/ld -nostdlib -T esp32_out.ld -T esp32.common.ld -T esp32.rom.ld -T esp32.peripherals.ld -u call_user_start_cpu0 -Wl,--gc-sections -Wl,-static -Wl,--undefined=uxTopUsedPriority

LIBS += gcc m app_update bt btdm_app c_nano coap coexist core cxx driver esp32 ethernet expat freertos g halhal json log lwip m mbedtls mdns micro-ecc net80211 nghttp nvs_flash openssl phy pp rtc sdmmc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps
#LIBS += gcc m app_update bt btdm_app coexist core driver esp32 ethernet expat freertos g halhal json log lwip m mbedtls net80211 newlib nghttp nvs_flash openssl phy pp rtc smartconfig spi_flash tcpip_adapter ulp vfs wpa wpa2 wpa_supplicant wps xtensa-debug-module
=======
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/expat
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/json
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/nghttp
INCLUDE_DIRS += $(TARGET_ESP32_SDK)/include/lwip
>>>>>>> 1. fig 更新 esp32 SDK库至 v2.0. app能正常编译成功  bootloader还编译不成功

TARGET_INTOROBOT_PATH = $(PLATFORM_MCU_PATH)/IntoRobot_Firmware_Driver
INCLUDE_DIRS += $(TARGET_INTOROBOT_PATH)/inc

LIB_DIRS += $(TARGET_ESP32_SDK)/lib
LIBS +=
