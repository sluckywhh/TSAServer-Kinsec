###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= 
LIB		= -lPgSQLKit_C -lxerces-c -lpthread -lm -lz
APPNAME		= TSADbService
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSADbServiceApp.o $(OBJPATH)TSADbService.o $(OBJPATH)XmlDocument.o

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

