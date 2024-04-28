LOCAL_DIR := $(GET_LOCAL_DIR)

GLOBAL_INCLUDES += \
	$(LOCAL_DIR)/include

MODULE_SRCS += \
	$(LOCAL_DIR)/clock.c \
	$(LOCAL_DIR)/clock_lib2.c \
	$(LOCAL_DIR)/clock_pll.c \
	$(LOCAL_DIR)/uart.c \
	$(LOCAL_DIR)/qtimer.c \
	$(LOCAL_DIR)/qtimer_mmap.c
