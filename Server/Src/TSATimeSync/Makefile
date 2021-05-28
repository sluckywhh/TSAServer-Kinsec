###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= -I $(MPATH)Include
LIB		= -L $(MPATH)Lib -lxerces-c -lpthread -lm -lz
APPNAME		= TSATimeSync
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSATimeSyncApp.o $(OBJPATH)TSATimeSync.o $(OBJPATH)XmlDocument.o

###  Start Make
all:		prepare release

prepare:
		@mkdir -p $(OBJPATH)

###  Compiling
$(OBJPATH)%.o :	%.cpp
		$(CPP) $< -c -o $@ $(INC) $(FLAGS)

###  Linking
release:	$(OBJS)
		$(CPP) -o $(MPATH)Bin/$(APPNAME) $(OBJS) $(LIB)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(MPATH)Bin/$(APPNAME)

