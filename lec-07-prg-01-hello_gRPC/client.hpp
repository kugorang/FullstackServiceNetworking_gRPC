#ifndef HELLO_CLIENT_HPP
#define HELLO_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include <memory>
#include "hello_grpc.pb.h"
#include "hello_grpc.grpc.pb.h"

using namespace std;
using namespace grpc;

/**
 * HelloClient 클래스
 * 
 * gRPC 클라이언트 구현 클래스입니다.
 * 서버와의 통신을 담당하며 RPC 호출을 수행합니다.
 */
class HelloClient
{
private:
    /**
     * gRPC stub (서비스의 로컬 프록시)
     * 원격 서비스 호출을 위한 인터페이스입니다.
     */
    unique_ptr<MyService::Stub> stub;
    
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    HelloClient(shared_ptr<Channel> channel);
    
    /**
     * Execute 메서드
     * 
     * 서버의 MyFunction을 호출하여 숫자의 제곱값을 받습니다.
     * 
     * @param number 서버에 전송할 숫자
     * @return 계산된 제곱값 (오류 시 -1)
     */
    int32_t Execute(int32_t number);
};

#endif
