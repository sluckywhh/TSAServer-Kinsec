###  Makefile
###  Create By Linzaogang
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= -I $(MPATH)Include -I /root/PKISDK/Include -I /root/openssl-1.0.0d/include
LIB		= -L /root/PKISDK/Lib -L /root/openssl-1.0.0d -lKTPKISDK -lKTSDK -lpthread -lm -lz -lcrypto -ldl
LIBNAME		= libTSASvrCltDll
LIBPATH		= $(MPATH)Lib/
OBJPATH		= $(MPATH)Obj/$(LIBNAME)/

###  Object List
OBJS		= $(OBJPATH)TSASvrStream.o $(OBJPATH)TSASvrContent.o $(OBJPATH)TSASvrClient.o $(OBJPATH)TSASvrCltDll.o $(OBJPATH)MailCoder.o

###  Start Make
all:		prepare release

prepare:
		@echo  ---------------------------------------------------------------
		@echo   Preparing for compiling ......
		@mkdir -p $(OBJPATH) 

###  Compiling
$(OBJPATH)%.o :	%.cpp
		$(CPP) -fPIC -c $< -o $@ $(FLAGS) $(INC)

###  Linking
release:	$(OBJS)
		$(CPP) -shared -o $(LIBPATH)$(LIBNAME).so $(OBJS) $(LIB)

