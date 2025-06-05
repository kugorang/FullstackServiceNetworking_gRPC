# gRPC C++ Examples

Python gRPC 예제를 C++로 변환한 프로젝트입니다.

## 프로젝트 구조

```
FullstackServiceNetworking_gRPC/
├── CMakeLists.txt                              # 루트 CMake 설정
├── init-vcpkg.bat                              # vcpkg 자동 설치 스크립트
├── lec-07-prg-01-hello_gRPC/                  # Unary RPC
│   ├── hello_grpc.proto
│   ├── server.hpp/cpp
│   └── client.hpp/cpp
├── lec-07-prg-02-bidirectional-streaming/     # 양방향 스트리밍
│   ├── bidirectional.proto
│   ├── server.hpp/cpp
│   └── client.hpp/cpp
├── lec-07-prg-03-clientstreaming/            # 클라이언트 스트리밍
│   ├── clientstreaming.proto
│   ├── server.hpp/cpp
│   └── client.hpp/cpp
└── lec-07-prg-04-serverstreaming/            # 서버 스트리밍
    ├── serverstreaming.proto
    ├── server.hpp/cpp
    └── client.hpp/cpp
```

## 필수 요구사항

- C++17 이상
- CMake 3.16 이상
- Visual Studio 2022 (Windows)

## 설치 및 빌드

### 1. vcpkg 및 필요 패키지 자동 설치
```bash
# 프로젝트 루트에서 실행
init-vcpkg.bat
```
이 스크립트는 다음 작업을 자동으로 수행합니다:
- vcpkg 다운로드 및 설치
- gRPC, Protobuf 패키지 설치
- 환경 설정

### 2. 프로젝트 빌드
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## 프로젝트별 설명

### 1. Hello gRPC (Unary RPC)
- 클라이언트가 숫자를 보내면 서버가 제곱값을 반환
- 가장 기본적인 RPC 패턴

### 2. Bidirectional Streaming
- 클라이언트와 서버가 동시에 메시지 스트림 주고받기
- 실시간 채팅, 화상회의 등에 활용

### 3. Client Streaming  
- 클라이언트가 여러 메시지를 보내고 서버가 단일 응답
- 파일 업로드, 데이터 집계 등에 활용

### 4. Server Streaming
- 클라이언트가 단일 요청, 서버가 여러 메시지 응답
- 실시간 피드, 대용량 데이터 다운로드 등에 활용

## 실행 방법

각 프로젝트 디렉토리에서:

```bash
# 서버 실행 (터미널 1)
./hello_grpc_server.exe

# 클라이언트 실행 (터미널 2)
./hello_grpc_client.exe
```

## 주요 특징

- Python 예제와 동일한 기능 구현
- 헤더/소스 파일 분리로 모듈화
- 상세한 한글 주석
- gRPC 모든 통신 패턴 예제 포함

## 라이선스

BSD 3-Clause License