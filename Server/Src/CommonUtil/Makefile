###  Makefile
###  Create By LinShan
AR		= ar rcs
CPP		= g++
FLAGS	= -D_LINUX -Wno-deprecated
MPATH	= ../../
INC		= -I $(MPATH)Include
LIB		= -L $(MPATH)Lib -lxerces-c -lpthread -lm -lz

APPNAME		= TSACommonUtil
OBJPATH		= $(MPATH)Obj/$(APPNAME)/
TRAGET		= $(MPATH)Lib/libTSACommonUtil.a

###  Object List
OBJS		= $(OBJPATH)TSACommonUtil.o $(OBJPATH)XmlDocument.o $(OBJPATH)MailCoder.o
###  Start Make
all:		prepare $(TRAGET)

prepare:
		@mkdir -p $(OBJPATH)

###  Compiling
$(OBJPATH)%.o :	%.cpp
		$(CPP) $< -c -o $@ $(INC) $(FLAGS)

###  Linking
$(TRAGET):	$(OBJS)
	$(AR) $(TRAGET) $(OBJS)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(TRAGET)

