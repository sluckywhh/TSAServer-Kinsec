###  Makefile
###  Create By Linzaogang
CPP		= g++ -g
FLAGS	= -D_LINUX -Wno-deprecated
MPATH	= ../../
PKIPATH	= /root/PKISDK/ktsof/
INC		= -I $(MPATH)Include -I$(PKIPATH)Include
LIB		= -L $(MPATH)Lib -L$(PKIPATH)Lib -lTSACommonUtil -lxerces-c -lKSSignLib -lMyToolLib -lMySMLib -lcrypto -lpthread -lm -lz


APPNAME		= TSAServiceHTTP
OBJPATH		= $(MPATH)Obj/$(APPNAME)/

###  Object List
OBJS		= $(OBJPATH)TSAServiceHTTP.o $(OBJPATH)TSAServiceHTTPApp.o $(OBJPATH)http_function.o $(OBJPATH)http_conn.o $(OBJPATH)STFTSAFunction.o

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

