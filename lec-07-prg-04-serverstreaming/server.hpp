#ifndef SERVER_STREAM_SERVER_HPP
#define SERVER_STREAM_SERVER_HPP

#include <grpcpp/grpcpp.h>
#include "serverstreaming.pb.h"
#include "serverstreaming.grpc.pb.h"

/**
 * ServerStreamingServer 클래스
 * 
 * 서버 스트리밍 gRPC 서비스를 구현합니다.
 * 클라이언트로부터 단일 요청을 받고 여러 메시지를 스트림으로 전송합니다.
 */
class ServerStreamingServer : public serverstreaming::ServerStreaming::Service {
public:
    /**
     * GetServerResponse - 서버 스트리밍 RPC 메서드
     * 
     * 클라이언트로부터 숫자를 받고
     * 여러 메시지를 스트림으로 응답합니다.
     * 
     * @param context gRPC 서버 컨텍스트
     * @param request 클라이언트로부터 받은 숫자
     * @param writer 서버 메시지 스트림 writer
     * @return Status gRPC 상태 코드
     */
    grpc::Status GetServerResponse(
        grpc::ServerContext* context,
        const serverstreaming::Number* request,
        grpc::ServerWriter<serverstreaming::Message>* writer) override;
};

#endif
