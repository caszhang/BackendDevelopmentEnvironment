EXECUTABLE := ../bin/service_foxea
LIBS := jsoncpp high_light tinyxml ssl crypto event util treeconf glog os dsalgo pthread 
BASE_PATH := ../../base
THIRDPARTY_PATH := ../../thirdparty
INCS := . ../include $(BASE_PATH) $(THIRDPARTY_PATH) $(THIRDPARTY_PATH)/glog-0.3.3/include $(THIRDPARTY_PATH)/treeconf $(THIRDPARTY_PATH)/libevent-2.0.21-stable/include $(THIRDPARTY_PATH)/openssl-1.0.1e/include $(THIRDPARTY_PATH)/tinyxml2-master $(THIRDPARTY_PATH)/jsoncpp-src-0.5.0/include
USER_MARCOS := TIXML_USE_STL _DEBUG 
LIBPATHS := ../lib $(BASE_PATH)/lib $(THIRDPARTY_PATH)/glog-0.3.3/lib $(THIRDPARTY_PATH)/treeconf $(THIRDPARTY_PATH)/libevent-2.0.21-stable/lib $(THIRDPARTY_PATH)/openssl-1.0.1e/lib $(THIRDPARTY_PATH)/tinyxml2-master/lib  $(THIRDPARTY_PATH)/jsoncpp-src-0.5.0/libs
WARNING_FLAGS = -Wall -Wextra
CFLAGS = -g $(WARNING_FLAGS) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE 
CC = g++
DIRS := . ./business ./common ./conf ./log ./server ./net
SOURCE := $(foreach DIRS,$(DIRS),$(wildcard $(DIRS)/*.cc)) 
OBJS := $(patsubst %.cc,%.o,$(SOURCE))
all: $(EXECUTABLE)

%.o:%.cc
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@
$(EXECUTABLE): $(OBJS) 
	$(CC) $(CFLAGS) -o  $(EXECUTABLE) $(OBJS)  $(addprefix -L,$(LIBPATHS)) $(addprefix -l,$(LIBS))
	rm -rf *.o
clean: 
	rm -rf *.o $(EXECUTABLE) $(OBJS)
