set(LLVM_TARGETS_TO_BUILD "Mips" CACHE STRING "")
set(LLVM_ENABLE_PROJECTS "clang;lld" CACHE STRING "")

# Disable components not needed for bare-metal
set(COMPILER_RT_BUILD_SANITIZERS OFF CACHE BOOL "")
set(COMPILER_RT_BUILD_XRAY OFF CACHE BOOL "")
set(COMPILER_RT_BUILD_LIBFUZZER OFF CACHE BOOL "")
set(COMPILER_RT_BUILD_PROFILE OFF CACHE BOOL "")
set(COMPILER_RT_BUILD_MEMPROF OFF CACHE BOOL "")
set(COMPILER_RT_BUILD_ORC OFF CACHE BOOL "")

set(LLVM_INSTALL_TOOLCHAIN_ONLY ON CACHE BOOL "")

set(LLVM_TOOLCHAIN_TOOLS
  llc
  llvm-ar
  llvm-as
  llvm-cxxfilt
  llvm-dis
  llvm-nm
  llvm-objcopy
  llvm-objdump
  llvm-ranlib
  llvm-readelf
  llvm-readobj
  llvm-size
  llvm-strings
  llvm-strip
  llvm-symbolizer
  opt
  CACHE STRING "")

set(LLVM_DISTRIBUTION_COMPONENTS
  clang
  lld
  clang-resource-headers
  ${LLVM_TOOLCHAIN_TOOLS}
  CACHE STRING "")
