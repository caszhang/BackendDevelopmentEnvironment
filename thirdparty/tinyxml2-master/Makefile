EXECUTABLE := ./lib/libtinyxml.a 
BASE_PATH := 
LIBS :=
INCS := . 
USER_MARCOS :=
LIBPATHS := 
WARNING_FLAGS = -Wall -Wextra
CFLAGS = -g $(WARNING_FLAGS) 
CC = g++
DIRS := . 
SOURCE := $(foreach DIRS,$(DIRS),$(wildcard $(DIRS)/*.cpp)) 
OBJS := $(patsubst %.cpp,%.o,$(SOURCE))
all: $(EXECUTABLE)

%.o:%.cc
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I,$(INCS)) -c $< -o $@
$(EXECUTABLE): $(OBJS) 
	$(AR) $(ARFLAGS) $@ $^
	rm *.o
clean: 
	rm -rf *.o $(EXECUTABLE) $(OBJS)
