# 1. Hello gRPC 프로젝트 CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(hello_gRPC)

# Proto 파일 경로 설정
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/hello_grpc.proto")

# proto 파일을 빌드 디렉토리로 복사
# (protoc가 소스 디렉토리의 proto 파일을 직접 읽도록)
configure_file(${PROTO_FILE} ${CMAKE_CURRENT_BINARY_DIR}/hello_grpc.proto COPYONLY)

# 부모 디렉토리에서 정의한 generate_grpc_cpp 함수 사용
# PROTO_SRCS와 PROTO_HDRS 변수에 생성된 파일 경로들이 저장됨
generate_grpc_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})

# 생성된 헤더 파일들을 찾을 수 있도록 include 경로 추가
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Server 실행 파일 빌드 설정
add_executable(hello_grpc_server
    server.cpp          # 메인 서버 구현 파일
    ${PROTO_SRCS}      # protobuf와 gRPC 생성 소스 파일들
)

# Server에 필요한 라이브러리 링크
# gRPC::grpc++ : gRPC C++ 라이브러리
# protobuf::libprotobuf : Protocol Buffers 라이브러리
# Threads::Threads : POSIX 스레드 라이브러리 (gRPC 내부에서 사용)
target_link_libraries(hello_grpc_server
    gRPC::grpc++
    # gRPC::grpc++_reflection  # 제거 - 별도 설정 필요
    protobuf::libprotobuf
    Threads::Threads
)

# Client 실행 파일 빌드 설정
add_executable(hello_grpc_client
    client.cpp          # 메인 클라이언트 구현 파일
    ${PROTO_SRCS}      # protobuf와 gRPC 생성 소스 파일들
)

# Client에 필요한 라이브러리 링크
target_link_libraries(hello_grpc_client
    gRPC::grpc++
    protobuf::libprotobuf
    Threads::Threads
)

# 빌드 타입별 최적화 설정 (선택사항)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Windows에서 DLL 복사 설정 (선택사항)
if(WIN32)
    # Visual Studio에서 실행 시 필요한 DLL들이 실행파일과 같은 위치에 있도록 설정
    set_property(TARGET hello_grpc_server PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:hello_grpc_server>)
    set_property(TARGET hello_grpc_client PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:hello_grpc_client>)
endif()
