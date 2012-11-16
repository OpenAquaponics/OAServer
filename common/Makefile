################################################################################
# Author : Joshua Eliser
################################################################################

# set where we are relative to the root of the source tree and include it
ROOT_SRC_PATH=..
include ${ROOT_SRC_PATH}/root_make.rules

# Default target
TARGETS=it_all

# Project name
PROJECT_NAME="COMMON"

all: ${TARGETS}

it_all:
	@echo " "
	@echo "   **** Execing Make it_all for the entire ${PROJECT_NAME} project ****"
	@echo "          Making the entire ${PROJECT_NAME} project"
	(cd ./ethernet;        make -s it_all)

clean:
	@echo " "
	@echo "   **** Execing Make clean for the entire ${PROJECT_NAME} project ****"
	@echo "          Cleaning the entire ${PROJECT_NAME} project"
	(cd ./ethernet;        make -s clean;      make -s clean_lib)

from_scratch:
	@echo " "
	@echo "   **** Execing Make from_scratch for the entire ${PROJECT_NAME} project ****"
	@echo "          Building from scratch the entire ${PROJECT_NAME} project"
	(cd ./ethernet;        make -s from_scratch)
