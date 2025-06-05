/**
 * gRPC Server Streaming Server
 * 
 * 서버 스트리밍을 구현하는 서버입니다.
 * 클라이언트로부터 단일 요청을 받고, 여러 메시지를 스트림으로 응답합니다.
 * Python의 server.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

// protoc로 생성된 헤더 파일들
#include "serverstreaming.pb.h"
#include "serverstreaming.grpc.pb.h"

// 네임스페이스 선언
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

// serverstreaming 네임스페이스 사용
using serverstreaming::ServerStreaming;
using serverstreaming::Message;
using serverstreaming::Number;

/**
 * ServerStreamingService 클래스
 * 
 * Python의 class ServerStreamingService에 대응
 * 서버 스트리밍 RPC를 구현합니다.
 */
class ServerStreamingService final : public ServerStreaming::Service {
public:
    /**
     * GetServerResponse - 서버 스트리밍 RPC 메서드
     * 
     * @param context gRPC 서버 컨텍스트
     * @param request 클라이언트로부터 받은 단일 요청
     * @param writer 클라이언트에게 메시지를 전송하는 스트림 writer
     * @return Status gRPC 상태 코드
     * 
     * Python의 def GetServerResponse(self, request, context)에 대응
     */
    Status GetServerResponse(ServerContext* context,
                           const Number* request,
                           ServerWriter<Message>* writer) override {
        
        // 받은 요청값 출력
        // Python의 print('Server processing gRPC server-streaming {%d}.' % request.value)에 대응
        std::cout << "서버가 gRPC 서버 스트리밍을 처리합니다 {" 
                  << request->value() << "}." << std::endl;
        
        // 전송할 메시지들
        // Python의 messages 리스트에 대응
        std::vector<std::string> messages = {
            "message #1",
            "message #2",
            "message #3",
            "message #4",
            "message #5"
        };
        
        // 각 메시지를 스트림으로 전송
        // Python의 for message in messages: yield message에 대응
        for (const auto& text : messages) {
            Message msg;
            msg.set_message(text);
            
            std::cout << "[서버 -> 클라이언트] " << msg.message() << std::endl;
            
            // 스트림에 메시지 쓰기
            if (!writer->Write(msg)) {
                std::cerr << "클라이언트 연결이 끊어졌습니다." << std::endl;
                return Status::CANCELLED;
            }
            
            // 메시지 간 약간의 지연 (옵션)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "서버 스트리밍 완료" << std::endl;
        return Status::OK;
    }
};

/**
 * 서버 실행 함수
 * 
 * Python의 serve() 함수에 대응
 */
void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ServerStreamingService service;
    
    // gRPC reflection 활성화
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    
    // 서버 빌더 설정
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    // 서버 시작
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "서버 시작. 포트 50051에서 리스닝 중입니다." << std::endl;
    
    // 종료 신호 대기
    server->Wait();
}

/**
 * 메인 함수
 */
int main(int argc, char** argv) {
    try {
        RunServer();
    } catch (const std::exception& e) {
        std::cerr << "서버 오류: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
