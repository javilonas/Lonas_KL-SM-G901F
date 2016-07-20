ifeq ($(TARGET_BOARD_PLATFORM),apq8084)

DLKM_DIR   := device/qcom/common/dlkm
LOCAL_PATH := $(call my-dir)
# This makefile is only for DLKM
ifneq ($(findstring vendor,$(LOCAL_PATH)),)

include $(CLEAR_VARS)
LOCAL_MODULE      := mhi.ko
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/modules/
include $(DLKM_DIR)/AndroidKernelModule.mk
endif
endif
