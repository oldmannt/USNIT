#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(../../../gearsbox/deps/djinni/support-lib)
include_directories(../../../gearsbox/deps/djinni/support-lib/jni)
include_directories(../../../gearsbox/generated-src/cpp)
include_directories(../../../gearsbox/generated-src/jni)
include_directories(../../../gearsbox/deps/curl)
include_directories(../../../gearsbox/deps/libuv/include)
include_directories(../../../gearsbox/deps/libuv/src)
include_directories(../../../gearsbox/deps/sqlite3)
include_directories(../../../gearsbox/src)
include_directories(../../../gearsbox/interface)
include_directories(../../../gearsbox/src/cpp/ui)
include_directories(../../../gearsbox/deps/djinni/support-lib)
include_directories(../../../gearsbox/deps/djinni/support-lib/jni)
include_directories(../../../gearsbox/generated-src/cpp)
include_directories(../../../gearsbox/generated-src/jni)
include_directories(../../../gearsbox/deps/curl)
include_directories(../../../gearsbox/deps/libuv/include)
include_directories(../../../gearsbox/deps/libuv/src)
include_directories(../../../gearsbox/deps/sqlite3)
include_directories(../../../gearsbox/src)
include_directories(../../../gearsbox/interface)
include_directories(../../../cpp)
include_directories(../../../cpp)
include_directories(../../../generated-src/cpp)
include_directories(../../../generated-src/jni)
include_directories(../../../generated-src/cpp)
include_directories(../../../generated-src/jni)

file(GLOB_RECURSE DJINNI ../../../gearsbox/deps/djinni/support-lib/jni/*.c* *.h*)
file(GLOB_RECURSE GEARS_GEN ../../../gearsbox/generated-src/*.c* *.h*)
file(GLOB_RECURSE CURL ../../../gearsbox/deps/curl/*.c* *.h*)
file(GLOB_RECURSE GEARS_SRC ../../../gearsbox/src/*.c* *.h*)
file(GLOB_RECURSE USNIT_SRC ../../../cpp/*.c* *.h*)
file(GLOB_RECURSE USNIT_GEN ../../../generated-src/*.c* *.h*)

file(GLOB_RECURSE LIBUV ../../../gearsbox/deps/libuv/src/fs-poll.c
../../../gearsbox/deps/libuv/src/inet.c
../../../gearsbox/deps/libuv/src/threadpool.c
../../../gearsbox/deps/libuv/src/uv-common.c
../../../gearsbox/deps/libuv/src/version.c
../../../gearsbox/deps/libuv/src/unix/async.c
../../../gearsbox/deps/libuv/src/unix/core.c
../../../gearsbox/deps/libuv/src/unix/dl.c
../../../gearsbox/deps/libuv/src/unix/fs.c
../../../gearsbox/deps/libuv/src/unix/getaddrinfo.c
../../../gearsbox/deps/libuv/src/unix/getnameinfo.c
../../../gearsbox/deps/libuv/src/unix/loop.c
../../../gearsbox/deps/libuv/src/unix/loop-watcher.c
../../../gearsbox/deps/libuv/src/unix/pipe.c
../../../gearsbox/deps/libuv/src/unix/poll.c
../../../gearsbox/deps/libuv/src/unix/process.c
../../../gearsbox/deps/libuv/src/unix/signal.c
../../../gearsbox/deps/libuv/src/unix/stream.c
../../../gearsbox/deps/libuv/src/unix/tcp.c
../../../gearsbox/deps/libuv/src/unix/thread.c
../../../gearsbox/deps/libuv/src/unix/timer.c
../../../gearsbox/deps/libuv/src/unix/tty.c
../../../gearsbox/deps/libuv/src/unix/udp.c
../../../gearsbox/deps/libuv/src/unix/proctitle.c
../../../gearsbox/deps/libuv/src/unix/linux-core.c
../../../gearsbox/deps/libuv/src/unix/linux-inotify.c
../../../gearsbox/deps/libuv/src/unix/linux-syscalls.c
../../../gearsbox/deps/libuv/src/unix/pthread-fixes.c
../../../gearsbox/deps/libuv/src/unix/android-ifaddrs.c
../../../gearsbox/deps/libuv/src/unix/pthread-barrier.c)

add_library(usnit_jni SHARED ${DJINNI} ${GEARS_GEN} ${CURL} ${LIBUV} ${GEARS_SRC} ${USNIT_SRC} ${USNIT_GEN})

# add lib dependencies
target_link_libraries(usnit_jni android log dl)


