#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project("Profiler unit tests")

# # Include event headers
# zephyr_library_include_directories(src/events)

# Add test sources
target_sources(app PRIVATE src/main.c)
