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

regal_cflags := -DANDROID=1 -DREGAL_NO_PNG=1 -DREGAL_CONFIG_FILE=/data/.regal -Werror

regal_path   := $(LOCAL_PATH)/../../../..

include $(regal_path)/build/zlib.inc
include $(regal_path)/build/snappy.inc
include $(regal_path)/build/apitrace.inc
include $(regal_path)/build/regal.inc

#
# zlib
#

zlib_src_files  := $(patsubst %,$(regal_path)/%,$(ZLIB.C))
zlib_src_files  := $(patsubst $(LOCAL_PATH)/%,%,$(zlib_src_files))

zlib_c_includes := $(regal_path)/src/zlib/include
zlib_c_includes := $(patsubst $(LOCAL_PATH)/../%,%,$(zlib_c_includes))

zlib_export_c_includes := $(regal_path)/include

#
# snappy
#

snappy_src_files  := $(patsubst %,$(regal_path)/%,$(SNAPPY.CXX))
snappy_src_files  := $(patsubst $(LOCAL_PATH)/%,%,$(snappy_src_files))

snappy_c_includes := $(regal_path)/src/snappy
snappy_c_includes := $(patsubst $(LOCAL_PATH)/../%,%,$(snappy_c_includes))

snappy_export_c_includes := $(regal_path)/include

#
# apitrace
#

apitrace_src_files := $(patsubst %,$(regal_path)/%,$(APITRACE.CXX))
apitrace_src_files := $(subst glproc_gl,glproc_egl,$(apitrace_src_files))
apitrace_src_files := $(patsubst $(LOCAL_PATH)/%,%,$(apitrace_src_files))

apitrace_c_includes := $(regal_path)/include $(regal_path)/src/apitrace/common $(regal_path)/src/apitrace/gen/dispatch $(regal_path)/src/apitrace/dispatch $(regal_path)/src/apitrace/helpers $(regal_path)/src/apitrace/wrappers $(regal_path)/src/apitrace
apitrace_c_includes += $(regal_path)/src/zlib/include $(regal_path)/src/zlib/src $(regal_path)/src/snappy
apitrace_c_includes += $(regal_path)/src/apitrace/thirdparty/khronos
apitrace_c_includes += $(regal_path)/src/regal $(regal_path)/src/mongoose $(regal_path)/src/squish
apitrace_c_includes := $(patsubst $(LOCAL_PATH)/../%,%,$(apitrace_c_includes))

apitrace_export_c_includes := $(regal_path)/include

#
# regal
#

regal_src_files := $(patsubst %,$(regal_path)/%,$(REGAL.CXX))
regal_src_files += $(regal_path)/src/mongoose/mongoose.c $(regal_path)/src/md5/src/md5.c $(regal_path)/src/jsonsl/jsonsl.c
regal_src_files := $(patsubst $(LOCAL_PATH)/%,%,$(regal_src_files))

regal_c_includes := $(regal_path)/include $(regal_path)/src/regal $(regal_path)/src/boost $(regal_path)/src/mongoose $(regal_path)/src/md5/include $(regal_path)/src/lookup3 $(regal_path)/src/jsonsl
regal_c_includes := $(patsubst $(LOCAL_PATH)/../%,%,$(regal_c_includes))

regal_export_c_includes := $(regal_path)/include

ifneq ($(REGAL_FORCE_REBUILD),true)

$(call ndk_log,Using prebuilt Regal libraries)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal_static
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Regal
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).so
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
include $(PREBUILT_SHARED_LIBRARY)

else # REGAL_FORCE_REBUILD == true

$(call ndk_log,Rebuilding Regal libraries from sources)

include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := $(zlib_src_files)
LOCAL_CFLAGS := $(regal_cflags) -DHAVE_UNISTD_H=1
LOCAL_C_INCLUDES := $(zlib_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(zlib_export_c_includes)
LOCAL_EXPORT_LDLIBS :=
LOCAL_ARM_MODE  := arm
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := snappy
LOCAL_SRC_FILES := $(snappy_src_files)
LOCAL_CFLAGS := $(regal_cflags) -DHAVE_UNISTD_H=1
LOCAL_C_INCLUDES := $(snappy_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(snappy_export_c_includes)
LOCAL_EXPORT_LDLIBS :=
LOCAL_ARM_MODE  := arm
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := apitrace
LOCAL_SRC_FILES := $(apitrace_src_files)
LOCAL_CFLAGS := $(regal_cflags) -DAPITRACE_TLS=0 -DTRACE_OS_LOG=0 -DTRACE_BACKTRACE=0 -DTRACE_ENABLED_CHECK=0

LOCAL_C_INCLUDES := $(apitrace_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(apitrace_export_c_includes)
LOCAL_EXPORT_LDLIBS :=
LOCAL_ARM_MODE  := arm
include $(BUILD_STATIC_LIBRARY)

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
LOCAL_MODULE := Regal
LOCAL_SRC_FILES := $(regal_src_files)
LOCAL_CFLAGS := $(regal_cflags)
LOCAL_C_INCLUDES := $(regal_c_includes)
LOCAL_EXPORT_C_INCLUDES := $(regal_export_c_includes)
LOCAL_STATIC_LIBRARIES := apitrace zlib snappy
LOCAL_LDLIBS := -llog
LOCAL_EXPORT_LDLIBS := -llog
LOCAL_ARM_MODE  := arm
include $(BUILD_SHARED_LIBRARY)

endif # REGAL_FORCE_REBUILD == true
