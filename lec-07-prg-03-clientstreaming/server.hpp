#ifndef CLIENT_STREAM_SERVER_HPP
#define CLIENT_STREAM_SERVER_HPP

#include <grpcpp/grpcpp.h>
#include "clientstreaming.pb.h"
#include "clientstreaming.grpc.pb.h"

using namespace clientstreaming;
using namespace grpc;

/**
 * ClientStreamingServer 클래스
 * 
 * 클라이언트 스트리밍 gRPC 서비스를 구현합니다.
 * 클라이언트로부터 여러 메시지를 받고 단일 응답을 반환합니다.
 */
class ClientStreamingServer : public ClientStreaming::Service
{
public:
    /**
     * GetServerResponse - 클라이언트 스트리밍 RPC 메서드
     * 
     * 클라이언트로부터 메시지 스트림을 받아 개수를 세고
     * 총 메시지 개수를 반환합니다.
     * 
     * @param context gRPC 서버 컨텍스트
     * @param reader 클라이언트 메시지 스트림 리더
     * @param response 메시지 개수를 담을 응답 객체
     * @return Status gRPC 상태 코드
     */
    Status GetServerResponse(ServerContext* context, ServerReader<Message>* reader,
        Number* response) override;
};

#endif
