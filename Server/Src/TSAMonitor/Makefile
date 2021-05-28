###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= 
LIB		= -lxerces-c -lpthread -lm -lz
APPNAME		= TSAMonitor
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSAMonitorApp.o $(OBJPATH)TSAMonitor.o $(OBJPATH)XmlDocument.o 

###  Start Make
all:		prepare release

prepare:
		@mkdir -p $(OBJPATH)

###  Compiling
$(OBJPATH)%.o :	%.cpp
		$(CPP) $< -c -o $@ $(FLAGS) $(INC)

###  Linking
release:	$(OBJS)
		$(CPP) -o $(MPATH)Bin/$(APPNAME) $(OBJS) $(LIB)

