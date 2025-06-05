# CMake 최소 버전 지정 (gRPC와 Protobuf의 최신 기능 사용을 위해 3.16 이상 필요)
cmake_minimum_required(VERSION 3.16)

# 프로젝트 이름 설정
project(FullstackServiceNetworking_gRPC)

# C++ 표준 설정 (gRPC는 C++14 이상 필요, 여기서는 C++17 사용)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg 또는 시스템에 설치된 패키지 찾기
# CONFIG 모드를 사용하여 *Config.cmake 파일들을 찾음
find_package(Threads REQUIRED)  # 멀티스레딩 지원
find_package(Protobuf CONFIG REQUIRED)  # Protocol Buffers
find_package(gRPC CONFIG REQUIRED)  # gRPC 라이브러리
find_package(absl CONFIG REQUIRED)  # Abseil 라이브러리 (gRPC 의존성)

# 공통 proto 파일 생성 함수 정의
# 이 함수는 각 하위 프로젝트에서 재사용됨
function(generate_grpc_cpp SRCS HDRS PROTO_FILE)
    # proto 파일의 경로와 이름 분리
    get_filename_component(PROTO_PATH "${PROTO_FILE}" PATH)
    get_filename_component(PROTO_NAME "${PROTO_FILE}" NAME_WE)

    # 생성될 파일들의 이름 설정
    set(PROTO_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h")
    set(GRPC_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")

    # protoc 명령어로 C++ 코드 생성
    add_custom_command(
        OUTPUT "${PROTO_SRC}" "${PROTO_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
        COMMAND $<TARGET_FILE:protobuf::protoc>
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
             --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
             -I "${PROTO_PATH}"
             --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
             "${PROTO_FILE}"
        DEPENDS "${PROTO_FILE}"
        COMMENT "Running C++ gRPC compiler on ${PROTO_FILE}"
        VERBATIM
    )

    # 출력 변수 설정
    set(${SRCS} "${PROTO_SRC}" "${GRPC_SRC}" PARENT_SCOPE)
    set(${HDRS} "${PROTO_HDR}" "${GRPC_HDR}" PARENT_SCOPE)
endfunction()

# 하위 프로젝트들 추가
# 각 디렉토리는 독립적인 CMake 프로젝트로 처리됨
add_subdirectory(lec-07-prg-01-hello_gRPC)
add_subdirectory(lec-07-prg-02-bidirectional-streaming)
add_subdirectory(lec-07-prg-03-clientstreaming)
add_subdirectory(lec-07-prg-04-serverstreaming)
