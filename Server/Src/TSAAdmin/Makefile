###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= 
LIB		= -lm -lz
APPNAME		= TSAAdmin
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSAAdmin.o

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

