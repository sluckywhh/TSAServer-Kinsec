###  Makefile
###  Create By Linzaogang
CPP		= g++ -g
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= -I $(MPATH)Include -I /root/PKISDK/Include -I /root/openssl-1.0.0d/include
LIB		= -L /root/PKISDK/Lib -lKTPKISDK -lKTSDK -lCommonKit_C -lxerces-c -luuid -lpthread  -lcrypto -ldl

APPNAME		= TSService
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSEngine.o $(OBJPATH)TSServer.o  $(OBJPATH)TSServiceApp.o $(OBJPATH)TSService.o $(OBJPATH)MailCoder.o $(OBJPATH)XmlDocument.o $(OBJPATH)pk7_sm2_sign.o $(OBJPATH)MyToolLib.o

###  Start Make
all:		prepare release

prepare:
		@mkdir -p $(OBJPATH)

###  Compiling
$(OBJPATH)%.o :	%.cc
		$(CPP) $< -c -o $@ $(INC) $(FLAGS)

###  Linking
release:	$(OBJS)
		$(CPP) -o $(MPATH)Bin/$(APPNAME) $(OBJS) $(LIB)

