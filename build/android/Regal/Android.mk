LOCAL_PATH := $(call my-dir)

# Regal should be shipped with prebult binaries, however, if people tinker, etc...
#

REGAL_FORCE_REBUILD := $(strip $(REGAL_FORCE_REBUILD))
ifndef REGAL_FORCE_REBUILD
  ifeq (,$(strip $(wildcard $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libRegal_static.a)))
    $(call __ndk_info,WARNING: Rebuilding Regal libraries from sources!)
    $(call __ndk_info,You might want to use build/android/build-regal.sh)
    $(call __ndk_info,in order to build prebuilt versions for $(TARGET_ARCH_ABI) to speed up your builds!)
    REGAL_FORCE_REBUILD := true
  endif
endif

regal_cflags := -DANDROID=1 -DREGAL_NO_PNG=1 -Werror

regal_path   := $(LOCAL_PATH)/../../..

# src_files should not include LOCAL_PATH
regal_src_files := $(wildcard $(regal_path)/src/regal/*.cpp) $(regal_path)/src/mongoose/mongoose.c $(regal_path)/src/md5/src/md5.c
regal_src_files := $(patsubst $(LOCAL_PATH)/%,%,$(regal_src_files))

regal_c_includes := $(regal_path)/include $(regal_path)/src/regal $(regal_path)/src/boost $(regal_path)/src/mongoose $(regal_path)/src/md5/include $(regal_path)/src/lookup3

regal_export_c_includes := $(regal_path)/include


ifneq ($(REGAL_FORCE_REBUILD),true)

$(call ndk_log,Using prebuilt Regal libraries)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal_static
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal_shared
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).so
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
include $(PREBUILT_SHARED_LIBRARY)

else # REGAL_FORCE_REBUILD == true

$(call ndk_log,Rebuilding Regal libraries from sources)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal_static
LOCAL_SRC_FILES := $(regal_src_files)
LOCAL_CFLAGS := $(regal_cflags)
LOCAL_C_INCLUDES := $(regal_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
LOCAL_EXPORT_LDLIBS := -llog
LOCAL_ARM_MODE  := arm
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal_shared
LOCAL_SRC_FILES := $(regal_src_files)
LOCAL_CFLAGS := $(regal_cflags)
LOCAL_C_INCLUDES := $(regal_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
LOCAL_LDLIBS := -llog
LOCAL_EXPORT_LDLIBS := -llog
LOCAL_ARM_MODE  := arm
include $(BUILD_SHARED_LIBRARY)

endif # REGAL_FORCE_REBUILD == true
