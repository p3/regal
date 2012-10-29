# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libdreamtorus
LOCAL_CFLAGS    := -Werror
LOCAL_ARM_MODE  := arm

SHARED_DIR      := $(LOCAL_PATH)/../../src

LOCAL_C_INCLUDES += $(SHARED_DIR)
MY_SRC_FILES := gl_code.cpp $(SHARED_DIR)/render.cpp
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(MY_SRC_FILES))
LOCAL_STATIC_LIBRARIES += Regal_static
LOCAL_CFLAGS    := -DANDROID=1
LOCAL_LDLIBS    := -llog 

#-lGLESv2 

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../../build/android)
$(call import-module, Regal)

