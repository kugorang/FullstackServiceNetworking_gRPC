/**
 * gRPC Hello World Client
 * 
 * 이 클라이언트는 서버에 연결하여 숫자를 전송하고 제곱값을 받아옵니다.
 * Python client.py의 기능을 C++로 구현했습니다.
 */

#include <iostream>
#include <memory>
#include <string>

// gRPC 관련 헤더들
#include <grpcpp/grpcpp.h>

// protoc로 생성된 헤더 파일들
#include "hello_grpc.pb.h"
#include "hello_grpc.grpc.pb.h"

// gRPC 네임스페이스 사용 선언
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

/**
 * MyServiceClient 클래스
 * 
 * gRPC 클라이언트를 구현하는 클래스입니다.
 * 서버와의 통신을 담당하며, RPC 호출을 수행합니다.
 */
class MyServiceClient {
public:
    /**
     * 생성자
     * 
     * @param channel 서버와의 통신 채널
     * 
     * Python의 stub = hello_grpc_pb2_grpc.MyServiceStub(channel)에 대응
     */
    MyServiceClient(std::shared_ptr<Channel> channel)
        : stub_(MyService::NewStub(channel)) {
    }
    
    /**
     * MyFunction RPC 호출
     * 
     * @param value 서버에 전송할 숫자
     * @return 서버로부터 받은 결과값 (제곱값)
     * 
     * Python의 response = stub.MyFunction(request)에 대응
     */
    int32_t CallMyFunction(int32_t value) {
        // 요청 메시지 생성 및 값 설정
        // Python의 request = hello_grpc_pb2.MyNumber(value=4)에 대응
        MyNumber request;
        request.set_value(value);
        
        // 응답을 받을 메시지 객체
        MyNumber response;
        
        // RPC 호출을 위한 컨텍스트 생성
        // 타임아웃, 메타데이터 등을 설정할 수 있음
        ClientContext context;
        
        // 동기식 RPC 호출 수행
        // Python의 response = stub.MyFunction(request)에 대응
        Status status = stub_->MyFunction(&context, request, &response);
        
        // RPC 호출 성공 여부 확인
        if (status.ok()) {
            std::cout << "RPC 호출 성공!" << std::endl;
            return response.value();
        } else {
            // 오류 처리
            std::cerr << "RPC 호출 실패: " 
                      << status.error_code() << ": " 
                      << status.error_message() << std::endl;
            
            // 오류 발생 시 -1 반환 (실제 프로덕션에서는 예외 처리 권장)
            return -1;
        }
    }
    
private:
    // gRPC stub (원격 서비스의 로컬 프록시)
    // Python의 stub 변수에 대응
    std::unique_ptr<MyService::Stub> stub_;
};

/**
 * 메인 함수
 */
int main(int argc, char** argv) {
    // 서버 주소 설정
    // Python의 channel = grpc.insecure_channel('localhost:50051')에 대응
    std::string server_address = "localhost:50051";
    
    try {
        // gRPC 채널 생성 (보안 없는 연결)
        // InsecureChannelCredentials는 개발/테스트용
        // 프로덕션에서는 SslCredentials 사용 권장
        auto channel = grpc::CreateChannel(
            server_address, 
            grpc::InsecureChannelCredentials()
        );
        
        // 클라이언트 객체 생성
        MyServiceClient client(channel);
        
        // 서버에 전송할 값
        // Python의 MyNumber(value=4)에 대응
        int32_t input_value = 4;
        
        std::cout << "서버에 전송할 값: " << input_value << std::endl;
        
        // RPC 호출 수행
        int32_t result = client.CallMyFunction(input_value);
        
        // 결과 출력
        // Python의 print("gRPC result:", response.value)에 대응
        if (result != -1) {
            std::cout << "gRPC 결과: " << result << std::endl;
            std::cout << input_value << "의 제곱은 " << result << "입니다." << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "클라이언트 오류: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
