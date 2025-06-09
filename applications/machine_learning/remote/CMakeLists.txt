#
# Copyright (c) 2024 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

################################################################################

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/configuration/${NORMALIZED_BOARD_TARGET}")
  message(FATAL_ERROR
          "Board ${BOARD} is not supported.\n"
          "Please make sure board specific configuration files are added to "
          "${CMAKE_CURRENT_SOURCE_DIR}/configuration/${BOARD}")
endif()

set(REMOTE_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/configuration/${NORMALIZED_BOARD_TARGET}")
string(CONFIGURE ${REMOTE_INC_DIR} REMOTE_INC_DIR)

project(remote)

target_sources(app PRIVATE src/main.c)

if((DEFINED CONFIG_SOC_NRF54H20_CPUPPR) OR (DEFINED CONFIG_SOC_NRF54H20_ENGB_CPUPPR))
	message(STATUS "Power Mode handler for RISC V is included.")
	target_sources(app PRIVATE ../../../tests/benchmarks/multicore/common/power_off.c)
endif()

# Add include directory for board specific CAF def files
zephyr_include_directories(
  ../configuration/common
  /configuration
  ${REMOTE_INC_DIR})

# Add subdirectory from common part.
add_subdirectory(../common/src/modules ${CMAKE_CURRENT_BINARY_DIR}/common/src/module)
