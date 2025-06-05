#ifndef HELLO_SERVICE_IMPL_HPP
#define HELLO_SERVICE_IMPL_HPP

#include <grpcpp/grpcpp.h>
#include "hello_grpc.pb.h"
#include "hello_grpc.grpc.pb.h"

using namespace grpc;

/**
 * MyServiceImplement 클래스
 * 
 * gRPC 서비스 구현 클래스입니다.
 * protoc가 생성한 MyService::Service를 상속받아
 * 실제 RPC 메서드를 구현합니다.
 */
class MyServiceImplement final : public MyService::Service
{
public:
    /**
     * MyFunction RPC 메서드
     * 
     * 클라이언트로부터 숫자를 받아 제곱값을 반환합니다.
     * 
     * @param context gRPC 서버 컨텍스트 (메타데이터, 취소 상태 등)
     * @param request 클라이언트 요청 (MyNumber 타입)
     * @param response 서버 응답 (MyNumber 타입)
     * @return Status gRPC 상태 코드
     */
    Status MyFunction(ServerContext* context, const MyNumber* request, 
        MyNumber* response) override;
};

#endif
