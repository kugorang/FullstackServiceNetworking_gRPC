# 4. Server Streaming 프로젝트 CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(server_streaming)

# Proto 파일 경로 설정
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/serverstreaming.proto")

# proto 파일 복사
configure_file(${PROTO_FILE} ${CMAKE_CURRENT_BINARY_DIR}/serverstreaming.proto COPYONLY)

# gRPC/Protobuf 코드 생성
generate_grpc_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

# Include 경로 설정
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Server 실행 파일
add_executable(serverstreaming_server
    server.cpp
    ${PROTO_SRCS}
)

target_link_libraries(serverstreaming_server
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    Threads::Threads
)

# Client 실행 파일
add_executable(serverstreaming_client
    client.cpp
    ${PROTO_SRCS}
)

target_link_libraries(serverstreaming_client
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
)

# 빌드 타입 설정
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Windows 설정
if(WIN32)
    set_property(TARGET serverstreaming_server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:serverstreaming_server>)
    set_property(TARGET serverstreaming_client PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:serverstreaming_client>)
endif()
