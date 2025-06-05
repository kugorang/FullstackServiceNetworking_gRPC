/**
 * gRPC Client Streaming Server
 * 
 * 클라이언트 스트리밍을 구현하는 서버입니다.
 * 클라이언트로부터 여러 메시지를 스트림으로 받고, 메시지 개수를 반환합니다.
 * Python의 server.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

// protoc로 생성된 헤더 파일들
#include "clientstreaming.pb.h"
#include "clientstreaming.grpc.pb.h"

// 네임스페이스 선언
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReader;

// clientstreaming 네임스페이스 사용
using clientstreaming::ClientStreaming;
using clientstreaming::Message;
using clientstreaming::Number;

/**
 * ClientStreamingService 클래스
 * 
 * Python의 class ClientStreamingServicer에 대응
 * 클라이언트 스트리밍 RPC를 구현합니다.
 */
class ClientStreamingService final : public ClientStreaming::Service {
public:
    /**
     * GetServerResponse - 클라이언트 스트리밍 RPC 메서드
     * 
     * @param context gRPC 서버 컨텍스트
     * @param reader 클라이언트로부터 메시지를 읽는 스트림 리더
     * @param response 클라이언트에게 보낼 단일 응답
     * @return Status gRPC 상태 코드
     * 
     * Python의 def GetServerResponse(self, request_iterator, context)에 대응
     */
    Status GetServerResponse(ServerContext* context,
                           ServerReader<Message>* reader,
                           Number* response) override {
        
        std::cout << "서버가 gRPC 클라이언트 스트리밍을 처리합니다." << std::endl;
        
        // 메시지 카운터
        // Python의 count = 0에 대응
        int32_t count = 0;
        
        // 클라이언트로부터 메시지를 읽을 객체
        Message request;
        
        // 스트림에서 모든 메시지 읽기
        // Python의 for message in request_iterator: 에 대응
        while (reader->Read(&request)) {
            std::cout << "[클라이언트 -> 서버] " << request.message() << std::endl;
            
            // 메시지 개수 증가
            // Python의 count += 1에 대응
            count++;
        }
        
        // 총 메시지 개수를 응답으로 설정
        // Python의 return clientstreaming_pb2.Number(value=count)에 대응
        response->set_value(count);
        
        std::cout << "총 " << count << "개의 메시지를 받았습니다." << std::endl;
        
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
    ClientStreamingService service;
    
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
