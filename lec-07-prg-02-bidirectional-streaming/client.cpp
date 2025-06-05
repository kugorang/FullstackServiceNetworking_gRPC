/**
 * gRPC Bidirectional Streaming Client
 * 
 * 양방향 스트리밍을 사용하는 클라이언트입니다.
 * 서버에 여러 메시지를 전송하면서 동시에 응답을 받습니다.
 * Python의 client.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>

// protoc로 생성된 헤더 파일들
#include "bidirectional.pb.h"
#include "bidirectional.grpc.pb.h"

// 네임스페이스 선언
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReaderWriter;

// bidirectional 네임스페이스 사용
using bidirectional::Bidirectional;
using bidirectional::Message;

/**
 * BidirectionalClient 클래스
 * 
 * 양방향 스트리밍을 구현하는 클라이언트입니다.
 */
class BidirectionalClient {
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     */
    BidirectionalClient(std::shared_ptr<Channel> channel)
        : stub_(Bidirectional::NewStub(channel)) {
    }
    
    /**
     * SendMessage - 양방향 스트리밍 통신 수행
     * 
     * Python의 send_message(stub) 함수에 대응
     */
    void SendMessage() {
        ClientContext context;
        
        // 양방향 스트림 생성
        // Python의 responses = stub.GetServerResponse(generate_messages())에 대응
        std::shared_ptr<ClientReaderWriter<Message, Message>> stream(
            stub_->GetServerResponse(&context));
        
        // 전송할 메시지들을 별도 스레드에서 전송
        // Python의 generate_messages() 함수를 스레드로 구현
        std::thread writer([stream]() {
            // Python의 messages 리스트에 대응
            std::vector<std::string> messages = {
                "message #1",
                "message #2",
                "message #3",
                "message #4",
                "message #5"
            };
            
            // 각 메시지 전송
            // Python의 for msg in messages: yield msg에 대응
            for (const auto& text : messages) {
                Message msg;
                msg.set_message(text);
                
                std::cout << "[클라이언트 -> 서버] " << msg.message() << std::endl;
                
                // 스트림에 메시지 쓰기
                if (!stream->Write(msg)) {
                    std::cerr << "메시지 전송 실패" << std::endl;
                    break;
                }
                
                // 메시지 간 약간의 지연 (옵션)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            // 더 이상 전송할 메시지가 없음을 알림
            stream->WritesDone();
        });
        
        // 서버로부터 응답 받기
        // Python의 for response in responses: 에 대응
        Message response;
        while (stream->Read(&response)) {
            std::cout << "[서버 -> 클라이언트] " << response.message() << std::endl;
        }
        
        // 쓰기 스레드 종료 대기
        writer.join();
        
        // 스트림 종료 및 상태 확인
        Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "RPC 실패: " << status.error_message() << std::endl;
        } else {
            std::cout << "양방향 스트리밍 완료" << std::endl;
        }
    }
    
private:
    std::unique_ptr<Bidirectional::Stub> stub_;
};

/**
 * run 함수
 * 
 * Python의 run() 함수에 대응
 */
void run() {
    std::string server_address = "localhost:50051";
    
    // 채널 생성
    // Python의 with grpc.insecure_channel('localhost:50051') as channel: 에 대응
    auto channel = grpc::CreateChannel(
        server_address, 
        grpc::InsecureChannelCredentials()
    );
    
    BidirectionalClient client(channel);
    client.SendMessage();
}

/**
 * 메인 함수
 * 
 * Python의 if __name__ == '__main__': 블록에 대응
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
