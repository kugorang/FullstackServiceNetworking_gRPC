/**
 * gRPC Server Streaming Client
 * 
 * 서버 스트리밍을 사용하는 클라이언트입니다.
 * 서버에 단일 요청을 보내고, 여러 메시지를 스트림으로 받습니다.
 * Python의 client.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>

// protoc로 생성된 헤더 파일들
#include "serverstreaming.pb.h"
#include "serverstreaming.grpc.pb.h"

// 네임스페이스 선언
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReader;

// serverstreaming 네임스페이스 사용
using serverstreaming::ServerStreaming;
using serverstreaming::Message;
using serverstreaming::Number;

/**
 * ServerStreamingClient 클래스
 * 
 * 서버 스트리밍을 구현하는 클라이언트입니다.
 */
class ServerStreamingClient {
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    ServerStreamingClient(std::shared_ptr<Channel> channel)
        : stub_(ServerStreaming::NewStub(channel)) {
    }
    
    /**
     * RecvMessage - 서버 스트리밍 통신 수행
     * 
     * Python의 recv_message(stub) 함수에 대응
     */
    void RecvMessage() {
        // 서버에 보낼 요청 생성
        // Python의 request = serverstreaming_pb2.Number(value=5)에 대응
        Number request;
        request.set_value(5);
        
        // RPC 컨텍스트 생성
        ClientContext context;
        
        // 서버로부터 메시지를 읽을 스트림 리더 생성
        // Python의 responses = stub.GetServerResponse(request)에 대응
        std::unique_ptr<ClientReader<Message>> reader(
            stub_->GetServerResponse(&context, request));
        
        std::cout << "서버에 요청 전송: " << request.value() << std::endl;
        
        // 서버로부터 스트림 메시지 읽기
        // Python의 for response in responses: 에 대응
        Message response;
        while (reader->Read(&response)) {
            std::cout << "[서버 -> 클라이언트] " << response.message() << std::endl;
        }
        
        // RPC 완료 상태 확인
        Status status = reader->Finish();
        
        if (status.ok()) {
            std::cout << "서버 스트리밍 완료" << std::endl;
        } else {
            std::cerr << "RPC 실패: " << status.error_message() << std::endl;
        }
    }
    
private:
    std::unique_ptr<ServerStreaming::Stub> stub_;
};

/**
 * run 함수
 * 
 * Python의 run() 함수에 대응
 */
void run() {
    std::string server_address = "localhost:50051";
    
    // 채널 생성
    auto channel = grpc::CreateChannel(
        server_address, 
        grpc::InsecureChannelCredentials()
    );
    
    ServerStreamingClient client(channel);
    client.RecvMessage();
}

/**
 * 메인 함수
 */
int main(int argc, char** argv) {
    try {
        run();
    } catch (const std::exception& e) {
        std::cerr << "클라이언트 오류: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
