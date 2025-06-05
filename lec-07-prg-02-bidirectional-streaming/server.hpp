#ifndef BIDI_SERVER_HPP
#define BIDI_SERVER_HPP

#include <grpcpp/grpcpp.h>
#include "bidirectional.pb.h"
#include "bidirectional.grpc.pb.h"

using namespace bidirectional;
using namespace grpc;

/**
 * BidirectionalServer 클래스
 * 
 * 양방향 스트리밍 gRPC 서비스를 구현합니다.
 * 클라이언트와 서버가 동시에 메시지를 주고받을 수 있습니다.
 */
class BidirectionalServer : public Bidirectional::Service {
public:
    /**
     * GetServerResponse - 양방향 스트리밍 RPC 메서드
     * 
     * 클라이언트로부터 메시지 스트림을 받으면서
     * 동시에 응답 스트림을 전송합니다.
     * 
     * @param context gRPC 서버 컨텍스트
     * @param stream 읽기/쓰기가 가능한 양방향 스트림
     * @return Status gRPC 상태 코드
     */
    Status GetServerResponse(ServerContext* context,
        ServerReaderWriter<Message, Message>* stream) override;
};

#endif
