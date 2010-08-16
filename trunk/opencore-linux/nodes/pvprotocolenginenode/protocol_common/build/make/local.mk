# Get the current local path as the first operation
LOCAL_PATH := $(call get_makefile_dir)

# Clear out the variables used in the local makefiles
include $(MK)/clear.mk

TARGET := protocolenginenode_common



XINCDIRS += ../../../config/$(BUILD_ARCH)  ../../../config/linux
XINCDIRS += ../../../include ../../src ../../../base/src

SRCDIR := ../../src
INCSRCDIR := ../../src

SRCS :=	pvmf_protocol_engine_common.cpp \
	pvmf_protocol_engine_node_common.cpp 
	
HDRS := pvmf_protocol_engine_node_common.h \
	pvmf_protocol_engine_common.h \
	pvmf_protocol_engine_internal.h


include $(MK)/library.mk
