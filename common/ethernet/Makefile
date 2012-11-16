################################################################################
# Author : Joshua Eliser
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH = ../..
include ${ROOT_SRC_PATH}/root_make.rules

# Library name
LIB_NAME = "ETHERNET"

# Headers files needed for the compile/linking phase
ALL_INCL_PATHS  = -I${ROOT_INCL_PATH}

# Files to be compiled by the makefile
SRCS_C   = Ethernet.c
OBJS_C   = $(patsubst %.c,%.c.o,${SRCS_C})
SRCS_CPP = Ethernet.cpp
OBJS_CPP = $(patsubst %.cpp,%.cpp.o,${SRCS_CPP})

# Name of the library file to be created
TARGETS=libethernet.a

# 'make all' target
all: $(TARGETS)

${OBJS_C} : ${SRCS_C} ./Makefile
	${GCC} -c ${ALL_INCL_PATHS} $* -o $*.o

${OBJS_CPP} : ${SRCS_CPP} ./Makefile
	${G++} -c ${ALL_INCL_PATHS} $* -o $*.o

# 'make' target
libethernet.a: ${OBJS_C} ${OBJS_CPP} ${SRCS_C} ${SRCS_CPP} ./Makefile
	ar -rv libethernet.a ${OBJS_C} ${OBJS_CPP}

# 'make install' target
install:
	cp ./libethernet.a ${ROOT_LIB_PATH}

# 'make clean' target
clean:
	rm -f ${TARGETS} ${OBJS}
	rm -f *.o

# 'make clean_lib' target
clean_lib:
	rm -f ${ROOT_LIB_PATH}/${TARGETS}

# 'make it_all' target
it_all:
	@echo " "
	@echo "         Making ${LIB_NAME} lib"
	make -s
	@echo "         Installing ${LIB_NAME} lib"
	make -s install

# 'make from_scratch' target
from_scratch:
	@echo " "
	@echo "         Cleaning Up ${LIB_NAME} libs/obs"
	make -s clean
	make -s clean_lib
	@echo "         Making ${LIB_NAME} lib"
	make -s
	@echo "         Installing ${LIB_NAME} lib"
	make -s install
