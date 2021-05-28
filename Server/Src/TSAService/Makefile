###  Makefile
###  Create By Linzaogang
CPP		= g++ -g
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= -I$(MPATH)Include -I/root/PKISDK/Include -I/root/openssl-1.0.0d/include
LIB		= -L/root/PKISDK/Lib -lKTPKISDK -lKTSDK -lCommonKit_C -lxerces-c -luuid -lpthread  -lcrypto -ldl

APPNAME		= TSAService
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSAEngine.o $(OBJPATH)TSASvrServer.o  $(OBJPATH)TSAServiceApp.o $(OBJPATH)TSAService.o $(OBJPATH)MailCoder.o $(OBJPATH)XmlDocument.o

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

