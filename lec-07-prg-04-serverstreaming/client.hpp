#ifndef SERVER_STREAM_CLIENT_HPP
#define SERVER_STREAM_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include <memory>
#include "serverstreaming.pb.h"
#include "serverstreaming.grpc.pb.h"

using namespace std;
using namespace serverstreaming;
using namespace grpc;

/**
 * ServerStreamingClient 클래스
 * 
 * 서버 스트리밍 gRPC 클라이언트를 구현합니다.
 * 서버에 단일 요청을 보내고 여러 메시지를 스트림으로 받습니다.
 */
class ServerStreamingClient
{
private:
    /**
     * gRPC stub
     * 원격 서비스 호출을 위한 인터페이스입니다.
     */
    unique_ptr<ServerStreaming::Stub> stub;
    
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    ServerStreamingClient(shared_ptr<Channel> channel);
    
    /**
     * ReceiveMessages 메서드
     * 
     * 서버에 숫자를 전송하고
     * 서버로부터 여러 메시지를 스트림으로 받습니다.
     * 
     * @param value 서버에 전송할 숫자
     */
    void ReceiveMessages(int32_t value);
};

#endif
