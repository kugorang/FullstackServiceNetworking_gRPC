/**
 * gRPC Client Streaming Client
 * 
 * 클라이언트 스트리밍을 사용하는 클라이언트입니다.
 * 서버에 여러 메시지를 스트림으로 전송하고, 단일 응답을 받습니다.
 * Python의 client.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>

// protoc로 생성된 헤더 파일들
#include "clientstreaming.pb.h"
#include "clientstreaming.grpc.pb.h"

// 네임스페이스 선언
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientWriter;

// clientstreaming 네임스페이스 사용
using clientstreaming::ClientStreaming;
using clientstreaming::Message;
using clientstreaming::Number;

/**
 * ClientStreamingClient 클래스
 * 
 * 클라이언트 스트리밍을 구현하는 클라이언트입니다.
 */
class ClientStreamingClient {
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    ClientStreamingClient(std::shared_ptr<Channel> channel)
        : stub_(ClientStreaming::NewStub(channel)) {
    }
    
    /**
     * SendMessage - 클라이언트 스트리밍 통신 수행
     * 
     * Python의 send_message(stub) 함수에 대응
     */
    void SendMessage() {
        // 서버로부터 받을 응답 객체
        Number response;
        
        // RPC 컨텍스트 생성
        ClientContext context;
        
        // 클라이언트 스트리밍을 위한 Writer 생성
        // Python의 stub.GetServerResponse(generate_messages())에 대응
        std::unique_ptr<ClientWriter<Message>> writer(
            stub_->GetServerResponse(&context, &response));
        
        // 전송할 메시지들
        // Python의 messages 리스트에 대응
        std::vector<std::string> messages = {
            "message #1",
            "message #2", 
            "message #3",
            "message #4",
            "message #5"
        };
        
        // 각 메시지 전송
        // Python의 generate_messages() 함수의 yield 부분에 대응
        for (const auto& text : messages) {
            Message msg;
            msg.set_message(text);
            
            std::cout << "[클라이언트 -> 서버] " << msg.message() << std::endl;
            
            // 스트림에 메시지 쓰기
            if (!writer->Write(msg)) {
                std::cerr << "메시지 전송 실패" << std::endl;
                break;
            }
        }
        
        // 모든 메시지 전송 완료를 서버에 알림
        writer->WritesDone();
        
        // RPC 완료 및 응답 받기
        Status status = writer->Finish();
        
        if (status.ok()) {
            // 서버로부터 받은 응답 출력
            // Python의 print("[server to client] %d" % response.value)에 대응
            std::cout << "[서버 -> 클라이언트] " << response.value() << std::endl;
            std::cout << "서버가 " << response.value() << "개의 메시지를 받았습니다." << std::endl;
        } else {
            std::cerr << "RPC 실패: " << status.error_message() << std::endl;
        }
    }
    
private:
    std::unique_ptr<ClientStreaming::Stub> stub_;
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
    
    ClientStreamingClient client(channel);
    client.SendMessage();
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
