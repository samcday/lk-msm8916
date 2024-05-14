LOCAL_DIR := $(GET_LOCAL_DIR)
MODULE := $(LOCAL_DIR)

EXTRA_OBJS += $(LOCAL_DIR)/target/armv7a-none-eabi/debug/librust_test.a

$(LOCAL_DIR)/target/armv7a-none-eabi/debug/librust_test.a:
	cd lib/rust-test && cargo build --target armv7a-none-eabi

MODULE_SRCS += $(LOCAL_DIR)/link-hack.c

.PHONY: $(LOCAL_DIR)/target/armv7a-none-eabi/debug/librust_test.a

include make/module.mk
