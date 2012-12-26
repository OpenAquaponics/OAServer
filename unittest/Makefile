################################################################################
# Makefile for the OAServer Unit Test
################################################################################
#
# Author: Joshua Eliser
#
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH = ..
include ${ROOT_SRC_PATH}/root_make.rules

# Library name
LIB_NAME = "Unit Test OAServer"

# Include and library paths
ALL_INCL_PATHS  = -I${ROOT_INCL_PATH} -I./OANode
ALL_LIB_PATHS   = -L${ROOT_LIB_PATH}

# Include and library files
ALL_PATHS       = ${ALL_INCL_PATHS} ${ALL_LIB_PATHS}
ALL_LIBS        = ${LIB_ETHERNET} ${LIB_DATABASE} ${LIB_UTIL} ${LIB_JSON} -lpthread

# Files to be compiled by the makefile
SRCS = unittest.cpp ./OANode/OANode.cpp
OBJS = ${SRCS:.cpp=.o}

# Main build target
TARGETS = unittest

# Compiler
CC = g++

all: ${TARGETS}

${OBJS} : ${SRCS} ${LOCAL_INCLS} ./Makefile
	${CC} ${ALL_PATHS} -c $*.cpp -o $*.o

unittest: ${OBJS} ${LOCAL_INCLS}
	${CC} ${ALL_PATHS} -o $@ ${OBJS} ${ALL_LIBS}

it_all:
	echo " "
	echo "         Making ${LIB_NAME} executable"
	make -s

clean:
	rm -f ${TARGETS} ${OBJS}
	cd OANode; make clean;

clean_app:
	rm -f ${TARGETS}


