
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
RTE_TARGET ?= x86_64-native-linuxapp-gcc
include $(RTE_SDK)/mk/rte.vars.mk

APP = delayeval
SRCS-y := main.c

CFLAGS += -O3 -Wno-unused-function
CFLAGS += $(WERROR_FLAGS)

include $(RTE_SDK)/mk/rte.extapp.mk

run:
	sudo ./build/delayeval
