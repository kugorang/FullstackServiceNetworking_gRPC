#ifndef CLIENT_STREAM_CLIENT_HPP
#define CLIENT_STREAM_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include <memory>
#include "clientstreaming.pb.h"
#include "clientstreaming.grpc.pb.h"

using namespace std;
using namespace grpc;
using namespace clientstreaming;

/**
 * ClientStreamingClient 클래스
 * 
 * 클라이언트 스트리밍 gRPC 클라이언트를 구현합니다.
 * 서버에 여러 메시지를 전송하고 단일 응답을 받습니다.
 */
class ClientStreamingClient
{
private:
    /**
     * gRPC stub
     * 원격 서비스 호출을 위한 인터페이스입니다.
     */
    unique_ptr<ClientStreaming::Stub> stub;
    
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    ClientStreamingClient(shared_ptr<Channel> channel);
    
    /**
     * SendMessages 메서드
     * 
     * 서버에 여러 메시지를 스트림으로 전송하고
     * 서버가 받은 총 메시지 개수를 응답으로 받습니다.
     */
    void SendMessages();
};

#endif
