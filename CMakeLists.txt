#
# Copyright (c) 2016 Intel Corporation
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.20.0)
set(SHIELD ssd1306_128x64)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nrf_connect_sdk_fundamentals)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/oled.c)

target_include_directories(app PRIVATE ./)
