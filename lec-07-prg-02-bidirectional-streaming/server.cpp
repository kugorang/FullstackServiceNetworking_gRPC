/**
 * gRPC Bidirectional Streaming Server
 * 
 * 양방향 스트리밍을 구현하는 서버입니다.
 * 클라이언트로부터 메시지 스트림을 받으면서 동시에 응답 스트림을 전송합니다.
 * Python의 server.py를 C++로 변환했습니다.
 */

#include <iostream>
#include <memory>
#include <string>
#include <thread>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

// protoc로 생성된 헤더 파일들
#include "bidirectional.pb.h"
#include "bidirectional.grpc.pb.h"

// 네임스페이스 선언
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;

// bidirectional 네임스페이스 사용
using bidirectional::Bidirectional;
using bidirectional::Message;

/**
 * BidirectionalService 클래스
 * 
 * Python의 class BidirectionalService(bidirectional_pb2_grpc.BidirectionalServicer)에 대응
 * 양방향 스트리밍 RPC를 구현합니다.
 */
class BidirectionalService final : public Bidirectional::Service {
public:
    /**
     * GetServerResponse - 양방향 스트리밍 RPC 메서드
     * 
     * @param context gRPC 서버 컨텍스트
     * @param stream 양방향 스트림 객체 (읽기/쓰기 모두 가능)
     * @return Status gRPC 상태 코드
     * 
     * Python의 def GetServerResponse(self, request_iterator, context)에 대응
     * Python에서는 yield를 사용하지만, C++에서는 stream->Write()를 사용
     */
    Status GetServerResponse(ServerContext* context,
                           ServerReaderWriter<Message, Message>* stream) override {
        
        std::cout << "서버가 gRPC 양방향 스트리밍을 처리합니다." << std::endl;
        
        // 클라이언트로부터 메시지를 읽을 객체
        Message request;
        
        // 스트림에서 메시지를 읽는 루프
        // Python의 for message in request_iterator: 에 대응
        while (stream->Read(&request)) {
            // 받은 메시지 출력
            std::cout << "[클라이언트 -> 서버] " << request.message() << std::endl;
            
            // 동일한 메시지를 클라이언트에게 다시 전송 (에코)
            // Python의 yield message에 대응
            Message response;
            response.set_message(request.message());
            
            // 스트림에 응답 쓰기
            if (!stream->Write(response)) {
                // Write 실패 시 연결이 끊어진 것으로 간주
                std::cerr << "클라이언트 연결이 끊어졌습니다." << std::endl;
                break;
            }
            
            std::cout << "[서버 -> 클라이언트] " << response.message() << std::endl;
        }
        
        std::cout << "양방향 스트리밍 완료" << std::endl;
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
    BidirectionalService service;
    
    // gRPC reflection 활성화
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    
    // 서버 빌더 설정
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    // 스레드 풀 크기 설정 (Python의 max_workers=10에 대응)
    builder.SetMaxReceiveMessageSize(INT_MAX);
    builder.SetMaxSendMessageSize(INT_MAX);
    
    // 서버 시작
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "서버 시작. 포트 50051에서 리스닝 중입니다." << std::endl;
    
    // 종료 신호 대기
    server->Wait();
}

/**
 * 메인 함수
 * 
 * Python의 if __name__ == '__main__': 블록에 대응
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
