MODULE=user
USER_MODULE_PATH=.

BUILD_PATH_EXT = $(USER_BUILD_PATH_EXT)

DEPENDENCIES = wiring wiring_ex system services communication hal

include ../build/platform-id.mk

ifeq ("","$(INTOROBOT_NO_PLATFORM)")
DEPENDENCIES += platform
CFLAGS += -DINTOROBOT_PLATFORM
endif

WARNINGS_AS_ERRORS ?= n

# Target this makefile is building.
TARGET_TYPE=a

include ../build/arm-tlm.mk
