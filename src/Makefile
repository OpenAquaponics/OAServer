################################################################################
# Makefile for the RPiServer exec
################################################################################
#
# Author: Joshua Eliser
#
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH = ..
include ${ROOT_SRC_PATH}/root_make.rules

# Library name
LIB_NAME = "OAServer"

# Include and library paths
ALL_INCL_PATHS  = -I${ROOT_INCL_PATH}
ALL_LIB_PATHS   = -L${ROOT_LIB_PATH}

# Include and library files
ALL_PATHS       = ${ALL_INCL_PATHS} ${ALL_LIB_PATHS}
ALL_LIBS        = ${LIB_ETHERNET} ${LIB_DATABASE} -lpthread

# Files to be compiled by the makefile
SRCS = HandlerThread.cpp OAServer.cpp Application.cpp
OBJS = ${SRCS:.cpp=.o}

# Main build target
TARGETS = OAServer

# Compiler
CC = g++

all: ${TARGETS}

${OBJS} : ${SRCS} ${LOCAL_INCLS} ./Makefile
	${CC} ${ALL_PATHS} -c $*.cpp -o $*.o

OAServer: ${OBJS} ${LOCAL_INCLS}
	${CC} ${ALL_PATHS} -o $@ ${OBJS} ${ALL_LIBS}

it_all:
	@echo " "
	@echo "         Making ${LIB_NAME} executable"
	make -s
	@echo "         Installing ${LIB_NAME} executable"
	make -s all
	cp ./OAServer ${ROOT_BIN_PATH}

clean:
	rm -f ${TARGETS} ${OBJS}
	rm -f ${ROOT_BIN_PATH}/OAServer

clean_app:
	rm -f ${TARGETS}
	rm -f ${ROOT_BIN_PATH}/OAServer


