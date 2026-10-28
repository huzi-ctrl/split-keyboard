# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()
zephyr_library_sources(pkb_status.c)
zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
