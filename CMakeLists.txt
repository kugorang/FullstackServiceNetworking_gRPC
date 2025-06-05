# CMakeList.txt: FullstackServiceNetworking_gRPC에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.20)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 로컬 vcpkg 툴체인 설정 (프로젝트 선언 전에!)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "Vcpkg toolchain file")

project ("FullstackServiceNetworking_gRPC")

set(CMAKE_CXX_STANDARD 20)

# Find packages
find_package(Threads REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 서브 프로젝트 추가
add_subdirectory(hello_grpc)
add_subdirectory(serverstreaming)
add_subdirectory(clientstreaming)
add_subdirectory(bidirectional)

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
