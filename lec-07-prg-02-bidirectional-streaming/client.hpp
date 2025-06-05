#ifndef BIDI_CLIENT_HPP
#define BIDI_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include <memory>
#include "bidirectional.pb.h"
#include "bidirectional.grpc.pb.h"

using namespace std;
using namespace bidirectional;
using namespace grpc;

/**
 * BidirectionalClient 클래스
 * 
 * 양방향 스트리밍 gRPC 클라이언트를 구현합니다.
 * 서버와 동시에 메시지를 주고받을 수 있습니다.
 */
class BidirectionalClient
{
private:
    /**
     * gRPC stub
     * 원격 서비스 호출을 위한 인터페이스입니다.
     */
    unique_ptr<Bidirectional::Stub> stub;
    
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    BidirectionalClient(shared_ptr<Channel> channel);
    
    /**
     * SendAndReceiveMessages 메서드
     * 
     * 서버와 양방향 스트리밍 통신을 수행합니다.
     * 별도 스레드에서 메시지를 전송하면서 동시에 응답을 받습니다.
     */
    void SendAndReceiveMessages();
};

#endif
