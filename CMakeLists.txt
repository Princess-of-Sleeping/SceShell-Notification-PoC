cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(sce_shell_notice_poc)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

enable_language(C ASM)

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(sce_shell_notice_poc
  src/main.c
  src/taihen_min.c
)

target_link_libraries(sce_shell_notice_poc
  SceLibKernel_stub
  SceKernelModulemgr_stub
  SceKernelThreadMgr_stub
  SceSysmem_stub
  taihen_stub
)

set_target_properties(sce_shell_notice_poc
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(sce_shell_notice_poc.suprx sce_shell_notice_poc
  UNSAFE
  CONFIG exports.yml
)

