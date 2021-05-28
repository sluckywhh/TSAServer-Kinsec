###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS	= -D_LINUX -D_FILE_OFFSET_BITS=64 -D__int32="int" -D__int64="long long" -Wno-deprecated
MPATH	= ../../
INC		= -I $(MPATH)Include
LIB		= -lSecSoap -lxerces-c -lpthread -lm -lz
APPNAME		= TSAServer
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSASetting.o $(OBJPATH)TSAManager.o $(OBJPATH)TSASoapServer.o $(OBJPATH)TSAServer.o $(OBJPATH)XmlDocument.o

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

