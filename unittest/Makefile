###Makefile for Testing rcPkg c code using cpputest
###By Feng 03/19/2020 @bu
#######v1.0
#-----NOTE:
#1) this is now includes cpputest. bottom line : it is the c++ program. It can take care c code, but need g++ for linking. 
#2) we mix gcc and g++ to compile, and in the end we link with g++.
#3) when linking 
#4)define the CPPUTES_HOME
#5)linking ld -o out.ext main.o source.o .....linking libries.....
# 		the order is important.
###Define variables######

GCC = gcc
GXX = g++

MAKEFILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
PROJECT_DIR=$(realpath $(MAKEFILE_DIR)..)
TEST_DIR=$(PROJECT_DIR)/unittest

CPPUTEST_HOME=$(PROJECT_DIR)/cpputest

# project source files path
SRC_DIRS=$(PROJECT_DIR)/src

# project header files path
INCLUDE_DIRS = $(PROJECT_DIR)/src

# add this to avoid problems when including cpputest headers on test files
INCLUDE_DIRS += -I $(CPPUTEST_HOME)/include


CFLAG += -Wall -g -Werror -O -DC_CPP -I$(INCLUDE_DIRS) 

CXXFLAG += ${CFLAG} -std=c++11

CFLAG += -include ${CPPUTEST_HOME}/include/CppUTest/MemoryLeakDetectorMallocMacros.h 

#LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
LOADFLAG += -s -lm -lz
LOADFLAG += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

#SRCS_0 = main.c ${fileDIR}helper_c.c 
SRCS_1 = test_main.cpp testers.cpp 
SRCS_1c = ${SRC_DIRS}/helper_c.c ${SRC_DIRS}/clonalDiversity_c.c

#SRCS_1=	NGSMapping_Adaptor_main.cpp ${ACCESSDIR}string_ext.cpp score.cpp ${ACCESSDIR}SequenceString.cpp AlignmentString.cpp pairwiseAlignment.cpp OverlapAlignment.cpp ${ACCESSDIR}FastaHandler.cpp SequenceHandler.cpp GapModel.cpp AffineGapModel.cpp MarkovChainGapModel_454.cpp TracebackTable.cpp SequenceHandlerCommon.cpp LocalAlignment.cpp ${ACCESSDIR}GzTools.cpp ${ACCESSDIR}FileHandler.cpp ${ACCESSDIR}FASTQ.cpp ${ACCESSDIR}FastqHandler.cpp

#OBJS_0 = ${SRCS_0:.c=.o}
OBJS_1 = ${SRCS_1:.cpp=.o}  ${SRCS_1c:.c=.o}
#PROG_0 = unit_testing
PROG_1 = rcPkg_tests

#DEPEND=$(GXX) $(CFLAG) -MM

######Rules######
.PHONY: clean all depend debug

all: $(PROG_1) 

debug: 
	@echo "***showing the VAR:";
	@echo "\tMAKEFILE_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))";	
	@echo "\tPROJECT_DIR=$(realpath $(MAKEFILE_DIR)..)"
	@echo "\tTEST_DIR=$(PROJECT_DIR)/unittest"
	@echo "\tCPPUTEST_HOME=$(PROJECT_DIR)/cpputest"
	@echo "\tSRC_DIRS=$(PROJECT_DIR)/src"
	@echo "\tINCLUDE_DIRS=$(PROJECT_DIR)/src"
	@echo "\tINCLUDE_DIRS+=$(CPPUTEST_HOME)/include"
	@echo "\t*********************************";echo "";
	
clean:
	rm -fr *.o *~ core $(PROG_1)  ${SRCS_1c:.c=.o}
	#cd $(SRC_DIRS) && rm -fr *.o *~ core; #ls ;   #pwd ; # note makefile is a script and each line (of commands) is doing its own sub-precess (&& is trying to stop if the "cd" command fails.

.cpp.o: #for cpp 
	$(GXX) $(CXXFLAG) -c $< -o $(addsuffix .o, $(basename $<))

.c.o:   #old fasion suffix rule, double suffix rule
	$(GCC) $(CFLAG) -c $< -o $(addsuffix .o, $(basename $<))

#$(PROG_0): $(OBJS_0)
#	$(GXX) -o $@ $+ $(LOADFLAG) #$(LD_LIBRARIES)		
#	@echo "******Make complete";echo "";

$(PROG_1): $(OBJS_1)
	$(GXX) -o $@  $+ $(LOADFLAG)
	@echo "******Make complete"; echo "";
	
depend: .depend

.depend: Makefile $(SRCS_1) $(SRCS_1c) 
	$(GCC) -MM $(CFLAG) ${SRCS_1c} >.depend
	$(GXX) -MM $(CFLAG) *.cpp >.depend
	@echo " "
	@echo "****Dependencies generated successfully."


include .depend 

