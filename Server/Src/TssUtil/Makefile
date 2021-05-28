###  Makefile
###  Create By LinShan
CPP		= g++
FLAGS		= -D_LINUX -Wno-deprecated
MPATH		= ../../
INC		= -I $(MPATH)Include -I /root/PKISDK/Include -I /root/openssl-1.0.0d/include
LIB		= -L /root/PKISDK/Lib -L /root/openssl-1.0.0d -lKTPKISDK -lKTSDK -lpthread -lm -lz -lcrypto -ldl
LIBNAME		= libTssUtil
LIBPATH		= $(MPATH)Lib/
OBJPATH		= $(MPATH)Obj/$(LIBNAME)/

###  Object List
OBJS		= $(OBJPATH)TssUtil.o $(OBJPATH)TssClient.o $(OBJPATH)MySMLib.o $(OBJPATH)MailCoder.o $(OBJPATH)pk7_sm2_sign.o $(OBJPATH)MyToolLib.o

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
		ar rcs $(LIBPATH)$(LIBNAME).a $(OBJS)
#release:	$(OBJS)
#		ar rcs $(LIBPATH)$(LIBNAME).a $(OBJS)
