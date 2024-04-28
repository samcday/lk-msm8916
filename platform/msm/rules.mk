LOCAL_DIR := $(GET_LOCAL_DIR)

LINKER_SCRIPT += $(BUILDDIR)/system-onesegment.ld

GLOBAL_INCLUDES += \
	$(LOCAL_DIR)/common/include

include $(LOCAL_DIR)/$(SUB_PLATFORM)/rules.mk
