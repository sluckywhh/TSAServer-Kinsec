ODIR			= ../../Obj
OUTPUTDIR		= ../../Bin
SRCDIR			= ./src

CC			= g++
RM			= rm -f
cc 			= gcc

CCFLAGS			= -g  -Wno-deprecated -D_LINUX -D_DEBUG
LIBS 			= -lm  -lc -ldl -lcurses -lxerces-c

OUT			= $(OUTPUTDIR)/sendmail

PROGRAM			= $(OUT)
########## object files	###########################

OUT_OBJ			=$(ODIR)/Main.o \
			     $(ODIR)/SmtpSocket.o \
			     $(ODIR)/SmtpConnect.o \
			     $(ODIR)/SmtpMessage.o \
			     $(ODIR)/Base64Code.o \
			     $(ODIR)/CommonHelper.o \
			     $(ODIR)/XMLConf.o 
###################################################

$(OUT)::$(OUT_OBJ)
	@echo ------ Linking...	------
	@$(CC) -o  $(OUT)  $(OUT_OBJ)  $(LIBS) $(CCFLAGS)
	@echo ------ make $@ OK. ------
link::
	@echo ------ Linking... ------
	@$(CC) -o  $(OUT)  $(OUT_OBJ)  $(LIBS) $(CCFLAGS)
	@echo ------ make $@ OK. ------

clean::
	@$(RM) $(OUT_OBJ) $(PROGRAM)

cleanobj::
	@$(RM) $(OUT_OBJ)
cleanbin::
	@$(RM) $(PROGRAM)

.SUFFIXES: .cpp .c .o .so .a
###########################################################
$(ODIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) -o $@ $(CCFLAGS) -c $<


