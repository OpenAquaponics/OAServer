################################################################################
# Makefile for building the entire RPionics project
################################################################################
# Author :
#
# Joshua Eliser
#
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH=.
include ${ROOT_SRC_PATH}/root_make.rules

# Default target
TARGETS=it_all

# Project name
PROJECT_NAME=OAServer

all: ${TARGETS}

it_all:
	@echo " "
	@echo "   **** Execing Make it_all for the entire ${PROJECT_NAME} project ****"
	@echo " "
	@echo "        Making the entire ${PROJECT_NAME} project"
	(cd ./common;          make -s it_all)
	(cd ./src;             make -s it_all)
	(cd ./unittest;        make -s it_all)

clean:
	@echo " "
	@echo "   **** Execing Make clean for the entire ${PROJECT_NAME} project ****"
	@echo " "
	@echo "        Cleaning the entire ${PROJECT_NAME} project"
	(cd ./common;          make -s clean)
	(cd ./src;             make -s clean)
	(cd ./unittest;        make -s clean)

from_scratch:
	@echo " "
	@echo "   **** Execing Make from_scratch for the entire ${PROJECT_NAME} project ****"
	@echo " "
	@echo "        Building from scratch the entire ${PROJECT_NAME} project"
	(cd ./common;          make -s from_scratch)
	(cd ./src;             make -s from_scratch)
	(cd ./unittest;        make -s from_scratch)
