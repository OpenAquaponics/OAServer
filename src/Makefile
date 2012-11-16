################################################################################
# Makefile for the RPiServer exec
################################################################################
#
# Author: Joshua Eliser
#
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH=..
include ${ROOT_SRC_PATH}/root_make.rules

# Library name
LIB_NAME = "OAServer"

# Include and library paths
ALL_INCL_PATHS  = ${ROOT_INCL}
ALL_LIB_PATHS   = ${ROOT_LIB}

# Includeand library files
CMN_ROOT_LIBS   = ${ROOT_ETHERNET}
ALL_LIBS        = ${CMN_ROBOT_LIBS}

# Files to be compiled by the makefile
SRCS = OAServer.cpp
OBJS = ${SRCS:.cpp=.o}

# Main build target
TARGETS = OAServer

# Compiler
CC = g++

all: ${TARGETS}

${OBJS} : ${SRCS} ${LOCAL_INCLS} ./Makefile
	${CC} ${ALL_INCL_PATHS} -c $*.cpp -o $*.o

RPionicsDaemon: ${OBJS} ${LOCAL_INCLS}
	${CC} ${ALL_INCL_PATHS} -o $@ ${OBJS} ${ALL_LIB_PATHS} ${ALL_LIBS}

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


