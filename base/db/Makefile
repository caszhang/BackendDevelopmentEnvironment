EXECUTABLE := ../lib/libmysqldb.a 
BASE_PATH := . ../../thirdparty
LIBS := mysqlclient pthread 
INCS := . .. $(BASE_PATH)/mysql/include
USER_MARCOS :=
LIBPATHS := $(BASE_PATH)/mysql/lib
WARNING_FLAGS = -Wall -Wextra
CFLAGS = -g $(WARNING_FLAGS) 
CC = g++
DIRS := . 
SOURCE := $(foreach DIRS,$(DIRS),$(wildcard $(DIRS)/*.cc)) 
OBJS := $(patsubst %.cc,%.o,$(SOURCE))
all: $(EXECUTABLE)

%.o:%.cc
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@
$(EXECUTABLE): $(OBJS) 
	$(AR) $(ARFLAGS) $@ $^
	rm *.o
clean: 
	rm -rf *.o $(EXECUTABLE) $(OBJS)
