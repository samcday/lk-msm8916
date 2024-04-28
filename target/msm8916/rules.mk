LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

PLATFORM := msm
SUB_PLATFORM := msm8916

MODULE_SRCS += \
	$(LOCAL_DIR)/init.c

include make/module.mk
