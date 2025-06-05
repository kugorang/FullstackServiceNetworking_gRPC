/**
 * gRPC Hello World Server
 * 
 * 이 서버는 클라이언트로부터 숫자를 받아서 제곱값을 반환하는 간단한 gRPC 서비스입니다.
 * Python의 hello_grpc.py에 있던 my_func() 함수의 기능을 C++로 구현했습니다.
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
// hello_grpc.pb.h : Protocol Buffer 메시지 정의 (MyNumber)
// hello_grpc.grpc.pb.h : gRPC 서비스 정의 (MyService)
#include "hello_grpc.pb.h"
#include "hello_grpc.grpc.pb.h"

// gRPC 네임스페이스 사용 선언
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

/**
 * MyServiceServicer 클래스
 * 
 * Python의 class MyServiceServicer(hello_grpc_pb2_grpc.MyServiceServicer)에 대응
 * 실제 RPC 메서드들을 구현하는 서비스 클래스입니다.
 * protoc가 생성한 MyService::Service를 상속받아 구현합니다.
 */
class MyServiceServicer final : public MyService::Service {
public:
    /**
     * MyFunction RPC 메서드 구현
     * 
     * @param context gRPC 서버 컨텍스트 (메타데이터, 취소 상태 등 포함)
     * @param request 클라이언트로부터 받은 요청 메시지 (MyNumber 타입)
     * @param response 클라이언트에게 보낼 응답 메시지 (MyNumber 타입)
     * @return Status gRPC 상태 코드 (OK, CANCELLED, INVALID_ARGUMENT 등)
     * 
     * Python의 def MyFunction(self, request, context)에 대응
     */
    Status MyFunction(ServerContext* context, 
                      const MyNumber* request,
                      MyNumber* response) override {
        
        // 클라이언트로부터 받은 값을 로그로 출력
        std::cout << "서버가 받은 값: " << request->value() << std::endl;
        
        // Python의 hello_grpc.my_func() 함수 로직을 여기서 직접 구현
        // 입력값의 제곱을 계산
        int32_t input_value = request->value();
        int32_t result = input_value * input_value;
        
        // 계산 결과를 response 메시지에 설정
        // Python의 response.value = ... 에 대응
        response->set_value(result);
        
        // 계산 결과를 로그로 출력
        std::cout << "서버가 보낼 값: " << response->value() << std::endl;
        
        // 성공 상태 반환
        return Status::OK;
    }
};

/**
 * gRPC 서버를 실행하는 함수
 * 
 * Python의 서버 초기화 및 실행 코드를 함수로 묶어서 구현
 */
void RunServer() {
    // 서버가 리스닝할 주소 설정
    // Python의 server.add_insecure_port('[::]:50051')에 대응
    std::string server_address("0.0.0.0:50051");
    
    // 서비스 인스턴스 생성
    MyServiceServicer service;
    
    // gRPC reflection 활성화 (디버깅 및 테스트용)
    // grpc_cli 같은 도구로 서비스를 동적으로 탐색 가능
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    
    // ServerBuilder를 사용하여 서버 구성
    ServerBuilder builder;
    
    // 리스닝 포트 설정 (보안 없는 연결)
    // InsecureServerCredentials는 개발/테스트용
    // 프로덕션에서는 SslServerCredentials 사용 권장
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // 서비스 등록
    // Python의 add_MyServiceServicer_to_server()에 대응
    builder.RegisterService(&service);
    
    // 서버 인스턴스 빌드 및 시작
    // Python의 server = grpc.server(...)와 server.start()에 대응
    std::unique_ptr<Server> server(builder.BuildAndStart());
    
    // 서버 시작 메시지 출력
    std::cout << "서버가 " << server_address << " 에서 시작되었습니다." << std::endl;
    
    // 서버가 종료될 때까지 대기
    // Python의 server.wait_for_termination()에 대응
    // Ctrl+C 또는 종료 신호를 받으면 자동으로 종료됨
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
